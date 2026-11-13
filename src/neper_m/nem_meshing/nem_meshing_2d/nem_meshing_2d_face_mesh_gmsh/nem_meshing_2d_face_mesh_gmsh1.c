/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2d_face_mesh_gmsh_.h"

// Meshing a face, from its boundary (edge) mesh, in a projection plane, using
// the mesh size map of its boundary nodes: their characteristic lengths
// increased by clgrad = ln (pl) times the distance, up to cl, and multiplied
// by the mesh size factor of the projection (in the Gmsh worker, see
// nem_mesh_gmsh_lib_2d).  As for the polys, cl and clgrad are scaled by a
// factor, calibrated so that the mean ratio of the elt lengths of the actual
// (back-projected) mesh to the mesh size map is 1 within the -mesh2dclreps
// value (see nem_mesh_gmsh_sizecalib_next); the characteristic lengths of the
// boundary nodes are not, as the boundary mesh is fixed.  The best mesh is
// kept.
// With split elts in 2D (see nem_meshing), the face is meshed as Gmsh does
// with its full-quad recombination: the boundary nodes passed to Gmsh are
// one in two (the others being the centres of the coarse boundary
// segments), the face is meshed at twice the mesh size, and the mesh is
// recombined (into quad elts and some tri elts, Blossom algorithm), then
// split into quad elts (see nem_meshing_2d_face_mesh_gmsh_split), and
// smoothed.  The calibration applies to the resulting mesh.
int
nem_meshing_2d_face_mesh_gmsh (struct TESS Tess, struct NODES RNodes,
                               struct MESH *RMesh, int face,
                               struct MESHPARA MeshPara, struct NODES Nodes,
                               struct MESH *Mesh, double cl, char *algo,
                               double rnd, double allowed_t, struct NODES *pN,
                               struct MESH *pM, int **pbnodes, int **plbnodes,
                               int *pbnodeqty, double *pacl,
                               struct timeval *pctrlc_t, double *pelapsed_t)
{
  int i, nb, status, done, best;
  struct timeval beg_time, end_time;
  double r, acl, elapsed_t;
  double **bnodecoos = NULL;
  int facttype;
  double factparms[NEM_GMSH_SIZEFACT_PARMQTY];
  struct NODES BNodes, BestN;
  struct MESH BestM;
  struct NEM_SIZECALIB Calib;
  int recombine = MeshPara.eltsplit && Tess.Dim == 2;
  int gqty, goffset = 0;
  double gfact = 1, **gcoos = NULL, *gcls = NULL;

  neut_nodes_set_zero (&BNodes);
  neut_nodes_set_zero (&BestN);
  neut_mesh_set_zero (&BestM);

  neut_nodes_reset (pN);
  neut_mesh_reset (pM);

  if (neut_gmsh_meshalgo2d_nb (algo, &nb) != 0)
  {
    printf ("\n");
    ut_print_message (2, 3, "Meshing algorithm `%s' unknown.\n", algo);
    abort ();
  }

// Building boundary mesh and projecting it ----------------------------

  nem_meshing_2d_face_mesh_gmsh_boundary (Tess, Nodes, Mesh, face, pbnodes,
                                          &bnodecoos, pbnodeqty);

  nem_meshing_2d_face_mesh_gmsh_proj (Tess, Nodes, face, MeshPara, *pbnodes,
                                      &bnodecoos, *pbnodeqty, &facttype, factparms);

  (*plbnodes) = ut_alloc_1d_int (*pbnodeqty);
  int *bestlbnodes = ut_alloc_1d_int (*pbnodeqty);

// Mesh size map: boundary nodes (actual coordinates) and their cls -----

  double clgrad = log (MeshPara.face_pl[face]);
  double *cls = ut_alloc_1d (*pbnodeqty);
  for (i = 0; i < *pbnodeqty; i++)
  {
    cls[i] = Nodes.NodeCl[(*pbnodes)[i]];
    neut_nodes_addnode (&BNodes, Nodes.NodeCoo[(*pbnodes)[i]], cls[i]);
  }

// Boundary nodes passed to Gmsh (one in two, with recombination) -------

