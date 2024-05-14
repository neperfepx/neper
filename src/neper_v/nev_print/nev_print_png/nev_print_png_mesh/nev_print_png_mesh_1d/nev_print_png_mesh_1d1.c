/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_mesh_1d_.h"

void
nev_print_png_mesh_1d (FILE * file, struct PRINT Print, struct TESS Tess,
                       struct NODES Nodes, struct MESH *Mesh, struct DATA NodeData,
                       struct DATA **MeshData)
{
  int *hidden = NULL;

  if (ut_array_1d_int_sum (Print.showelt1d + 1, Mesh[1].EltQty) == 0)
    return;

  hidden = ut_alloc_1d_int (Mesh[1].EltQty + 1);

  nev_print_png_mesh_1d_compress (Print, Tess, Mesh, hidden);

  nev_print_png_mesh_1d_print (file, Print, Nodes, Mesh, NodeData, MeshData, hidden);

  ut_free_1d_int (&hidden);

  return;
}
