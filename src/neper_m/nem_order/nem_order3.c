/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

void
nem_order_dim_record (struct MESH *pMesh, int *nodes, int node_o2)
{
  int i, j, **sec = NULL, nodeqty_o1;
  int *pos = ut_alloc_1d_int (2);
  int comelt, *comelts = NULL, comeltqty;

  neut_elt_orderarrays ((*pMesh).EltType, (*pMesh).Dimension, NULL, NULL, &sec, &nodeqty_o1);

  neut_mesh_nodes_comelts (*pMesh, nodes, 2, &comelts, &comeltqty);

  for (i = 0; i < comeltqty; i++)
  {
    comelt = comelts[i];

    for (j = 0; j < 2; j++)
      pos[j] = ut_array_1d_int_eltpos ((*pMesh).EltNodes[comelt],
                                       nodeqty_o1, nodes[j]);
    (*pMesh).EltNodes[comelt][sec[pos[0]][pos[1]]] = node_o2;
  }

  ut_free_1d_int (pos);
  ut_free_1d_int (comelts);

  return;
}
