/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_3d_.h"

void
nem_meshing_3d_poly_record_nodes (struct TESS Tess, int poly, struct NODES N,
                                  int **pnode_nbs, struct NODES *pNodes,
                                  struct MESH *Mesh)
{
  int i, j, bak, skinnodes_qty, nb, tmp;
  int *skinnodes = NULL;
  double *dist = NULL;

  /* calculating the node numbers - some are already recorded ("2D"
   * nodes), the others are new.  We use the node positions. */

  neut_mesh_elsets_nodes (Mesh[2], Tess.PolyFaceNb[poly] + 1,
                          Tess.PolyFaceQty[poly], &skinnodes, &skinnodes_qty);

  (*pnode_nbs) = ut_alloc_1d_int (N.NodeQty + 1);

  /* there are skinnodes_qty nodes whose positions in Nodes are
   * recorded in skinnodes.  The nodes are at random places in N.
   * Searching them and their numbers, in turn. */

  dist = ut_alloc_1d (N.NodeQty + 1);

  /* for every skin node (whose pos in Nodes / coo is known, looking
   * for it in N. */
  for (i = 0; i < skinnodes_qty; i++)
  {
    for (j = 1; j <= N.NodeQty; j++)
      dist[j] = ut_space_dist ((*pNodes).NodeCoo[skinnodes[i]], N.NodeCoo[j]);

    tmp = 1 + ut_array_1d_min_index (dist + 1, N.NodeQty);

    if (fabs (dist[tmp]) > 1e-11 || (*pnode_nbs)[tmp] != 0)
    {
      ut_print_message (2, 0,
                        "nem_meshing_3d: skin node not found or bad node_nbs\n");
      abort ();
    }

    (*pnode_nbs)[tmp] = skinnodes[i];
  }
  ut_free_1d_int (&skinnodes);

  nb = (*pNodes).NodeQty;

  /* new nodes: their node_nbs are zero; filling with new values */
  tmp = ut_array_1d_int_eltpos ((*pnode_nbs) + 1, N.NodeQty, 0);
  while (tmp > 0)
  {
    tmp++;
    (*pnode_nbs)[tmp] = ++nb;
    tmp = ut_array_1d_int_eltpos ((*pnode_nbs) + 1, N.NodeQty, 0);
  }

  /* adding body nodes to global nodes */
  bak = (*pNodes).NodeQty;
  for (i = 1; i <= N.NodeQty; i++)
    if ((*pnode_nbs)[i] > bak)
      neut_nodes_addnode (pNodes, N.NodeCoo[i], -1);

  ut_free_1d (&dist);

  return;
}

void
nem_meshing_3d_poly_record_elts (int poly, struct MESH M, int *node_nbs,
                                 struct MESH *Mesh)
{
  int i, j;
  int *elt_nbs = NULL;

  /* renumbering mesh nodes to match global nodes */
  for (i = 1; i <= M.EltQty; i++)
    for (j = 0; j < 4; j++)
      M.EltNodes[i][j] = node_nbs[M.EltNodes[i][j]];

  /* renumbering mesh elts */
  elt_nbs = ut_alloc_1d_int (M.EltQty + 1);
  for (i = 1; i <= M.EltQty; i++)
    elt_nbs[i] = Mesh[3].EltQty + i;

  for (j = 1; j <= M.Elsets[1][0]; j++)
    M.Elsets[1][j] = elt_nbs[M.Elsets[1][j]];

  for (i = 1; i <= M.EltQty; i++)
    neut_mesh_addelt (Mesh + 3, M.EltNodes[i]);

  neut_mesh_addelset (Mesh + 3, M.Elsets[1] + 1, M.Elsets[1][0]);

  if (Mesh[3].ElsetQty != poly)
    ut_print_neperbug ();

  ut_free_1d_int (&elt_nbs);

  return;
}
