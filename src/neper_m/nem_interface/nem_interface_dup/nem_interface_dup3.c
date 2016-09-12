/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_duplicate (int dim, int *cells, int cellqty,
			 int *nodes, int nodeqty,
			 struct NODES *pNodes, struct MESH *Mesh)
{
  int i, j, k, node, newnode, eltnodeqty, cell;
  int eltqty, *elts = NULL;

  eltnodeqty = neut_elt_nodeqty (Mesh[dim].EltType, Mesh[dim].Dimension,
      Mesh[dim].EltOrder);

  // Starting from 1: the first cell is skipped.
  for (i = 1; i < cellqty; i++)
  {
    cell = cells[i];

    for (j = 0; j < nodeqty; j++)
    {
      node = nodes[j];

      neut_nodes_addnode (pNodes, (*pNodes).NodeCoo[node],
				  (*pNodes).NodeCl[node]);
      newnode = (*pNodes).NodeQty;

      (*pNodes).DupNodeQty++;
      (*pNodes).DupNodeNb
	= ut_realloc_1d_int ((*pNodes).DupNodeNb,
			     (*pNodes).DupNodeQty + 1);
      (*pNodes).DupNodeNb[(*pNodes).DupNodeQty] = newnode;

      (*pNodes).DupNodeMaster
	= ut_realloc_1d_int ((*pNodes).DupNodeMaster, (*pNodes).NodeQty
	    + 1);
      (*pNodes).DupNodeMaster[(*pNodes).NodeQty] = node;

      (*pNodes).DupNodeCell
	= ut_realloc_1d_int ((*pNodes).DupNodeCell, (*pNodes).NodeQty + 1);
      (*pNodes).DupNodeCell[(*pNodes).NodeQty] = cell;

      neut_mesh_node_elset_elts (Mesh[dim], node, cell, &elts, &eltqty);
      if (eltqty == 0)
	abort ();

      for (k = 0; k < eltqty; k++)
	ut_array_1d_int_findnreplace (Mesh[dim].EltNodes[elts[k]],
				      eltnodeqty, node, newnode);
    }
  }

  ut_free_1d_int (elts);

  return;
}
