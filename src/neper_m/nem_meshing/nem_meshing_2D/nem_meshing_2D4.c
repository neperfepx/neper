/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2D_.h"

void
nem_meshing_2D_face_proj (struct TESS Tess, struct NODES RNodes,
			  struct MESH *RMesh, int face,
			  double *face_eq, struct NODES *pN)
{
  struct NODES Nint;
  struct MESH Mint;

  neut_nodes_set_zero (&Nint);
  neut_mesh_set_zero (&Mint);

  if (Tess.FaceState[face] > 0 && RNodes.NodeQty == 0)
  {
    neut_tess_face_interpolmesh (Tess, face, &Nint, &Mint);
    neut_nodes_proj_alongontomesh (pN, face_eq, Nint, Mint, 1);
  }

  if (RNodes.NodeQty > 0)
    neut_nodes_proj_alongontomesh (pN, face_eq, RNodes, RMesh[2], face);

  neut_nodes_free (&Nint);
  neut_mesh_free (&Mint);

  return;
}

void
nem_meshing_2D_face_record_nodes (struct TESS Tess, int face, struct NODES N,
				  struct MESH M, int *master_id,
				  int **pnode_nbs, struct NODES *pNodes,
				  struct MESH *Mesh, struct MESHPARA MeshPara)
{
  int i, j, bak, nb, tmp, bnodeqty, *bnodes = NULL;
  double *dist = NULL;
  double *bnodedists = NULL;

  M = M;

  /* calculating the node numbers - some are already recorded ("1D"
   * nodes), the others are new.  We use the node positions. */

  neut_mesh_elsets_nodes (Mesh[1], Tess.FaceEdgeNb[face] + 1,
			  Tess.FaceVerQty[face], &bnodes, &bnodeqty);

  bnodedists = ut_alloc_1d (bnodeqty);

  dist = ut_alloc_1d (bnodeqty);

  for (i = 0; i < bnodeqty; i++)
  {
    for (j = 0; j < bnodeqty; j++)
      dist[j] = ut_space_dist ((*pNodes).NodeCoo[bnodes[i]],
			       (*pNodes).NodeCoo[bnodes[j]]);

    dist[i] = DBL_MAX;
    bnodedists[i] = ut_array_1d_min (dist, bnodeqty);
  }

  ut_free_1d_ (&dist);

  (*pnode_nbs) = ut_alloc_1d_int (N.NodeQty + 1);

  /* there are bnodeqty nodes whose positions in Nodes are
   * recorded in bnodes.  The nodes are at random places in N.
   * Searching them and their numbers, in turn. */

  dist = ut_alloc_1d (N.NodeQty + 1);

  /* for every skin node (whose pos in Nodes / coo is known, looking
   * for it in N. */
  for (i = 0; i < bnodeqty; i++)
  {
    for (j = 1; j <= N.NodeQty; j++)
      dist[j] = ut_space_dist ((*pNodes).NodeCoo[bnodes[i]], N.NodeCoo[j]);

    tmp = 1 + ut_array_1d_min_index (dist + 1, N.NodeQty);

    if (dist[tmp] > 1e-3 * bnodedists[i] || (*pnode_nbs)[tmp] != 0)
    {
      printf ("dist = %g > %g for parent node:", dist[tmp], 1e-3 * bnodedists[i]);
      ut_array_1d_fprintf (stdout, (*pNodes).NodeCoo[bnodes[i]], 3, "%f");
      printf ("projection along/onto:");
      ut_array_1d_fprintf (stdout, MeshPara.face_eq[face], 3, "%f");
      ut_array_1d_fprintf (stdout, Tess.FaceEq[face], 4, "%f");

      printf ("parent 1D nodes (%d):\n", bnodeqty);
      for (j = 0; j < bnodeqty; j++)
	ut_array_1d_fprintf (stdout, (*pNodes).NodeCoo[bnodes[j]], 3,
			     "%f");
      printf ("returned nodes (%d):\n", N.NodeQty);
      ut_array_2d_fprintf (stdout, N.NodeCoo + 1, N.NodeQty, 3, "%f");

      ut_error_reportbug ();
      abort ();
    }

    (*pnode_nbs)[tmp] = bnodes[i];
  }

  nb = (*pNodes).NodeQty;

  /* new nodes: their node_nbs are zero; filling with new values */
  tmp = ut_array_1d_int_eltpos ((*pnode_nbs) + 1, N.NodeQty, 0);
  while (tmp != -1)
  {
    tmp++;
    (*pnode_nbs)[tmp] = ++nb;
    tmp = ut_array_1d_int_eltpos ((*pnode_nbs) + 1, N.NodeQty, 0);
  }

  /* adding body nodes to global nodes */
  bak = (*pNodes).NodeQty;
  for (i = 1; i <= N.NodeQty; i++)
    if ((*pnode_nbs)[i] > bak)
    {
      neut_nodes_addnode (pNodes, N.NodeCoo[i], -1);

      if (master_id)
	neut_nodes_markasslave (pNodes, (*pNodes).NodeQty,
				master_id[i], Tess.PerFaceShift[face]);
    }

  ut_free_1d_int (bnodes);
  ut_free_1d (bnodedists);
  ut_free_1d (dist);

  return;
}

void
nem_meshing_2D_face_record_elts (int face, struct MESH M, int *node_nbs,
				 struct MESH *Mesh)
{
  int i, j;
  int *elt_nbs = NULL;

  /* renumbering mesh nodes to match global nodes */
  for (i = 1; i <= M.EltQty; i++)
    for (j = 0; j < 3; j++)
    {
      M.EltNodes[i][j] = node_nbs[M.EltNodes[i][j]];
      if (M.EltNodes[i][j] == 0)
	ut_error_reportbug ();
    }

  /* renumbering mesh elts */
  elt_nbs = ut_alloc_1d_int (M.EltQty + 1);
  for (i = 1; i <= M.EltQty; i++)
    elt_nbs[i] = Mesh[2].EltQty + i;

  for (j = 1; j <= M.Elsets[1][0]; j++)
    M.Elsets[1][j] = elt_nbs[M.Elsets[1][j]];

  for (i = 1; i <= M.EltQty; i++)
    neut_mesh_addelt (Mesh + 2, M.EltNodes[i]);

  neut_mesh_addelset (Mesh + 2, M.Elsets[1] + 1, M.Elsets[1][0]);

  if (Mesh[2].ElsetQty != face)
    ut_error_reportbug ();

  ut_free_1d_int (elt_nbs);

  return;
}
