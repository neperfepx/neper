/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1d_.h"

void
nem_meshing_1d (struct IN_M In, struct MESHPARA *pMeshPara, struct TESS *pTess,
                struct NODES RNodes, struct MESH *RMesh, struct NODES *pNodes,
                struct MESH *Mesh)
{
  int i, edge, edgeqty, *edges = NULL, Node0DQty = (*pNodes).NodeQty;
  struct NODES *N =
    (struct NODES *) calloc ((*pTess).EdgeQty + 1, sizeof (struct NODES));
  struct MESH *M =
    (struct MESH *) calloc ((*pTess).EdgeQty + 1, sizeof (struct MESH));
  int **master_id = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
  double *Node0DCl = NULL;
  char *message = ut_alloc_1d_char (8);
  int **N_global_id = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);

  // This array records the cls of the 0D nodes.  Due to a tolerancy on
  // the element size in nem_meshing_1d_edge, elements a little larger than the
  // cl at the 0D nodes can be generated.  This requires to update the
  // 0D nodes to at least the size of the 1d elements they belong to.
  // Node0DCl records what should become the 0D node cls; they are updated
  // at the end.

  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    neut_nodes_set_zero (N + i);
    neut_mesh_set_zero (M + i);
  }

  Mesh[1] = neut_mesh_alloc (1, "tri", 1, 0, 0);

  ut_print_message (0, 2, "1D meshing... ");

  // edges to mesh; default is 'all'
  neut_tess_expr_edges (pTess, In.meshedge, &edges, &edgeqty);

  ut_print_progress (stdout, 0, (*pTess).EdgeQty, "%3.0f%%", message);

  nem_meshing_1d_pre (pMeshPara, (*pTess), RNodes, RMesh, *pNodes, &Node0DCl);

  // meshing edges
  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];

    if (strcmp ((*pTess).Type, "periodic") || !(*pTess).PerEdgeMaster[edge])
      nem_meshing_1d_edge (*pMeshPara, (*pTess), RNodes, RMesh, edge,
                           (*pMeshPara).edge_cl[edge], (*pMeshPara).pl, *pNodes,
                           Node0DCl, N + edge, M + edge);

    ut_print_progress (stdout, edge, (*pTess).EdgeQty, "%3.0f%%", message);
  }

  // slave edges, if any
  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];

    if (!strcmp ((*pTess).Type, "periodic") && (*pTess).PerEdgeMaster[edge])
      nem_meshing_1d_edge_per ((*pTess), N, M, N + edge, M + edge, master_id + edge, edge);
  }

  // Recording edge meshes in global mesh
  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];

    while (Mesh[1].ElsetQty != edge - 1)
      neut_mesh_addelset (Mesh + 1, NULL, 0);

    nem_meshing_1d_edge_record ((*pTess), edge, N[edge], M[edge], master_id[edge], pNodes,
                                N_global_id, Mesh);
  }

  nem_meshing_1d_post (pNodes, Mesh, Node0DQty, Node0DCl);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    neut_nodes_free (N + i);
  free (N);
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    neut_mesh_free (M + i);
  free (M);

  if (!strcmp ((*pTess).Type, "periodic"))
    neut_nodes_init_nodeslave (pNodes);

  ut_free_2d_int (&master_id, (*pTess).EdgeQty + 1);

  ut_free_1d_char (&message);
  ut_free_1d (&Node0DCl);
  ut_free_2d_int (&N_global_id, (*pTess).EdgeQty + 1);
  ut_free_1d_int (&edges);

  return;
}
