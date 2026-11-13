/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_png_mesh_co_.h"

// The cohesive elts (prisms in 3D, quadrangles in 2D, possibly
// of zero thickness) are printed as their faces (split into triangles) and
// their edges, skipping the degenerate ones.
void
nev_print_real_png_mesh_co (FILE * file, struct PRINT Print, struct MESH *Mesh,
                            struct DATA NodeData, struct DATA **MeshData)
{
  int edgeqty = 0, *edges = NULL, *elt2deltco = NULL;
  struct NODES N;
  struct MESH M2D;

  if (Mesh[4].EltQty == 0 || !Print.showeltco || Print.showeltco[0] <= 0)
    return;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M2D);

  nev_print_real_png_mesh_co_compress (Print, Mesh[4], NodeData, &N, &M2D, &elt2deltco,
                                       &edges, &edgeqty);

  ut_print_message (0, 4, "Number of cohesive elts printed: %d (%d faces, %d edges).\n",
                    Print.showeltco[0], M2D.EltQty, edgeqty);

  nev_print_real_png_mesh_co_print_faces (file, Print, N, M2D, elt2deltco, MeshData[4][0]);

  nev_print_real_png_mesh_co_print_edges (file, Print, N, edges, edgeqty, MeshData[4][0]);

  ut_free_1d_int (&elt2deltco);
  ut_free_1d_int (&edges);
  neut_nodes_free (&N);
  neut_mesh_free (&M2D);

  return;
}
