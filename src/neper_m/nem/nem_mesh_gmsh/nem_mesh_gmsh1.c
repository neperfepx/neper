/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_mesh_gmsh_.h"

int
nem_mesh_3d_gmsh (struct TESS Tess, int poly, struct NODES Nodes,
		  struct MESH *Mesh, double cl, double clreps,
		  char *gmsh, char *tmp, char
		  *algo, char *opti, double rnd, double allowed_t,
		  struct NODES *pN, struct MESH *pM,
		  double *pacl, struct timeval *pctrlc_t, double *pelapsed_t)
{
  FILE *file, *file2;
  char *Args = NULL;
  struct timeval beg_time, end_time;
  int nb, optinb, status;
  struct MESH M2;
  int **Elsets = NULL;
  int ElsetQty;
  int match;
  struct MESH Skin;
  struct MESH Garbage;
  struct PART PGarbage;
  char *filename = ut_alloc_1d_char (100);
  char *filename_surf = ut_alloc_1d_char (100);
  char *filename_msh = ut_alloc_1d_char (100);
  double clmod;
  int redo, iter;

#ifdef HAVE_OPENMP
  sprintf (filename, "%s/tmp%d-%d.geo", tmp, getpid (), omp_get_thread_num ());
  sprintf (filename_surf, "%s/tmp%d-%d-surf.msh", tmp, getpid (), omp_get_thread_num ());
  sprintf (filename_msh, "%s/tmp%d-%d.msh", tmp, getpid (), omp_get_thread_num ());
#else // HAVE_OPENMP
  sprintf (filename, "%s/tmp%d-1.geo", tmp, getpid ());
  sprintf (filename_surf, "%s/tmp%d-1-surf.msh", tmp, getpid ());
  sprintf (filename_msh, "%s/tmp%d-1.msh", tmp, getpid ());
#endif // HAVE_OPENMP

  double speed = 0.2;

  neut_nodes_free (pN);
  neut_mesh_free (pM);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);
  neut_mesh_set_zero (&Skin);
  neut_mesh_set_zero (&M2);
  neut_mesh_set_zero (&Garbage);
  neut_part_set_zero (&PGarbage);

  neut_gmsh_rc ("bak");

/***********************************************************************
 * writing 2D elements (w their nodes) in separate file */

  neut_mesh_poly_boundmesh (Tess, poly, Mesh[2], &Skin);

  file2 = ut_file_open (filename_surf, "W");
  neut_mesh_fprintf_gmsh (file2, "2", Nodes, Garbage, Garbage, Skin,
			  Garbage, PGarbage, Garbage, NULL, "binary");
  ut_file_close (file2, filename_surf, "W");

  clmod = cl;
  redo = 0;
  iter = 1;
  double prevacl = 1e32;
  int cross = 0;
  double lmean;

  do
  {
    // writing master file - general options + poly

    file = ut_file_open (filename, "W");
    fprintf (file, "// Poly %d\n", Tess.CellId ? Tess.CellId[poly] : poly);
    if (neut_gmsh_meshalgo3d_nb (algo, &nb) != 0)
    {
      printf ("\n");
      ut_print_message (2, 3, "Meshing algorithm `%s' unknown.\n", algo);
      abort ();
    }

    status = neut_gmsh_optialgo_nb (opti, &optinb);
    if (status == -1)
    {
      printf ("\n");
      ut_print_message (2, 3, "Mesh optimization algorithm `%s' unknown.\n",
			opti);
      abort ();
    }

    nem_mesh_gmsh_options (file, 1, nb, optinb, rnd);
    nem_mesh_3d_gmsh_writepoly (clmod, filename_surf, file);
    ut_file_close (file, filename, "W");

    // meshing
    gettimeofday (&beg_time, NULL);
    Args = ut_alloc_1d_char (1000);
    sprintf (Args,
	     "%s -3 -v 0 -order 1 %s -o %s > /dev/null 2> /dev/null",
	     gmsh, filename, filename_msh);
    ut_sys_runwtime (gmsh, Args, allowed_t, pctrlc_t);
    gettimeofday (&end_time, NULL);
    (*pelapsed_t) = ut_time_subtract (&beg_time, &end_time);
    ut_free_1d_char (Args);

  /***********************************************************************
   * reading 3D mesh */

    if (ut_file_exist (filename_msh) == 0)
    {
      status = 1;
      break;
    }
    else
    {
      status = 0;
      file = ut_file_open (filename_msh, "R");
      neut_mesh_fscanf_msh (file, pN, NULL, NULL, NULL, pM, NULL);
      ut_file_close (file, filename_msh, "R");
    }

    if ((*pM).EltQty == 0)
    {
      status = 1;
      break;
    }

    neut_mesh_eltlength (*pN, *pM, &lmean);

    // printf ("mean = %f vs cl = %f (clmod = %f qty = %d cross = %d)\n", acl, cl, clmod, (*pM).EltQty, cross);

    if ((prevacl > cl && lmean < cl) || (prevacl < cl && lmean > cl))
      cross++;

    redo = 0;
    // if ((iter == 1 && lmean > (1 + clreps) * cl)
    // || (iter >  1 && ut_num_requal (lmean, cl, clreps) == 0))
    if (ut_num_requal (lmean, cl, clreps) == 0)
    {
      if (lmean > cl)
	clmod /= (1 + speed / (cross + 1));
      else
	clmod *= (1 + speed / (cross + 1));
      redo = 1;
    }

    if (redo == 1)
    {
      neut_nodes_free (pN);
      neut_mesh_free (pM);
    }

    // printf ("\niter = %d clmod = %f lmean = %f cl = %f\n", iter, clmod, lmean, cl);
    // printf ("redo = %d\n", redo);
    iter++;
  }
  while (redo == 1 && cross < speed / clreps && clmod >= cl / 5);

  if (ut_file_exist (filename_msh) == 0)
    status = 1;
  else
  {
    status = 0;
    file = ut_file_open (filename_msh, "R");
    neut_mesh_fscanf_msh (file, pN, NULL, NULL, NULL, pM, NULL);
    ut_file_close (file, filename_msh, "R");

    if ((*pM).EltQty <= 0)
      status = 1;
    else
    {
      neut_mesh_Osize (*pN, *pM, cl, pacl);

      // nodes: skin nodes (numbers refer to *pN)
      neut_mesh_init_nodeelts (pM, (*pN).NodeQty);

      neut_mesh3d_mesh2d (*pN, *pM, &M2, &Elsets, &ElsetQty, 0);
      neut_mesh_mergeelsets (&M2);

      match = neut_mesh_cmp (Nodes, Skin, *pN, M2);

      if (match == 1)
	status = 0;
      else
	status = 2;		// means meshes do not match

      ut_free_2d_int (Elsets, ElsetQty);
    }
  }

  if (status == 0)
  {
#ifndef DEVEL_DEBUGGING_TEST
    remove (filename);
    remove (filename_surf);
    remove (filename_msh);
#endif
  }

  neut_mesh_free (&Skin);
  neut_mesh_free (&M2);
  ut_free_1d_char (filename);
  ut_free_1d_char (filename_msh);

  neut_gmsh_rc ("unbak");

  return status;
}
