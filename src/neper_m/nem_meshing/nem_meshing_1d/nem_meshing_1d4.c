/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1d_.h"

void
nem_meshing_1d_edge_algo_mesh (int node1, int node2, int edge, double l,
                               double cl, double pl, struct NODES Nodes,
                               double *Node0DCl, struct NODES *pN,
                               struct MESH *pM)
{
  int i, j, ptqty, flip;
  double cl1, cl2, dist1, dist2, *Coo = NULL, *Cl = NULL;

  cl1 = Nodes.NodeCl[node1];
  cl2 = Nodes.NodeCl[node2];

  if (cl < 1e-15 || pl < 1)
  {
    printf ("\n");
    printf ("Bad meshing parameters for edge %d: cl = %f pl = %f\n", edge, cl,
            pl);
    ut_print_neperbug ();
  }

  // trick to support cl < cli
  if (cl1 > cl || cl2 > cl)
  {
    cl1 = ut_num_min (cl1, cl);
    cl2 = ut_num_min (cl2, cl);
  }

  // if cl1 > cl2, flipping edge before passing it to nem_mesh_1d_unitsegment
  flip = (cl1 > cl2);

  if (flip)
    ut_num_switch (&cl1, &cl2);

  nem_mesh_1d_unitsegment (cl1 / l, cl2 / l, cl / l, pl, &ptqty, &Coo, &Cl);

  // if cl1 > cl2, flipping back edge
  if (flip)
  {
    ut_array_1d_reverseelts (Coo, ptqty + 2);
    for (i = 0; i <= ptqty + 1; i++)
      Coo[i] = 1 - Coo[i];
    ut_array_1d_reverseelts (Cl, ptqty + 2);
  }

  // Recording N -------------------------------------------------------

  (*pN).NodeQty = 2 + ptqty;
  (*pN).NodeCoo = ut_alloc_2d (3 + ptqty, 3);
  (*pN).NodeCl = ut_alloc_1d (3 + ptqty);

  // recording coo of the boundary nodes (which are actually recorded as
  // 0D-mesh nodes)
  ut_array_1d_memcpy (Nodes.NodeCoo[node1], 3, (*pN).NodeCoo[1]);
  ut_array_1d_memcpy (Nodes.NodeCoo[node2], 3, (*pN).NodeCoo[ptqty + 2]);

  // recording properties for the body nodes (coo + cl)
  for (i = 1; i <= ptqty; i++)
  {
    // i'st body node has pos i + 1
    for (j = 0; j < 3; j++)
      (*pN).NodeCoo[i + 1][j] =
        (1 - Coo[i]) * Nodes.NodeCoo[node1][j] +
        Coo[i] * Nodes.NodeCoo[node2][j];

    (*pN).NodeCl[i + 1] = Cl[i] * l;
  }
  dist1 = ut_space_dist ((*pN).NodeCoo[1], (*pN).NodeCoo[2]);
  dist2 = ut_space_dist ((*pN).NodeCoo[ptqty + 1], (*pN).NodeCoo[ptqty + 2]);

  Node0DCl[node1] = ut_num_max (Node0DCl[node1], dist1);
  Node0DCl[node2] = ut_num_max (Node0DCl[node2], dist2);

  // Recording M -------------------------------------------------------

  neut_mesh_set_zero (pM);

  (*pM).Dimension = 1;
  (*pM).EltOrder = 1;
  (*pM).EltQty = ptqty + 1;

  (*pM).EltNodes = ut_alloc_2d_int ((*pM).EltQty + 1, 2);
  for (i = 1; i <= (*pM).EltQty; i++)
    ut_array_1d_int_set_2 ((*pM).EltNodes[i], i, i + 1);

  (*pM).ElsetQty = 1;
  (*pM).Elsets = ut_alloc_2d_int ((*pM).ElsetQty + 1, (*pM).EltQty + 1);
  ut_array_1d_int_set_id ((*pM).Elsets[1], (*pM).EltQty + 1);
  (*pM).Elsets[1][0] = (*pM).EltQty;

  ut_free_1d (&Coo);
  ut_free_1d (&Cl);

