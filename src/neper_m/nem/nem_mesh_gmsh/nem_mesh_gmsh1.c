/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_mesh_gmsh_.h"

// Meshing a poly, from its boundary (skin) mesh, using the mesh size map of
// its skin nodes: their characteristic lengths increased by clgrad times the
// distance, up to cl (see the Gmsh worker in nem_mesh_gmsh3.c).  As Gmsh's 3D
// mesher does not provide element lengths equal to the mesh size (they are
// generally larger), cl and clgrad are scaled by a factor, calibrated so that
// the mean ratio of the elt lengths to the mesh size map (at the elt centres)
// is 1 within clreps (see nem_mesh_gmsh_sizecalib_next); the characteristic
// lengths of the skin nodes are not, as the skin mesh is fixed.  The best
// mesh is kept.
int
nem_mesh_3d_gmsh (struct TESS Tess, int poly, struct NODES Nodes,
                  struct MESH *Mesh, double cl, double clgrad, double clreps,
                  char *algo, char *opti, double rnd, double allowed_t,
                  struct NODES *pN, struct MESH *pM, double *pacl,
                  struct timeval *pctrlc_t, double *pelapsed_t)
{
  int done, best, nb, optinb, status;
  int **Elsets = NULL;
  int ElsetQty;
  int match;
  double r, elapsed_t;
  struct NEM_SIZECALIB Calib;
  struct timeval beg_time, end_time;
  struct MESH M2, Skin, SkinLoc, BestM;
  struct NODES SkinNodes, BestN;

  neut_nodes_reset (pN);
  neut_mesh_reset (pM);

  neut_mesh_set_zero (&Skin);
  neut_mesh_set_zero (&SkinLoc);
  neut_mesh_set_zero (&BestM);
  neut_mesh_set_zero (&M2);
  neut_nodes_set_zero (&SkinNodes);
  neut_nodes_set_zero (&BestN);

  if (neut_gmsh_meshalgo3d_nb (algo, &nb) != 0)
  {
    printf ("\n");
    ut_print_message (2, 3, "Meshing algorithm `%s' unknown.\n", algo);
    abort ();
  }

  if (neut_gmsh_optialgo_nb (opti, &optinb) == -1)
  {
    printf ("\n");
    ut_print_message (2, 3, "Mesh optimization algorithm `%s' unknown.\n", opti);
    abort ();
  }

/***********************************************************************
 * building the 2D elements (w their nodes) to pass to the Gmsh library */

  neut_mesh_poly_boundmesh (Tess, poly, Mesh[2], &Skin);

  // only the skin nodes are passed (with a local numbering, and their
  // characteristic lengths)
  nem_mesh_3d_gmsh_localskin (Nodes, Skin, &SkinNodes, &SkinLoc);

  (*pelapsed_t) = 0;
  nem_mesh_gmsh_sizecalib_init (&Calib);
  do
  {
    gettimeofday (&beg_time, NULL);
    status = nem_mesh_gmsh_lib_3d (nb, optinb, rnd, Calib.alpha * cl,
                                   Calib.alpha * clgrad, SkinNodes, SkinLoc,
                                   allowed_t, pctrlc_t, pN, pM);
    gettimeofday (&end_time, NULL);
    // (longest request, as the allowed time applies to each request)
    elapsed_t = ut_time_subtract (&beg_time, &end_time);
    (*pelapsed_t) = ut_num_max (*pelapsed_t, elapsed_t);

    if (status || (*pM).EltQty == 0)
      break;

    r = nem_mesh_gmsh_sizeratio (SkinNodes, cl, clgrad, *pN, *pM);

    done = nem_mesh_gmsh_sizecalib_next (&Calib, r, clreps, &best);

    if (best)
    {
      neut_nodes_memcpy (*pN, &BestN);
      neut_mesh_memcpy (*pM, &BestM);
    }
  }
  while (!done);

  if (Calib.besterr == DBL_MAX) // no mesh
    status = 1;
  else
  {
    neut_nodes_memcpy (BestN, pN);
    neut_mesh_memcpy (BestM, pM);

    // size quality, for the (unscaled) mesh size map
    neut_mesh_Osize_sizemap (*pN, *pM, SkinNodes, cl, clgrad, pacl);

    // nodes: skin nodes (numbers refer to *pN)
    neut_mesh_init_nodeelts (pM, (*pN).NodeQty);

    neut_mesh3d_mesh2d (*pN, *pM, &M2, &Elsets, &ElsetQty, 0);
    neut_mesh_mergeelsets (&M2);

    match = neut_mesh_cmp (Nodes, Skin, *pN, M2);

    if (match == 1)
      status = 0;
    else
      status = 2;               // means meshes do not match

    ut_free_2d_int (&Elsets, ElsetQty);
  }

  neut_mesh_free (&SkinLoc);
  neut_nodes_free (&SkinNodes);
  neut_nodes_free (&BestN);
  neut_mesh_free (&BestM);
  neut_mesh_free (&Skin);
  neut_mesh_free (&M2);

  return status;
}
