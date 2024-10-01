/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_smoothing_.h"

// Laplacian smoothing consists in modifying the coordinates of a node,
// X, using the coordinates of the neighboring nodes: X = (1 - A) X_n +
// A X_n where X_n is the barycentre of the neighboring nodes.  A
// belongs to [0, 1] and is an adjustable parameter. Several iterations
// are applied (itermax).  There is no stop criterion, itermax will
// always be reached.
void
nem_smoothing_laplacian (struct TESS Tess, struct NODES *pNodes,
                         struct MESH *Mesh, int dim, double A, int itermax,
                         char *nodetype)
{
  int i, j, NodeQty, nodeqty2, domtype;
  int *nodes = NULL;
  int *nodes2 = NULL;
  double **NodeCoo = NULL;

  if (dim <= 0)
    return;

  neut_mesh_nodes (Mesh[dim], &nodes, &NodeQty);
  neut_mesh_nodes (Mesh[dim - 1], &nodes2, &nodeqty2);

  for (i = 0; i < nodeqty2; i++)
    NodeQty -= ut_array_1d_int_rmelt (nodes, NodeQty, nodes2[i], 1);

  if (!strcmp (nodetype, "interior"))
  {
    for (i = 0; i < NodeQty; i++)
    {
      neut_mesh_node_domtype (Tess, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
                              nodes[i], &domtype);
      if (domtype != -1)
        nodes[i] = 0;
    }
    NodeQty -= ut_array_1d_int_rmelt (nodes, NodeQty, 0, INT_MAX);
  }

  NodeCoo = ut_alloc_2d (Mesh[dim].NodeQty + 1, 3);

  for (i = 0; i < itermax; i++)
  {
#pragma omp parallel for private (j) schedule(dynamic)
    for (j = 0; j < NodeQty; j++)
    {
      int neighnodeqty;
      int *neighnodes = NULL;
      double *neighnodeweights = NULL;
      double *NeighCoo = ut_alloc_1d (3);

      neut_mesh_node_neighnodes (Mesh[dim], nodes[j], &neighnodes,
                                 &neighnodeqty);
      nem_smoothing_neighnodeweights (*pNodes, Mesh, nodes[j], neighnodes,
                                      neighnodeqty, &neighnodeweights);
      nem_smoothing_barypos (*pNodes, neighnodes, neighnodeweights,
                             neighnodeqty, NeighCoo);

      nem_smoothing_newcoo ((*pNodes).NodeCoo[nodes[j]], NeighCoo, A,
                            NodeCoo[nodes[j]]);

      ut_free_1d_int (&neighnodes);
      ut_free_1d (&neighnodeweights);
      ut_free_1d (&NeighCoo);
    }

    nem_smoothing_nodes_updatecoo (pNodes, nodes, NodeQty, NodeCoo);
  }

  ut_free_1d_int (&nodes);
  ut_free_1d_int (&nodes2);
  ut_free_2d (&NodeCoo, Mesh[dim].NodeQty + 1);

  return;
}