  return;
}

void
nem_meshing_1d_edge_projontomesh (struct TESS Tess, int edge, struct NODES N0,
                                  struct MESH M0, struct NODES *pN,
                                  struct MESH M)
{
  int i, j, elt, node1, node2, ver1, ver2;
  double l, alpha, eps;
  double *rccoo = NULL;
  double *ccoo = NULL;
  double length;

  // recording N0 node curvilinear coos
  rccoo = ut_alloc_1d (M0.Elsets[1][0] + 1);

  rccoo[0] = 0;
  l = 0;
  for (i = 1; i <= M0.Elsets[1][0]; i++)
  {
    elt = M0.Elsets[1][i];
    node1 = M0.EltNodes[elt][0];
    node2 = M0.EltNodes[elt][1];
    l += ut_space_dist (N0.NodeCoo[node1], N0.NodeCoo[node2]);
    rccoo[i] = l / Tess.EdgeLength[edge];
  }

  if (!ut_num_equal (rccoo[0], 0, 1e-6)
      || !ut_num_equal (rccoo[M0.Elsets[1][0]], 1, 1e-6))
    ut_print_neperbug ();

  // recording N0 node curvilinear coos
  ver1 = Tess.EdgeVerNb[edge][0];
  ver2 = Tess.EdgeVerNb[edge][1];

  length = ut_space_dist (Tess.VerCoo[ver1], Tess.VerCoo[ver2]);

  ccoo = ut_alloc_1d ((*pN).NodeQty);

  for (i = 0; i < (*pN).NodeQty; i++)
    ccoo[i] = ut_space_dist ((*pN).NodeCoo[1], (*pN).NodeCoo[i + 1]) / length;

  // for each *pN node, calculating its new coordinates in the M0

  eps = 1e-9;
  for (i = 0; i < (*pN).NodeQty; i++)
  {
    // looking for elt to project onto from the node coo
    elt = -1;
    alpha = 0;
    for (j = 0; j < M0.Elsets[1][0]; j++)
      if (ccoo[i] > rccoo[j] - eps && ccoo[i] < rccoo[j + 1] + eps)
      {
        elt = M0.Elsets[1][j + 1];
        alpha = (ccoo[i] - rccoo[j]) / (rccoo[j + 1] - rccoo[j]);
        break;
      }

    if (elt == -1)
      ut_print_neperbug ();

    node1 = M0.EltNodes[elt][0];
    node2 = M0.EltNodes[elt][1];

    for (j = 0; j < 3; j++)
      (*pN).NodeCoo[i + 1][j] =
        (1 - alpha) * N0.NodeCoo[node1][j] + alpha * N0.NodeCoo[node2][j];
  }

  nem_meshing_1d_edge_proj_fixcl (M, pN);

  ut_free_1d (&rccoo);
  ut_free_1d (&ccoo);

  return;
}

void
nem_meshing_1d_edge_projontodomain (struct TESS Tess, int edge,
                                    struct NODES *pN, struct MESH M)
{
  int i, domedge;
  int *domfaces = NULL, domfaceqty;

  // 2D
  if (Tess.Dim == 2)
  {
    domedge = Tess.EdgeDom[edge][0] != -1 ? Tess.EdgeDom[edge][1] : -1;

    if (domedge == -1)
      abort ();

    for (i = 2; i < (*pN).NodeQty; i++)
      neut_primparms_point_proj (Tess.DomEdgeType[domedge], Tess.DomEdgeParms[domedge],
                                 (*pN).NodeCoo[i], (*pN).NodeCoo[i]);

  }

  // 3D
  else
  {
    neut_tess_edge_domfaces (Tess, edge, &domfaces, &domfaceqty);

    for (i = 2; i < (*pN).NodeQty; i++)
      neut_tess_domfaces_point_proj (Tess, domfaces, domfaceqty,
                                     (*pN).NodeCoo[i], (*pN).NodeCoo[i]);

    ut_free_1d_int (&domfaces);
  }

  nem_meshing_1d_edge_proj_fixcl (M, pN);

  return;
}
