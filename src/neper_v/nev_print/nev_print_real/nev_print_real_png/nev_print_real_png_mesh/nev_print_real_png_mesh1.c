/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_png_mesh_.h"

void
nev_print_real_png_mesh (FILE * file, struct PRINT Print, struct TESS Tess,
                struct NODES Nodes, struct MESH *Mesh,
                struct DATA NodeData, struct DATA **MeshData)
{
  int **printelt3dface = NULL;

  nev_print_real_png_mesh_3d (file, Print, Nodes, Mesh, NodeData, MeshData,
                     &printelt3dface);

  nev_print_real_png_mesh_co (file, Print, Mesh, NodeData, MeshData);

  nev_print_real_png_mesh_2d (file, Print, Nodes, Mesh, NodeData, MeshData);

  nev_print_real_png_mesh_1d (file, Print, Tess, Nodes, Mesh, NodeData, MeshData);

  nev_print_real_png_mesh_0d (file, Print, Mesh, NodeData, MeshData);

  nev_print_real_png_mesh_nodes (file, Print, Nodes, Mesh, NodeData);

  ut_free_2d_int (&printelt3dface, Mesh[3].EltQty + 1);

  return;
}
