/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_smoothing_.h"

// Laplacian smoothing consists in modifying the coordinates of a node,
// X, using the coordinates of the neighbouring nodes: X = (1 - A) X_n +
// A X_n where X_n is the barycentre of the neighbouring nodes.  A
// belongs to [0, 1] and is an adjustable parameter. Several iterations
// are applied (itermax).  There is no stop criterion, itermax will
// always be reached.
void
nem_smoothing_laplacian (struct NODES *pNodes,
			 struct MESH *Mesh, int dim, double A, int itermax)
{
  int i, j, nodeqty, nodeqty2, neighnodeqty;
  double *NeighCoo = NULL;
  int *nodes = NULL, *neighnodes = NULL, *nodes2 = NULL;
  double *neighnodeweights = NULL;
  double **NodeCoo = NULL;

  if (dim <= 0)
    return;

  NeighCoo = ut_alloc_1d (3);

  neut_mesh_nodes (Mesh[dim], &nodes, &nodeqty);
  neut_mesh_nodes (Mesh[dim - 1], &nodes2, &nodeqty2);

  for (i = 0; i < nodeqty2; i++)
    nodeqty -= ut_array_1d_int_deletencompress (nodes, nodeqty, nodes2[i], 1);

  NodeCoo = ut_alloc_2d (Mesh[dim].NodeQty + 1, 3);

  for (i = 0; i < itermax; i++)
  {
    for (j = 0; j < nodeqty; j++)
    {
      neut_mesh_node_neighnodes (Mesh[dim], nodes[j], &neighnodes,
				 &neighnodeqty);
      nem_smoothing_neighnodeweights (*pNodes, Mesh, nodes[j], neighnodes,
				      neighnodeqty, &neighnodeweights);
      nem_smoothing_barypos (*pNodes, neighnodes, neighnodeweights,
			     neighnodeqty, NeighCoo);

      ut_free_1d_int (neighnodes);
      neighnodes = NULL;
      ut_free_1d (neighnodeweights);
      neighnodeweights = NULL;

      nem_smoothing_newcoo ((*pNodes).NodeCoo[nodes[j]], NeighCoo, A,
			    NodeCoo[nodes[j]]);
    }

    nem_smoothing_nodes_updatecoo (pNodes, nodes, nodeqty, NodeCoo);
  }

  ut_free_1d_int (nodes);
  ut_free_1d_int (nodes2);
  ut_free_1d (NeighCoo);
  ut_free_2d (NodeCoo, Mesh[dim].NodeQty + 1);

  return;
}
