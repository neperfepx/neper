/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_smoothing_.h"

void
nem_smoothing_barypos (struct NODES Nodes,
		       int *neighnodes, double *neighnodeweights,
		       int neighnodeqty, double *NeighCoo)
{
  neut_nodes_wbary (Nodes, neighnodes, neighnodeweights, neighnodeqty,
		    NeighCoo, NULL);

  return;
}

void
nem_smoothing_newcoo (double *coo, double *neighcoo, double A, double *newcoo)
{
  int k;

  for (k = 0; k < 3; k++)
    newcoo[k] = (1 - A) * coo[k] + A * neighcoo[k];

  return;
}

void
nem_smoothing_nodes_updatecoo (struct NODES *pNodes, int *nodes, int nodeqty,
			       double **NodeCoo)
{
  int i;

  for (i = 0; i < nodeqty; i++)
    ut_array_1d_memcpy ((*pNodes).NodeCoo[nodes[i]], 3, NodeCoo[nodes[i]]);

  return;
}

void
nem_smoothing_neighnodeweights (struct NODES Nodes,
				struct MESH *Mesh,
				int node, int *neighnodes,
				int neighnodeqty, double **pneighnodeweights)
{
  int dim, i;

  (*pneighnodeweights) = ut_alloc_1d (neighnodeqty);

  dim = neut_mesh_node_dim (Mesh[0], Mesh[1], Mesh[2], Mesh[3], node);

  if (dim == 2)
    ut_array_1d_set (*pneighnodeweights, neighnodeqty, 1);
  else
    for (i = 0; i < neighnodeqty; i++)
    {
      (*pneighnodeweights)[i]
        = 1 / ut_space_dist (Nodes.NodeCoo[node], Nodes.NodeCoo[neighnodes[i]]);

      dim = neut_mesh_node_dim (Mesh[0], Mesh[1], Mesh[2], Mesh[3], neighnodes[i]);
      (*pneighnodeweights)[i] *= (dim <= 1) ? 10 : 1;
    }

  return;
}
