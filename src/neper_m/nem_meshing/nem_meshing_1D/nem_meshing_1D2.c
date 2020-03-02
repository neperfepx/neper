/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1D_.h"

void
nem_meshing_1D_pre (struct MESHPARA *pMeshPara, struct TESS Tess,
                    struct NODES RNodes, struct MESH *RMesh,
                    struct NODES Nodes, double **pNodeCl)
{
  int i, j, edge;
  struct NODES N;
  struct MESH M;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  (*pNodeCl) = ut_alloc_1d (Nodes.NodeQty + 1);
  ut_array_1d_memcpy (Nodes.NodeCl + 1, Nodes.NodeQty, (*pNodeCl) + 1);

  // in case of faces with < 3 edges, we want to make sure that the
  // face contour has >= 3 elts
  for (i = 1; i <= Tess.FaceQty; i++)
    if (Tess.FaceVerQty[i] > 0 && Tess.FaceVerQty[i] <= 2)
    {
      int eltqty = 0;
      double *edgel = ut_alloc_1d (Tess.FaceVerQty[i]);
      for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      {
        edge = Tess.FaceEdgeNb[i][j];
        nem_meshing_1D_edge (*pMeshPara, Tess, RNodes, RMesh, edge,
                             (*pMeshPara).edge_cl[edge], (*pMeshPara).pl,
                             Nodes, *pNodeCl, &N, &M);
        edgel[j - 1] = Tess.EdgeLength[edge];
        eltqty += M.EltQty;
      }
      int id = 1 + ut_array_1d_max_index (edgel, Tess.FaceVerQty[i]);
      edge = Tess.FaceEdgeNb[i][id];

      // if < 3, we add elt(s) to the longest edge
      if (eltqty < 3)
        (*pMeshPara).edge_cl[edge] =
          ut_num_min ((*pMeshPara).edge_cl[edge],
                      Tess.EdgeLength[edge] / (4 - Tess.FaceVerQty[i]));
    }

  neut_mesh_free (&M);
  neut_nodes_free (&N);

  return;
}

void
nem_meshing_1D_edge (struct MESHPARA MeshPara, struct TESS Tess,
                     struct NODES RNodes, struct MESH *RMesh, int edge,
                     double cl, double pl, struct NODES Nodes,
                     double *Node0DCl, struct NODES *pN, struct MESH *pM)
{
  // if the edge must be copied, we copy and escape
  if (!strcmp (MeshPara.edge_op[edge], "copy"))
    neut_mesh_elset_mesh (RNodes, RMesh[1], edge, pN, pM, NULL);

  else
    nem_meshing_1D_edge_algo (Tess, RNodes, RMesh, edge, cl, pl, Nodes,
                              Node0DCl, pN, pM);

  return;
}

void
nem_meshing_1D_edge_per (struct TESS Tess, struct NODES *N, struct MESH *M,
                         struct NODES *pN, struct MESH *pM, int **pmaster_id,
                         int edge)
{
  int master;

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  master = Tess.PerEdgeMaster[edge];
  neut_mesh_elset_mesh (N[master], M[master], 1, pN, pM, pmaster_id);
  neut_nodes_shift (N + edge,
                    Tess.PerEdgeShift[edge][0] * Tess.PeriodicDist[0],
                    Tess.PerEdgeShift[edge][1] * Tess.PeriodicDist[1],
                    Tess.PerEdgeShift[edge][2] * Tess.PeriodicDist[2]);

  if (Tess.PerEdgeOri[edge] == -1)
  {
    neut_nodes_reverse (N + edge);
    ut_array_1d_int_reverseelts (*pmaster_id + 1, N[edge].NodeQty);
  }
}

void
nem_meshing_1D_edge_record (struct TESS Tess, int edge, struct NODES N,
                            struct MESH M, int *master_id,
                            struct NODES *pNodes, int **N_global_id,
                            struct MESH *Mesh)
{
  nem_meshing_1D_edge_record_nodes (Tess, edge, N, master_id, N_global_id,
                                    pNodes);

  nem_meshing_1D_edge_record_elts (edge, M, N_global_id[edge], Mesh);

  return;
}

void
nem_meshing_1D_post (struct NODES *pNodes, struct MESH *Mesh, int Node0DQty,
                     double *Node0DCl)
{
  int i, j;
  double max, *cl = NULL;

  // Node0DCl may have been changed for some 0D nodes because of the 5%
  // tolerancy involved during 1D meshing.  Here, we are assiging all
  // periodic nodes the same cl, which is the maximum of all values (not
  // the master value).
  if ((*pNodes).PerNodeQty > 0)
    for (i = 1; i <= Node0DQty; i++)
      if ((*pNodes).PerNodeMaster[i] == 0)
      {
        cl = ut_alloc_1d ((*pNodes).PerNodeSlaveQty[i] + 1);
        cl[0] = Node0DCl[i];
        for (j = 1; j <= (*pNodes).PerNodeSlaveQty[i]; j++)
          cl[j] = Node0DCl[(*pNodes).PerNodeSlaveNb[i][j]];
        max = ut_array_1d_max (cl, (*pNodes).PerNodeSlaveQty[i] + 1);
        Node0DCl[i] = max;
        for (j = 1; j <= (*pNodes).PerNodeSlaveQty[i]; j++)
          Node0DCl[(*pNodes).PerNodeSlaveNb[i][j]] = max;
        ut_free_1d (&cl);
      }

  ut_array_1d_memcpy (Node0DCl + 1, Node0DQty, (*pNodes).NodeCl + 1);

  neut_mesh_init_nodeelts (Mesh + 1, (*pNodes).NodeQty);
  neut_mesh_init_eltelset (Mesh + 1, NULL);

  return;
}
