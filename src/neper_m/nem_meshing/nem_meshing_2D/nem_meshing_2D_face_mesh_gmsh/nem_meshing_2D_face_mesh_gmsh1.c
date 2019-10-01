/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2D_face_mesh_gmsh_.h"

int
nem_meshing_2D_face_mesh_gmsh (struct TESS Tess, struct NODES RNodes,
                               struct MESH *RMesh, int face, struct MESHPARA MeshPara,
                               struct NODES Nodes, struct MESH *Mesh,
                               double cl, char *gmsh, char *tmp, char *algo,
                               double rnd, double allowed_t, struct NODES *pN,
                               struct MESH *pM, int **pbnodes, int **plbnodes,
                               int *pbnodeqty, double *pacl,
                               struct timeval *pctrlc_t, double *pelapsed_t)
{
  FILE *file = NULL;
  int i, j, nb, status;
  struct timeval beg_time, end_time;
  double acl, *n = ut_alloc_1d (3);
  struct MESH M1D;
  int match;
  char *filename = ut_alloc_1d_char (100);
  char *filename_msh = ut_alloc_1d_char (100);
  double **bnodecoos = NULL;
  double *face_eq = MeshPara.face_eq ? MeshPara.face_eq[face] : NULL;

  neut_nodes_free (pN);
  neut_mesh_free (pM);
  neut_mesh_set_zero (pM);
  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);
  neut_mesh_set_zero (&M1D);

  neut_gmsh_rc ("bak");

// Opening geo input file ----------------------------------------------

#ifdef HAVE_OPENMP
  sprintf (filename, "%s/tmp%d-%d.geo", tmp, getpid (),
           omp_get_thread_num ());
  sprintf (filename_msh, "%s/tmp%d-%d.msh", tmp, getpid (),
           omp_get_thread_num ());
#else // HAVE_OPENMP
  sprintf (filename, "%s/tmp%d-1.geo", tmp, getpid ());
  sprintf (filename_msh, "%s/tmp%d-1.msh", tmp, getpid ());
#endif // HAVE_OPENMP

  file = ut_file_open (filename, "W");

// Writing meshing options ---------------------------------------------

  fprintf (file, "// face %d.\n", face);
  fprintf (file, "// face state = %d\n", Tess.FaceState[face]);
  if (neut_gmsh_meshalgo2d_nb (algo, &nb) != 0)
  {
    printf ("\n");
    ut_print_message (2, 3, "Meshing algorithm `%s' unknown.\n", algo);
    abort ();
  }

  nem_mesh_gmsh_options (file, nb, 4, 1, rnd);

// Building boundary mesh and projecting it ----------------------------

  nem_meshing_2D_face_mesh_gmsh_boundary (Tess, Nodes, Mesh, face,
                                          pbnodes, &bnodecoos, pbnodeqty);

  nem_meshing_2D_face_mesh_gmsh_proj (Tess, face, MeshPara, &bnodecoos,
                                      *pbnodeqty);

// Writing boundary mesh -----------------------------------------------

  nem_meshing_2D_face_mesh_gmsh_writeboundary (Nodes, *pbnodes, bnodecoos,
                                               *pbnodeqty, file);

  ut_file_close (file, filename, "W");

// Meshing -------------------------------------------------------------

  gettimeofday (&beg_time, NULL);
  char *Args = ut_alloc_1d_char (1000);
  sprintf (Args, "%s -2 -v 0 %s > /dev/null 2> /dev/null", gmsh, filename);

  ut_sys_runwtime (gmsh, Args, allowed_t, pctrlc_t);
  gettimeofday (&end_time, NULL);
#pragma omp critical
  (*pelapsed_t) = ut_time_subtract (&beg_time, &end_time);
  ut_free_1d_char (Args);

// Reading output mesh -------------------------------------------------

  if (ut_file_exist (filename_msh))
  {
    status = 0;
    file = ut_file_open (filename_msh, "R");
    neut_mesh_fscanf_msh (file, pN, NULL, NULL, pM, NULL, NULL);
    ut_file_close (file, filename_msh, "R");

    if ((*pM).EltQty <= 0)
      status = 1;
    else
    {
      neut_mesh_Osize (*pN, *pM, cl, &acl);
#pragma omp critical
      (*pacl) = acl;

      int node1d, *nodes1d = NULL, nodeqty1d;
      (*plbnodes) = ut_alloc_1d_int (*pbnodeqty);

      // reconstructing boundary mesh (node numbers refer to *pN)
      neut_mesh_init_nodeelts (pM, (*pN).NodeQty);
      neut_mesh2d_mesh1d (*pN, *pM, &M1D, NULL, NULL, NULL, 0);
      // neut_mesh_mergeelsets (&M1D); // is this useful?
      neut_mesh_nodes (M1D, &nodes1d, &nodeqty1d);

      int pos;
      double min_dist_max = 0, *dists = ut_alloc_1d (nodeqty1d);
      for (i = 0; i < *pbnodeqty; i++)
      {
        for (j = 0; j < nodeqty1d; j++)
        {
          node1d = nodes1d[j];
          dists[j] = ut_space_dist (bnodecoos[i], (*pN).NodeCoo[node1d]);
        }

        pos = ut_array_1d_min_index (dists, nodeqty1d);
        (*plbnodes)[i] = nodes1d[pos];
        if ((*plbnodes)[i] <= 0 || (*plbnodes)[i] > (*pN).NodeQty)
        {
          printf ("(*plbnodes)[i] = %d\n", (*plbnodes)[i]);
          printf ("(*pN).NodeQty = %d\n", (*pN).NodeQty);
          ut_error_reportbug ();
        }

        min_dist_max = ut_num_max (min_dist_max, dists[pos]);
      }
      ut_free_1d (dists);

      ut_free_1d_int_ (&nodes1d);

      if (*pbnodeqty == nodeqty1d && min_dist_max < 1e-9)
        match = 1;
      else
      {
        printf ("\n");
        printf ("*pbnodeqty = %d\n", *pbnodeqty);
        printf ("nodeqty1d = %d\n", nodeqty1d);
        printf ("min_dist_max = %f\n", min_dist_max);
        abort ();
      }

      if (match == 1)
        status = 0;
      else
        status = 2;             // means meshes do not match
    }
  }

  else
    status = 1;

// Fixing mesh ---------------------------------------------------------

// Back-projecting mesh ------------------------------------------------

  nem_meshing_2D_face_mesh_gmsh_backproj (Tess, RNodes, RMesh, face, MeshPara,
                                          Nodes, *pbnodes, *plbnodes,
                                          *pbnodeqty, pN, *pM);

  // aligning element normal to face normal (should be the case, but
  // not always)

  if (face_eq)
    for (i = 1; i <= (*pM).EltQty; i++)
    {
      neut_mesh_elt_normal (*pM, *pN, i, n);
      if (ut_vector_scalprod (face_eq + 1, n) < 0)
        ut_array_1d_int_switch ((*pM).EltNodes[i], 0, 2);
    }

// Closing -------------------------------------------------------------

  if (status == 0)
  {
    // commenting these two lines will break multimeshing
#ifndef DEVEL_DEBUGGING_TEST
    remove (filename);
    remove (filename_msh);
#endif
  }

#ifndef DEVEL_DEBUGGING_TEST
  remove ("debugp.pos");
  remove ("debugr.pos");
#endif

  neut_gmsh_rc ("unbak");

  ut_free_1d (n);
  neut_mesh_free (&M1D);
  ut_free_1d_char (filename);
  ut_free_1d_char (filename_msh);
  ut_free_2d (bnodecoos, *pbnodeqty);

  return status;
}
