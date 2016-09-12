/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1D_.h"

void
nem_meshing_1D (struct MESHPARA MeshPara, struct TESS Tess,
		struct NODES RNodes, struct MESH *RMesh,
		struct NODES *pNodes, struct MESH *Mesh)
{
  int i, j, k, elt, master;
  int ver1, ver2, edge;
  struct NODES N;
  struct MESH M;
  int *node_nbs = NULL;
  int *elt_nbs = NULL;
  double l;
  char *message = ut_alloc_1d_char (8);
  int Node0DQty = (*pNodes).NodeQty;
  int *master_id = NULL;
  double *cl = NULL;
  double max;

  // This array records the cls of the 0D nodes.  Due to a tolerancy on
  // the element size in EdgeMeshing, elements a little larger than the
  // cl at the 0D nodes can be generated.  This requires to update the
  // 0D nodes to at least the size of the 1D elements they belong to. 
  // NodeCl records what should become the 0D node cls; they are updated
  // at the end.
  double *NodeCl = ut_alloc_1d (Node0DQty + 1);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (Mesh + 1);
  Mesh[1].Dimension = 1;
  Mesh[1].EltOrder = 1;
  Mesh[1].EltType = ut_alloc_1d_char (5);
  sprintf (Mesh[1].EltType, "tri");

  ut_print_message (0, 2, "1D meshing... ");
  ut_array_1d_memcpy (NodeCl + 1, Node0DQty, (*pNodes).NodeCl + 1);

  ut_print_progress (stdout, 0, Tess.EdgeQty, "%3.0f%%", message);

  // in case of faces with <= 2 edges, we want to make sure that the
  // face contour has >= 3 elts
  for (i = 1; i <= Tess.FaceQty; i++)
    if (Tess.FaceVerQty[i] > 0 && Tess.FaceVerQty[i] <= 2)
    {
      int eltqty = 0;
      double *edgel = ut_alloc_1d (Tess.FaceVerQty[i]);
      for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      {
	edge = Tess.FaceEdgeNb[i][j];
	EdgeMeshing (Tess, edge, MeshPara.edge_cl[edge], MeshPara.pl, *pNodes,
		     NodeCl, &N, &M);
	edgel[j - 1] = Tess.EdgeLength[edge];
	eltqty += M.EltQty;
      }
      int id = 1 + ut_array_1d_max_index (edgel, Tess.FaceVerQty[i]);
      edge = Tess.FaceEdgeNb[i][id];

      // if < 3, we add elt(s) to the longest edge
      if (eltqty < 3)
	MeshPara.edge_cl[edge]
	  = ut_num_min (MeshPara.edge_cl[edge],
			Tess.EdgeLength[edge] / (4 - Tess.FaceVerQty[i]));
    }

  // Meshing -----------------------------------------------------------
  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    if (strncmp (Tess.Type, "periodic", 8) != 0 || Tess.PerEdgeMaster[i] == 0)
    {
      if (MeshPara.edge_op == NULL || MeshPara.edge_op[i] == 0)
	EdgeMeshing (Tess, i, MeshPara.edge_cl[i], MeshPara.pl, *pNodes,
		     NodeCl, &N, &M);
      else
	neut_mesh_elset_mesh (RNodes, RMesh[1], i, &N, &M, NULL);

      // projecting nodes if needed (remeshing) --------------------------

      if (Tess.Dim > 1 && RNodes.NodeQty > 0
	  && (MeshPara.edge_op == NULL || MeshPara.edge_op[i] == 0))
      {
	int node1, node2;
	double alpha, eps;
	double *rccoo = NULL;
	double *ccoo = NULL;
	double length;

	// recording RNodes node curvilinear coos
	rccoo = ut_alloc_1d (RMesh[1].Elsets[i][0] + 1);

	rccoo[0] = 0;
	l = 0;
	for (j = 1; j <= RMesh[1].Elsets[i][0]; j++)
	{
	  elt = RMesh[1].Elsets[i][j];
	  node1 = RMesh[1].EltNodes[elt][0];
	  node2 = RMesh[1].EltNodes[elt][1];
	  l += ut_space_dist (RNodes.NodeCoo[node1], RNodes.NodeCoo[node2]);
	  rccoo[j] = l / Tess.EdgeLength[i];
	}

	if (fabs (rccoo[0]) > 1e-6
	    || fabs (rccoo[RMesh[1].Elsets[i][0]] - 1) > 1e-6)
	  ut_error_reportbug ();

	// recording RNodes node curvilinear coos
	ver1 = Tess.EdgeVerNb[i][0];
	ver2 = Tess.EdgeVerNb[i][1];

	length = ut_space_dist (Tess.VerCoo[ver1], Tess.VerCoo[ver2]);

	ccoo = ut_alloc_1d (N.NodeQty);

	for (j = 0; j < N.NodeQty; j++)
	  ccoo[j] = ut_space_dist (N.NodeCoo[1], N.NodeCoo[j + 1]) / length;

	// for each N node, calculating its new coordinates in the RMesh

	eps = 1e-9;
	for (j = 0; j < N.NodeQty; j++)
	{
	  // looking for elt to project onto from the node coo
	  elt = -1;
	  alpha = 0;
	  for (k = 0; k < RMesh[1].Elsets[i][0]; k++)
	    if (ccoo[j] > rccoo[k] - eps && ccoo[j] < rccoo[k + 1] + eps)
	    {
	      elt = RMesh[1].Elsets[i][k + 1];
	      alpha = (ccoo[j] - rccoo[k]) / (rccoo[k + 1] - rccoo[k]);
	      break;
	    }

	  if (elt == -1)
	    ut_print_reportbug ();

	  node1 = RMesh[1].EltNodes[elt][0];
	  node2 = RMesh[1].EltNodes[elt][1];

	  for (k = 0; k < 3; k++)
	    N.NodeCoo[j + 1][k] = (1 - alpha) * RNodes.NodeCoo[node1][k]
	      + alpha * RNodes.NodeCoo[node2][k];
	}

	ut_free_1d (rccoo);
	ut_free_1d (ccoo);
      }
    }
    else
    {
      master = Tess.PerEdgeMaster[i];
      neut_mesh_elset_mesh (*pNodes, Mesh[1], master, &N, &M, &master_id);

      neut_nodes_shift (&N,
			Tess.PerEdgeShift[i][0] * Tess.PeriodicDist[0],
			Tess.PerEdgeShift[i][1] * Tess.PeriodicDist[1],
			Tess.PerEdgeShift[i][2] * Tess.PeriodicDist[2]);

      if (Tess.PerEdgeOri[i] == -1)
      {
	neut_nodes_reverse (&N);
	ut_array_1d_int_reverseelts (master_id + 1, N.NodeQty);
      }
    }