  if (recombine)
  {
    gfact = 2;
    nem_meshing_2d_face_mesh_gmsh_coarse (Tess, Mesh, face, *pbnodes, bnodecoos, cls,
                                          *pbnodeqty, &goffset, &gcoos, &gcls, &gqty);
    ut_array_1d_scale (gcls, gqty, gfact);
  }
  else
  {
    gqty = *pbnodeqty;
    gcoos = ut_alloc_2d (gqty, 3);
    ut_array_2d_memcpy (bnodecoos, gqty, 3, gcoos);
    gcls = ut_alloc_1d (gqty);
    ut_array_1d_memcpy (cls, gqty, gcls);
  }

// Meshing (with calibration of the mesh size map) ----------------------

  (*pelapsed_t) = 0;
  nem_mesh_gmsh_sizecalib_init (&Calib);
  do
  {
    gettimeofday (&beg_time, NULL);
    status = nem_mesh_gmsh_lib_2d (nb, recombine, rnd, Calib.alpha * gfact * cl,
                                   Calib.alpha * gfact * clgrad, gcoos, gcls, gqty,
                                   facttype, factparms, allowed_t, pctrlc_t, pN, pM);
    gettimeofday (&end_time, NULL);
    // (longest request, as the allowed time applies to each request)
    elapsed_t = ut_time_subtract (&beg_time, &end_time);
    (*pelapsed_t) = ut_num_max (*pelapsed_t, elapsed_t);

    if (status || (*pM).EltQty <= 0)
      break;

    if (recombine)
      nem_meshing_2d_face_mesh_gmsh_split (gcoos, gqty, bnodecoos, *pbnodeqty, goffset,
                                           pN, pM);

    // boundary nodes of the mesh (in the projection plane), then
    // back-projection
    nem_meshing_2d_face_mesh_gmsh_bnodes (*pN, pM, bnodecoos, *pbnodeqty,
                                          *plbnodes);

    // (as Gmsh does after the split)
    if (recombine)
      nem_meshing_2d_face_mesh_gmsh_smooth (*pM, *plbnodes, *pbnodeqty, pN);

    nem_meshing_2d_face_mesh_gmsh_backproj (Tess, RNodes, RMesh, face, MeshPara,
                                            Nodes, *pbnodes, *plbnodes,
                                            *pbnodeqty, pN, *pM);

    r = nem_mesh_gmsh_sizeratio (BNodes, cl, clgrad, *pN, *pM);

    done = nem_mesh_gmsh_sizecalib_next (&Calib, r, MeshPara.mesh2dclreps[face],
                                         &best);

    if (best)
    {
      neut_nodes_memcpy (*pN, &BestN);
      neut_mesh_memcpy (*pM, &BestM);
      ut_array_1d_int_memcpy (*plbnodes, *pbnodeqty, bestlbnodes);
    }
  }
  while (!done);

// Recording the best mesh ----------------------------------------------

  if (Calib.besterr == DBL_MAX) // no mesh
    status = 1;
  else
  {
    status = 0;

    neut_nodes_memcpy (BestN, pN);
    neut_mesh_memcpy (BestM, pM);
    ut_array_1d_int_memcpy (bestlbnodes, *pbnodeqty, *plbnodes);

    // size quality, for the (unscaled) mesh size map
    neut_mesh_Osize_sizemap (*pN, *pM, BNodes, cl, clgrad, &acl);
#pragma omp critical
    (*pacl) = acl;

    // characteristic lengths of the nodes: values of the mesh size map
    // (those of the boundary nodes, at the boundary nodes), used for 3D
    // meshing
    (*pN).NodeCl = ut_realloc_1d ((*pN).NodeCl, (*pN).NodeQty + 1);
    for (i = 1; i <= (*pN).NodeQty; i++)
      (*pN).NodeCl[i] = neut_nodes_point_clgrad (BNodes, (*pN).NodeCoo[i], cl, clgrad);
    for (i = 0; i < *pbnodeqty; i++)
      (*pN).NodeCl[(*plbnodes)[i]] = cls[i];
  }

// Closing -------------------------------------------------------------

  neut_nodes_free (&BNodes);
  neut_nodes_free (&BestN);
  neut_mesh_free (&BestM);
  ut_free_2d (&bnodecoos, *pbnodeqty);
  ut_free_2d (&gcoos, gqty);
  ut_free_1d (&gcls);
  ut_free_1d (&cls);
  ut_free_1d_int (&bestlbnodes);

  return status;
}
