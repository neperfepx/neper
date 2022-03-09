/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1d_.h"

void
nem_meshing_1d_edge_algo (struct TESS Tess, struct NODES RNodes,
                          struct MESH *RMesh, int edge, double cl, double pl,
                          struct NODES Nodes, double *Node0DCl,
                          struct NODES *pN, struct MESH *pM)
{
  int iter, itermax = 100;
  double l = Tess.EdgeLength[edge];
  struct NODES N0;
  struct MESH M0;

  neut_nodes_set_zero (&N0);
  neut_mesh_set_zero (&M0);

  // standard case
  if (RNodes.NodeQty == 0 && !neut_tess_edge_iscurved (Tess, edge))
    nem_meshing_1d_edge_algo_mesh (Tess.EdgeVerNb[edge][0],
                                   Tess.EdgeVerNb[edge][1], edge, l, cl, pl,
                                   Nodes, Node0DCl, pN, pM);

  // case of remeshing
  else if (RNodes.NodeQty > 0)
  {
    nem_meshing_1d_edge_algo_mesh (Tess.EdgeVerNb[edge][0],
                                   Tess.EdgeVerNb[edge][1], edge, l, cl, pl,
                                   Nodes, Node0DCl, pN, pM);

    neut_mesh_elset_mesh (RNodes, RMesh[1], edge, &N0, &M0, NULL);

    nem_meshing_1d_edge_projontomesh (Tess, edge, N0, M0, pN, *pM);
  }

  // case of curved edge
  // Since we do not know the geometry of the edge, we proceed by iterations,
  // by successive remeshing.
  else if (neut_tess_edge_iscurved (Tess, edge))
  {
    nem_meshing_1d_edge_algo_mesh (Tess.EdgeVerNb[edge][0],
                                   Tess.EdgeVerNb[edge][1], edge, l, cl, pl,
                                   Nodes, Node0DCl, pN, pM);

    nem_meshing_1d_edge_projontodomain (Tess, edge, pN, *pM);

    iter = 0;
    do
    {
      neut_nodes_memcpy (*pN, &N0);
      neut_mesh_memcpy (*pM, &M0);

      neut_mesh_elset_length (N0, M0, 1, Tess.EdgeLength + edge);

      nem_meshing_1d_edge_algo_mesh (Tess.EdgeVerNb[edge][0],
                                     Tess.EdgeVerNb[edge][1], edge,
                                     Tess.EdgeLength[edge], cl, pl, Nodes,
                                     Node0DCl, pN, pM);

      nem_meshing_1d_edge_projontomesh (Tess, edge, N0, M0, pN, *pM);
      nem_meshing_1d_edge_projontodomain (Tess, edge, pN, *pM);
    }
    while (N0.NodeQty != (*pN).NodeQty && ++iter < itermax);
  }

  neut_nodes_free (&N0);
  neut_mesh_free (&M0);

  return;
}

void
nem_meshing_1d_edge_record_nodes (struct TESS Tess, int edge, struct NODES N,
                                  int *master_id, int **N_global_id,
                                  struct NODES *pNodes)
{
  int i;

  // note: we check if the nodes are given in the sense of the edge,
  // or in the opposite sense.  Recording nodes accordingly...

  N_global_id[edge] = ut_alloc_1d_int (N.NodeQty + 1);
  N_global_id[edge][1] = Tess.EdgeVerNb[edge][0];
  N_global_id[edge][N.NodeQty] = Tess.EdgeVerNb[edge][1];
  for (i = 2; i < N.NodeQty; i++)
    N_global_id[edge][i] = (*pNodes).NodeQty + i - 1;

  /* adding body nodes to global nodes */
  for (i = 2; i < N.NodeQty; i++)
  {
    neut_nodes_addnode (pNodes, N.NodeCoo[i], N.NodeCl[i]);

    if (master_id)
      neut_nodes_markasslave (pNodes, (*pNodes).NodeQty,
                              N_global_id[Tess.
                                          PerEdgeMaster[edge]][master_id[i]],
                              Tess.PerEdgeShift[edge]);
  }

  return;
}

void
nem_meshing_1d_edge_record_elts (int edge, struct MESH M, int *N_global_id,
                                 struct MESH *Mesh)
{
  int i, j, *elt_nbs = NULL;

  /* renumbering mesh nodes to match global nodes */
  for (i = 1; i <= M.EltQty; i++)
    for (j = 0; j < 2; j++)
      M.EltNodes[i][j] = N_global_id[M.EltNodes[i][j]];

  /* renumbering mesh elts */
  elt_nbs = ut_alloc_1d_int (M.EltQty + 1);
  for (i = 1; i <= M.EltQty; i++)
    elt_nbs[i] = Mesh[1].EltQty + i;

  for (i = 1; i <= M.Elsets[1][0]; i++)
    M.Elsets[1][i] = elt_nbs[M.Elsets[1][i]];

  for (i = 1; i <= M.EltQty; i++)
    neut_mesh_addelt (Mesh + 1, M.EltNodes[i]);

  neut_mesh_addelset (Mesh + 1, M.Elsets[1] + 1, M.Elsets[1][0]);

  if (Mesh[1].ElsetQty != edge)
    ut_print_neperbug ();

  ut_free_1d_int (&elt_nbs);

  return;
}
