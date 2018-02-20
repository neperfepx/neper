/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_mesh_.h"

void
nev_print_mesh (FILE * file, struct PRINT Print,
		struct TESS Tess, struct NODES Nodes,
		struct MESH *Mesh,
		struct NODEDATA NodeData, struct MESHDATA *MeshData)
{
  int **printelt3dface = NULL;

  nev_print_mesh_3d (file, Print, Nodes, Mesh, NodeData, MeshData,
		     &printelt3dface);

  nev_print_mesh_2d (file, Print, Nodes, Mesh, NodeData, MeshData);

  nev_print_mesh_1d (file, Print, Tess, Mesh, NodeData, MeshData);

  nev_print_mesh_0d (file, Print, Mesh, NodeData, MeshData);

  nev_print_mesh_nodes (file, Print, Nodes, Mesh, NodeData);

  return;
}
