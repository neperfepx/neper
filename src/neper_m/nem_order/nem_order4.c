/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

// order-2 node of the edge of two order-1 nodes, -1 if none.  Unlike
// neut_mesh_order1nodes_order2node, all the elements of the two nodes
// are considered, as the two nodes can be in an element (e.g. a
// cohesive element) without forming one of its edges.
void
nem_order_periodic_dup_order2node (struct MESH *Mesh, int node1, int node2,
                                   int *pnode)
{
  int i, m, elt, pos1, pos2, nodeqty_o1, EltQty, *elts = NULL;
  int nodes[2] = { node1, node2 };
  int **sec = NULL;

  (*pnode) = -1;

  for (m = 1; m <= 4 && (*pnode) == -1; m++)
  {
    if (Mesh[m].EltQty == 0 || Mesh[m].EltOrder != 2 || !Mesh[m].NodeElts)
      continue;

    neut_elt_orderarrays (Mesh[m].EltType, Mesh[m].Dimension, NULL, NULL, &sec,
                          &nodeqty_o1);

    neut_mesh_nodes_comelts (Mesh[m], nodes, 2, &elts, &EltQty);

    for (i = 0; i < EltQty; i++)
    {
      elt = elts[i];
      pos1 = ut_array_1d_int_eltpos (Mesh[m].EltNodes[elt], nodeqty_o1, node1);
      pos2 = ut_array_1d_int_eltpos (Mesh[m].EltNodes[elt], nodeqty_o1, node2);

      if (pos1 != -1 && pos2 != -1 && sec[pos1][pos2] > 0)
      {
        (*pnode) = Mesh[m].EltNodes[elt][sec[pos1][pos2]];
        break;
      }
    }

    ut_free_2d_int (&sec, nodeqty_o1);
    ut_free_1d_int (&elts);
  }

  return;
}
