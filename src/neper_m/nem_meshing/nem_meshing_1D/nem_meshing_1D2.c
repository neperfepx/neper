/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1D_.h"

void
EdgeMeshing (struct TESS Tess, int edge, double cl, double pl,
	     struct NODES Nodes, double *NodeCl,
	     struct NODES *pN, struct MESH *pM)
{
  int i, j, node1, node2, ptqty;
  double l = Tess.EdgeLength[edge];
  double cl1, cl2;
  double dist1, dist2;
  double *Coo = NULL;
  double *Cl = NULL;

  node1 = Tess.EdgeVerNb[edge][0];
  node2 = Tess.EdgeVerNb[edge][1];

  cl1 = Nodes.NodeCl[node1];
  cl2 = Nodes.NodeCl[node2];

  if (cl <= 1e-15 || pl < 1)
  {
    printf ("\n");
    printf ("Bad meshing parameters for edge %d: cl = %f pl = %f\n",
	    edge, cl, pl);
    ut_error_reportbug ();
  }

  // trick to support cl < cli
  if (cl1 > cl || cl2 > cl)
  {
    cl1 = ut_num_min (cl1, cl);
    cl2 = ut_num_min (cl2, cl);
  }

  // if cl1 > cl2, reversing edge before passing it to
  // nem_mesh_1d_unitsegment
  int reverse = (cl1 <= cl2) ? 0 : 1;

  if (reverse == 1)
    ut_num_switch (&cl1, &cl2);

  nem_mesh_1d_unitsegment (cl1 / l, cl2 / l, cl / l, pl, &ptqty, &Coo, &Cl);

  // if cl1 > cl2, reversing back the edge
  if (reverse == 1)
  {
    ut_array_1d_reverseelts (Coo, ptqty + 2);
    for (i = 0; i <= ptqty + 1; i++)
      Coo[i] = 1 - Coo[i];
    ut_array_1d_reverseelts (Cl, ptqty + 2);
  }

/***********************************************************************
 * recording N */

  neut_nodes_set_zero (pN);

  (*pN).NodeQty = 2 + ptqty;
  (*pN).NodeCoo = ut_alloc_2d (3 + ptqty, 3);
  (*pN).NodeCl = ut_alloc_1d (3 + ptqty);

  // recording coo of the boundary nodes (which are actually recorded as
  // 0D-mesh nodes)
  ut_array_1d_memcpy ((*pN).NodeCoo[1], 3, Nodes.NodeCoo[node1]);
  ut_array_1d_memcpy ((*pN).NodeCoo[ptqty + 2], 3, Nodes.NodeCoo[node2]);

  /* recording properties for the body nodes (coo + cl) */
  for (i = 1; i <= ptqty; i++)
  {
    /* i'st body node has pos i + 1 */
    for (j = 0; j < 3; j++)
      (*pN).NodeCoo[i + 1][j] =
	(1 - Coo[i]) * Nodes.NodeCoo[node1][j]
	+ Coo[i] * Nodes.NodeCoo[node2][j];

    (*pN).NodeCl[i + 1] = Cl[i] * l;
  }
  dist1 = ut_space_dist ((*pN).NodeCoo[1], (*pN).NodeCoo[2]);
  dist2 = ut_space_dist ((*pN).NodeCoo[ptqty + 1], (*pN).NodeCoo[ptqty + 2]);

  /*
     printf ("node %d: %f <- %f\n", node1, NodeCl[node1],
     ut_num_max (NodeCl[node1], dist1));
     printf ("node %d: %f <- %f\n", node2, NodeCl[node2],
     ut_num_max (NodeCl[node2], dist2));
   */
  NodeCl[node1] = ut_num_max (NodeCl[node1], dist1);
  NodeCl[node2] = ut_num_max (NodeCl[node2], dist2);

/***********************************************************************
 * recording M */

  neut_mesh_set_zero (pM);

  (*pM).Dimension = 1;
  (*pM).EltOrder = 1;
  (*pM).EltQty = ptqty + 1;

  (*pM).EltNodes = ut_alloc_2d_int ((*pM).EltQty + 1, 2);
  for (i = 1; i <= (*pM).EltQty; i++)
  {
    (*pM).EltNodes[i][0] = i;
    (*pM).EltNodes[i][1] = i + 1;
  }

  (*pM).ElsetQty = 1;
  (*pM).Elsets = ut_alloc_2d_int ((*pM).ElsetQty + 1, (*pM).EltQty + 1);
  (*pM).Elsets[1][0] = (*pM).EltQty;
  for (i = 1; i <= (*pM).EltQty; i++)
    (*pM).Elsets[1][i] = i;

  ut_free_1d (Coo);
  ut_free_1d (Cl);

  return;
}
