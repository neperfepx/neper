/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_msh_.h"

void
neut_mesh_fprintf_msh_nodes_print_mesh (struct MESH Mesh, int *print)
{
  int i, j, eltnodeqty;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  for (i = 1; i <= Mesh.EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      print[Mesh.EltNodes[i][j]] = 1;

  return;
}