    // sticking nodes to the global nodes ------------------------------

    node_nbs = ut_alloc_1d_int (N.NodeQty + 1);
    // note: we check if the nodes are given in the sense of the edge,
    // or in the opposite sense.  Recording nodes accordingly...

    node_nbs[1] = Tess.EdgeVerNb[i][0];
    node_nbs[N.NodeQty] = Tess.EdgeVerNb[i][1];
    for (j = 2; j < N.NodeQty; j++)
      node_nbs[j] = (*pNodes).NodeQty + j - 1;

    /* adding body nodes to global nodes */
    for (j = 2; j < N.NodeQty; j++)
    {
      neut_nodes_addnode (pNodes, N.NodeCoo[j], N.NodeCl[j]);

      if (master_id)
	neut_nodes_markasslave (pNodes, (*pNodes).NodeQty, master_id[j],
				Tess.PerEdgeShift[i]);
    }

    /* renumbering mesh nodes to match global nodes */
    for (j = 1; j <= M.EltQty; j++)
      for (k = 0; k < 2; k++)
	M.EltNodes[j][k] = node_nbs[M.EltNodes[j][k]];

    ut_free_1d_int (node_nbs);

    /* renumbering mesh elts */
    elt_nbs = ut_alloc_1d_int (M.EltQty + 1);
    for (j = 1; j <= M.EltQty; j++)
      elt_nbs[j] = Mesh[1].EltQty + j;

    for (k = 1; k <= M.Elsets[1][0]; k++)
      M.Elsets[1][k] = elt_nbs[M.Elsets[1][k]];

    ut_free_1d_int (elt_nbs);

    for (j = 1; j <= M.EltQty; j++)
      neut_mesh_addelt (Mesh + 1, M.EltNodes[j]);

    neut_mesh_addelset (Mesh + 1, M.Elsets[1] + 1, M.Elsets[1][0]);

    ut_print_progress (stdout, i, Tess.EdgeQty, "%3.0f%%", message);

    neut_mesh_free (&M);
    neut_nodes_free (&N);
    ut_free_1d_int (master_id);
    master_id = NULL;
  }

  // NodeCl may have been changed for some 0D nodes because of the 5%
  // tolerancy involved during 1D meshing.  Here, we are assiging all
  // periodic nodes the same cl, which is the maximum of all values (not
  // the master value).
  if ((*pNodes).PerNodeQty > 0)
    for (i = 1; i <= Node0DQty; i++)
      if ((*pNodes).PerNodeMaster[i] == 0)
      {
	cl = ut_alloc_1d ((*pNodes).PerNodeSlaveQty[i] + 1);
	cl[0] = NodeCl[i];
	for (j = 1; j <= (*pNodes).PerNodeSlaveQty[i]; j++)
	  cl[j] = NodeCl[(*pNodes).PerNodeSlaveNb[i][j]];
	max = ut_array_1d_max (cl, (*pNodes).PerNodeSlaveQty[i] + 1);
	NodeCl[i] = max;
	for (j = 1; j <= (*pNodes).PerNodeSlaveQty[i]; j++)
	  NodeCl[(*pNodes).PerNodeSlaveNb[i][j]] = max;
	ut_free_1d (cl);
	cl = NULL;
      }

  ut_array_1d_memcpy ((*pNodes).NodeCl + 1, Node0DQty, NodeCl + 1);

  neut_mesh_init_nodeelts (Mesh + 1, (*pNodes).NodeQty);
  neut_mesh_init_eltelset (Mesh + 1, NULL);

  ut_free_1d_char (message);
  ut_free_1d (NodeCl);

  return;
}
