/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_pinching_.h"

void
nem_meshing_pinching (struct TESS Tess,
		      struct NODES Nodes, struct MESH *Mesh)
{
  int i, j, elt1d;
  double n1n2angle;
  int elt1dqty, *elts1d = NULL;
  int elt2dqty, *elts2d = NULL;
  int edgeqty, *edges = NULL;

  ut_print_message (0, 3, "Checking 2D mesh for pinching out...\n");

  // for every polyhedron, checking if it is pinched out somewhere.
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    neut_tess_poly_edges (Tess, i, &edges, &edgeqty);
    neut_mesh_elsets_elts (Mesh[1], edges, edgeqty, &elts1d, &elt1dqty);

    for (j = 0; j < elt1dqty; j++)
    {
      elt1d = elts1d[j];
      neut_mesh_elt1d_elsets2d_elts2d (Mesh[1], elt1d, Mesh[2],
	                               Tess.PolyFaceNb[i] + 1,
	                               Tess.PolyFaceQty[i],
				       &elts2d, &elt2dqty);
      if (elt2dqty != 2)
	abort ();
      neut_mesh_elt2dpair_angle (Nodes, Mesh[2], elts2d[0], elts2d[1],
				 &n1n2angle);
      if (ut_num_equal (n1n2angle, 180, 1e-3))
      {
	ut_print_message (0, 3, "poly %d: pinching between elts %d %d (faces %d %d)\n",
	        i, elts2d[0], elts2d[1], Mesh[2].EltElset[elts2d[0]],
		Mesh[2].EltElset[elts2d[1]]);
	abort ();
      }
    }
  }

  ut_free_1d_int (elts2d);
  ut_free_1d_int (elts1d);

  return;
}
