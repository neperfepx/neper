/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_duplicate (int *seeds, int seedqty,
			 int *nodes, int nodeqty,
			 struct NODES *pNodes, struct MESH *Mesh)
{
  int i, j, k, node, newnode, eltnodeqty, seed;
  int eltqty, *elts = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  eltnodeqty = neut_elt_nodeqty (Mesh[dim].EltType, Mesh[dim].Dimension,
      Mesh[dim].EltOrder);

  if (!(*pNodes).DupNodeSeed)
    (*pNodes).DupNodeSeed = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  for (j = 0; j < nodeqty; j++)
  {
    node = nodes[j];

    // attaching master dup node to the first seed.
    // slave dup nodes will be attached to other seeds.
    (*pNodes).DupNodeSeed[node] = seeds[0];

    // Starting from 1: the first seed is skipped.
    for (i = 1; i < seedqty; i++)
    {
      seed = seeds[i];

      newnode = neut_nodes_addnode (pNodes, (*pNodes).NodeCoo[node],
                                    (*pNodes).NodeCl ? (*pNodes).NodeCl[node] : -1);

      (*pNodes).DupNodeQty++;
      (*pNodes).DupNodeNb
	= ut_realloc_1d_int ((*pNodes).DupNodeNb,
			     (*pNodes).DupNodeQty + 1);
      (*pNodes).DupNodeNb[(*pNodes).DupNodeQty] = newnode;

      (*pNodes).DupNodeMaster
	= ut_realloc_1d_int ((*pNodes).DupNodeMaster, (*pNodes).NodeQty
	    + 1);
      (*pNodes).DupNodeMaster[(*pNodes).NodeQty] = node;

      (*pNodes).DupNodeSeed
	= ut_realloc_1d_int ((*pNodes).DupNodeSeed, (*pNodes).NodeQty + 1);
      (*pNodes).DupNodeSeed[(*pNodes).NodeQty] = seed;

      // Updating node numbers in the elements of the upper-dimension mesh

      neut_mesh_node_elset_elts (Mesh[dim], node, seed, &elts, &eltqty);

      // This can happen in the case of a periodic (slave) seed, i.e. a seed > CellQty
      // if (eltqty == 0)
      //   abort ();

      for (k = 0; k < eltqty; k++)
	ut_array_1d_int_findnreplace (Mesh[dim].EltNodes[elts[k]],
				      eltnodeqty, node, newnode);
    }
  }

  ut_free_1d_int (elts);

  return;
}
