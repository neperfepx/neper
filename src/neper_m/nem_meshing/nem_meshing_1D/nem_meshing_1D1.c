/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1D_.h"

void
nem_meshing_1D (struct MESHPARA *pMeshPara, struct TESS Tess,
                struct NODES RNodes, struct MESH *RMesh, struct NODES *pNodes,
                struct MESH *Mesh)
{
  int i, Node0DQty = (*pNodes).NodeQty;
  struct NODES *N =
    (struct NODES *) calloc (Tess.EdgeQty + 1, sizeof (struct NODES));
  struct MESH *M =
    (struct MESH *) calloc (Tess.EdgeQty + 1, sizeof (struct MESH));
  int **master_id = ut_alloc_1d_pint (Tess.EdgeQty + 1);
  double *Node0DCl = NULL;
  char *message = ut_alloc_1d_char (8);
  int **N_global_id = ut_alloc_1d_pint (Tess.EdgeQty + 1);

  // This array records the cls of the 0D nodes.  Due to a tolerancy on
  // the element size in nem_meshing_1D_edge, elements a little larger than the
  // cl at the 0D nodes can be generated.  This requires to update the
  // 0D nodes to at least the size of the 1D elements they belong to.
  // Node0DCl records what should become the 0D node cls; they are updated
  // at the end.

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    neut_nodes_set_zero (N + i);
    neut_mesh_set_zero (M + i);
  }

  Mesh[1] = neut_mesh_alloc (1, "tri", 1, 0, 0);

  ut_print_message (0, 2, "1D meshing... ");

  ut_print_progress (stdout, 0, Tess.EdgeQty, "%3.0f%%", message);

  nem_meshing_1D_pre (pMeshPara, Tess, RNodes, RMesh, *pNodes, &Node0DCl);

  // meshing edges
  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    if (strcmp (Tess.Type, "periodic") || !Tess.PerEdgeMaster[i])
      nem_meshing_1D_edge (*pMeshPara, Tess, RNodes, RMesh, i,
                           (*pMeshPara).edge_cl[i], (*pMeshPara).pl, *pNodes,
                           Node0DCl, N + i, M + i);

    ut_print_progress (stdout, i, Tess.EdgeQty, "%3.0f%%", message);
  }

  // slave edges, if any
  for (i = 1; i <= Tess.EdgeQty; i++)
    if (!strcmp (Tess.Type, "periodic") && Tess.PerEdgeMaster[i])
      nem_meshing_1D_edge_per (Tess, N, M, N + i, M + i, master_id + i, i);

  // Recording edge meshes in global mesh
  for (i = 1; i <= Tess.EdgeQty; i++)
    nem_meshing_1D_edge_record (Tess, i, N[i], M[i], master_id[i], pNodes,
                                N_global_id, Mesh);

  nem_meshing_1D_post (pNodes, Mesh, Node0DQty, Node0DCl);

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    neut_mesh_free (M + i);
    neut_nodes_free (N + i);
  }
  free (M);
  free (N);

  neut_nodes_init_nodeslave (pNodes);

  ut_free_2d_int (&master_id, Tess.EdgeQty + 1);

  ut_free_1d_char (&message);
  ut_free_1d (&Node0DCl);
  ut_free_2d_int (&N_global_id, Tess.EdgeQty + 1);

  return;
}
