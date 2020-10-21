/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_nodes_.h"

void
neut_nodes_scale (struct NODES *pNodes, double scalex, double scaley,
                  double scalez)
{
  int i;

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    (*pNodes).NodeCoo[i][0] *= scalex;
    (*pNodes).NodeCoo[i][1] *= scaley;
    (*pNodes).NodeCoo[i][2] *= scalez;
  }

  return;
}

void
neut_nodes_shift (struct NODES *pNodes, double shiftx, double shifty,
                  double shiftz)
{
  int i;

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    (*pNodes).NodeCoo[i][0] += shiftx;
    (*pNodes).NodeCoo[i][1] += shifty;
    (*pNodes).NodeCoo[i][2] += shiftz;
  }

  return;
}

void
neut_nodes_rotate (struct NODES *pNodes, double r1, double r2, double r3,
                   double theta)
{
  int i;
  double **g = ol_g_alloc ();
  double *r = ol_r_alloc ();

  ol_r_set_this (r, r1, r2, r3);
  ol_rtheta_g (r, -theta, g);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    ol_g_vect_vect (g, (*pNodes).NodeCoo[i], (*pNodes).NodeCoo[i]);

  ol_g_free (g);
  ol_r_free (r);

  return;
}

void
neut_nodes_bbox (struct NODES Nodes, double **bbox)
{
  int *nodes = ut_alloc_1d_int (Nodes.NodeQty);

  ut_array_1d_int_set_id (nodes, Nodes.NodeQty);

  neut_nodes_nodes_bbox (Nodes, nodes, Nodes.NodeQty, bbox);

  ut_free_1d_int (&nodes);

  return;
}

void
neut_nodes_nodes_bbox (struct NODES Nodes, int *nodes, int NodeQty, double **bbox)
{
  int i, j, node;

  for (i = 0; i < 3; i++)
  {
    bbox[i][0] = DBL_MAX;
    bbox[i][1] = -DBL_MAX;
  }

  for (i = 0; i < NodeQty; i++)
  {
    node = nodes[i];
    for (j = 0; j < 3; j++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], Nodes.NodeCoo[node][j]);
      bbox[j][1] = ut_num_max (bbox[j][1], Nodes.NodeCoo[node][j]);
    }
  }

  return;
}

void
neut_nodes_bbox_vect (struct NODES Nodes, double *bbox)
{
  int i;

  for (i = 0; i < 3; i++)
  {
    bbox[2 * i] = 1e32;
    bbox[2 * i + 1] = -1e32;
  }

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    bbox[0] = ut_num_min (bbox[0], Nodes.NodeCoo[i][0]);
    bbox[1] = ut_num_max (bbox[1], Nodes.NodeCoo[i][0]);
    bbox[2] = ut_num_min (bbox[2], Nodes.NodeCoo[i][1]);
    bbox[3] = ut_num_max (bbox[3], Nodes.NodeCoo[i][1]);
    bbox[4] = ut_num_min (bbox[4], Nodes.NodeCoo[i][2]);
    bbox[5] = ut_num_max (bbox[5], Nodes.NodeCoo[i][2]);
  }

  return;
}

void
neut_nodes_set_zero (struct NODES *pNodes)
{
  (*pNodes).NodeQty = 0;
  (*pNodes).NodeCoo = NULL;
  (*pNodes).NodeCl = NULL;

  (*pNodes).Periodic = ut_alloc_1d_int (3);
  (*pNodes).PeriodicDist = ut_alloc_1d (3);

  (*pNodes).PerNodeQty = 0;
  (*pNodes).PerNodeNb = NULL;
  (*pNodes).PerNodeMaster = NULL;
  (*pNodes).PerNodeShift = NULL;
  (*pNodes).PerNodeSlaveQty = NULL;
  (*pNodes).PerNodeSlaveNb = NULL;

  (*pNodes).DupNodeQty = 0;
  (*pNodes).DupNodeNb = NULL;
  (*pNodes).DupNodeMaster = NULL;
  (*pNodes).DupNodeSeed = NULL;
  (*pNodes).DupNodeSlaveQty = NULL;
  (*pNodes).DupNodeSlaveNb = NULL;

  (*pNodes).PartQty = 0;
  (*pNodes).NodePart = NULL;
  (*pNodes).Parts = NULL;

  return;
}

void
neut_nodes_free (struct NODES *pNodes)
{
  if (!pNodes)
    return;

  ut_free_2d (&(*pNodes).NodeCoo, (*pNodes).NodeQty + 1);
  ut_free_1d (&(*pNodes).NodeCl);

  ut_free_1d_int (&(*pNodes).Periodic);
  ut_free_1d (&(*pNodes).PeriodicDist);

  ut_free_1d_int (&(*pNodes).NodePart);
  ut_free_2d_int (&(*pNodes).Parts, (*pNodes).PartQty + 1);

  ut_free_1d_int (&(*pNodes).PerNodeNb);
  ut_free_1d_int (&(*pNodes).PerNodeMaster);
  ut_free_2d_int (&(*pNodes).PerNodeShift, (*pNodes).NodeQty + 1);
  ut_free_2d_int (&(*pNodes).PerNodeSlaveNb, (*pNodes).NodeQty + 1);

  ut_free_1d_int (&(*pNodes).DupNodeNb);
  ut_free_1d_int (&(*pNodes).DupNodeMaster);
  ut_free_1d_int (&(*pNodes).DupNodeSeed);
  ut_free_1d_int (&(*pNodes).DupNodeSlaveQty);
  ut_free_2d_int (&(*pNodes).DupNodeSlaveNb, (*pNodes).NodeQty + 1);

  return;
}

void
neut_nodes_reset (struct NODES *pNodes)
{
  neut_nodes_free (pNodes);

  neut_nodes_set_zero (pNodes);

  return;
}

int
neut_nodes_addnode (struct NODES *pNodes, double *NodeCoo, double NodeCl)
{
  (*pNodes).NodeQty++;

  (*pNodes).NodeCoo =
    ut_realloc_2d_addline ((*pNodes).NodeCoo, (*pNodes).NodeQty + 1, 3);
  (*pNodes).NodeCoo[0] = NULL;  /* mandatory */

  ut_array_1d_memcpy (NodeCoo, 3, (*pNodes).NodeCoo[(*pNodes).NodeQty]);

  (*pNodes).NodeCl = ut_realloc_1d ((*pNodes).NodeCl, (*pNodes).NodeQty + 1);
  (*pNodes).NodeCl[(*pNodes).NodeQty] = NodeCl;

  if ((*pNodes).Periodic && ut_array_1d_int_sum ((*pNodes).Periodic, 3) > 0)
  {
    (*pNodes).PerNodeMaster =
      ut_realloc_1d_int ((*pNodes).PerNodeMaster, (*pNodes).NodeQty + 1);
    (*pNodes).PerNodeMaster[(*pNodes).NodeQty] = 0;

    (*pNodes).PerNodeShift =
      ut_realloc_2d_int_addline ((*pNodes).PerNodeShift,
                                 (*pNodes).NodeQty + 1, 3);
    (*pNodes).PerNodeShift[0] = NULL;

    (*pNodes).PerNodeSlaveQty =
      ut_realloc_1d_int ((*pNodes).PerNodeSlaveQty, (*pNodes).NodeQty + 1);
    (*pNodes).PerNodeSlaveQty[(*pNodes).NodeQty] = 0;

    (*pNodes).PerNodeSlaveNb =
      ut_realloc_1d_pint ((*pNodes).PerNodeSlaveNb, (*pNodes).NodeQty + 1);
    (*pNodes).PerNodeSlaveNb[0] = NULL;
    (*pNodes).PerNodeSlaveNb[(*pNodes).NodeQty] = NULL;
  }

  return (*pNodes).NodeQty;
}

void
neut_nodes_memcpy (struct NODES Nodes, struct NODES *pNodes2)
{
  int i, id;

  neut_nodes_free (pNodes2);

  (*pNodes2).NodeQty = Nodes.NodeQty;
  (*pNodes2).NodeCoo = ut_alloc_2d (Nodes.NodeQty + 1, 3);

  ut_array_2d_memcpy (Nodes.NodeCoo + 1, Nodes.NodeQty, 3,
                      (*pNodes2).NodeCoo + 1);

  if (Nodes.NodeCl)
  {
    (*pNodes2).NodeCl = ut_alloc_1d (Nodes.NodeQty + 1);
    ut_array_1d_memcpy (Nodes.NodeCl + 1, Nodes.NodeQty,
                        (*pNodes2).NodeCl + 1);
  }

  if (Nodes.Periodic)
  {
    (*pNodes2).Periodic = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy (Nodes.Periodic, 3, (*pNodes2).Periodic);
  }

  if (Nodes.PeriodicDist)
  {
    (*pNodes2).PeriodicDist = ut_alloc_1d (3);
    ut_array_1d_memcpy (Nodes.PeriodicDist, 3, (*pNodes2).PeriodicDist);
  }

  (*pNodes2).PartQty = Nodes.PartQty;
  if (Nodes.PartQty)
  {
    (*pNodes2).NodePart = ut_alloc_1d_int ((*pNodes2).NodeQty + 1);
    ut_array_1d_int_memcpy (Nodes.NodePart + 1, Nodes.PartQty, (*pNodes2).NodePart + 1);

    (*pNodes2).Parts = ut_alloc_1d_pint ((*pNodes2).PartQty + 1);
    (*pNodes2).Parts[0] = NULL;

    for (i = 1; i <= (*pNodes2).PartQty; i++)
    {
      (*pNodes2).Parts[i] = ut_alloc_1d_int (Nodes.Parts[i][0] + 1);
      ut_array_1d_int_memcpy (Nodes.Parts[i], Nodes.Parts[i][0] + 1, (*pNodes2).Parts[i]);
    }
  }


  (*pNodes2).PerNodeQty = Nodes.PerNodeQty;

  if ((*pNodes2).PerNodeQty > 0)
  {
    if (Nodes.PerNodeNb)
    {
      (*pNodes2).PerNodeNb = ut_alloc_1d_int ((*pNodes2).PerNodeQty + 1);
      ut_array_1d_int_memcpy (Nodes.PerNodeNb + 1, (*pNodes2).PerNodeQty,
                              (*pNodes2).PerNodeNb + 1);
    }

    if (Nodes.PerNodeMaster)
    {
      (*pNodes2).PerNodeMaster = ut_alloc_1d_int ((*pNodes2).NodeQty + 1);
      ut_array_1d_int_memcpy (Nodes.PerNodeMaster + 1, (*pNodes2).NodeQty,
                              (*pNodes2).PerNodeMaster + 1);
    }

    if (Nodes.PerNodeShift)
    {
      (*pNodes2).PerNodeShift = ut_alloc_2d_int ((*pNodes2).NodeQty + 1, 3);
      ut_array_2d_int_memcpy (Nodes.PerNodeShift + 1, (*pNodes2).NodeQty, 3,
                              (*pNodes2).PerNodeShift + 1);
    }

    if (Nodes.PerNodeSlaveQty)
    {
      (*pNodes2).PerNodeSlaveQty = ut_alloc_1d_int ((*pNodes2).NodeQty + 1);
      ut_array_1d_int_memcpy (Nodes.PerNodeSlaveQty + 1, (*pNodes2).NodeQty,
                              (*pNodes2).PerNodeSlaveQty + 1);
    }

    if (Nodes.PerNodeSlaveNb)
    {
      (*pNodes2).PerNodeSlaveNb = ut_alloc_1d_pint ((*pNodes2).NodeQty + 1);
      for (i = 1; i <= (*pNodes2).PerNodeQty; i++)
      {
        id = (*pNodes2).PerNodeNb[i];
        (*pNodes2).PerNodeSlaveNb[id] =
          ut_alloc_1d_int ((*pNodes2).PerNodeSlaveQty[id] + 1);

        ut_array_1d_int_memcpy (Nodes.PerNodeSlaveNb[id] + 1,
                                (*pNodes2).PerNodeSlaveQty[id],
                                (*pNodes2).PerNodeSlaveNb[id] + 1);
      }
    }
  }

  (*pNodes2).DupNodeQty = Nodes.DupNodeQty;

  if ((*pNodes2).DupNodeQty > 0)
  {
    if (Nodes.DupNodeNb)
    {
      (*pNodes2).DupNodeNb = ut_alloc_1d_int ((*pNodes2).DupNodeQty + 1);
      ut_array_1d_int_memcpy (Nodes.DupNodeNb + 1, (*pNodes2).DupNodeQty,
                              (*pNodes2).DupNodeNb + 1);
    }

    if (Nodes.DupNodeMaster)
    {
      (*pNodes2).DupNodeMaster = ut_alloc_1d_int ((*pNodes2).NodeQty + 1);
      ut_array_1d_int_memcpy (Nodes.DupNodeMaster + 1, (*pNodes2).NodeQty,
                              (*pNodes2).DupNodeMaster + 1);
    }

    if (Nodes.DupNodeSeed)
    {
      (*pNodes2).DupNodeSeed = ut_alloc_1d_int ((*pNodes2).NodeQty + 1);
      ut_array_1d_int_memcpy (Nodes.DupNodeSeed + 1, (*pNodes2).NodeQty,
                              (*pNodes2).DupNodeSeed + 1);
    }

    if (Nodes.DupNodeSlaveQty)
    {
      (*pNodes2).DupNodeSlaveQty = ut_alloc_1d_int ((*pNodes2).NodeQty + 1);
      ut_array_1d_int_memcpy (Nodes.DupNodeSlaveQty + 1, (*pNodes2).NodeQty,
                              (*pNodes2).DupNodeSlaveQty + 1);
    }

    if (Nodes.DupNodeSlaveNb)
    {
      (*pNodes2).DupNodeSlaveNb = ut_alloc_1d_pint ((*pNodes2).NodeQty + 1);
      for (i = 1; i <= (*pNodes2).DupNodeQty; i++)
      {
        id = (*pNodes2).DupNodeNb[i];
        (*pNodes2).DupNodeSlaveNb[id] =
          ut_alloc_1d_int ((*pNodes2).DupNodeSlaveQty[id] + 1);

        ut_array_1d_int_memcpy (Nodes.DupNodeSlaveNb[id] + 1,
                                (*pNodes2).DupNodeSlaveQty[id],
                                (*pNodes2).DupNodeSlaveNb[id] + 1);
      }
    }
  }

  return;
}

void
neut_nodes_proj_alongontomesh (struct NODES *pN, double *n, struct NODES N,
                               struct MESH M, int elset)
{
  int i;

  for (i = 1; i <= (*pN).NodeQty; i++)
    neut_node_proj_alongontomesh ((*pN).NodeCoo[i], n, N, M, elset);

  return;
}

void
neut_node_proj_alongontomesh (double *coo, double *n, struct NODES N,
                              struct MESH M, int elset)
{
  int j, elt, status;
  double *coo2 = ut_alloc_1d (3);
  double *eq = ut_alloc_1d (4);

  // FILE* file;
  // struct PART PGarbage;
  // sprintf (message, "face-%d-bef.msh", i);
  // file = ut_file_open (message, "w");
  // WriteMeshGmsh (file, N, Garbage, M, Garbage, NULL, NULL, NULL, PGarbage);
  // ut_file_close (file, message, "w");

  status = 0;

  for (j = 1; j <= M.Elsets[elset][0]; j++)
  {
    elt = M.Elsets[elset][j];
    ut_array_1d_memcpy (coo, 3, coo2);
    neut_mesh_elt_eq (M, N, elt, eq);

    ut_space_point_dir_plane_proj (coo2, n, eq, coo2);

    if (ut_space_triangle_point_in
        (N.NodeCoo[M.EltNodes[elt][0]], N.NodeCoo[M.EltNodes[elt][1]],
         N.NodeCoo[M.EltNodes[elt][2]], coo2, 1e-4, 1e-4) == 1)
    {
      ut_array_1d_memcpy (coo2, 3, coo);
      status = 1;
      break;
    }
  }

  if (status == 0)
  {
    printf ("\nnode not caught during backward projection\n");
    ut_print_neperbug ();
  }

  ut_free_1d (&coo2);
  ut_free_1d (&eq);

  // sprintf (message, "face-%d-aft.msh", i);
  // file = ut_file_open (message, "w");
  // WriteMeshGmsh (file, N, Garbage, M, Garbage, NULL, NULL, NULL, PGarbage);
  // ut_file_close (file, message, "w");

  return;
}

int
neut_nodes_point_closestnode (struct NODES Nodes, double *coo, int *pnode)
{
  int i;
  double *dist = ut_alloc_1d (Nodes.NodeQty + 1);

  for (i = 1; i <= Nodes.NodeQty; i++)
    dist[i] = ut_space_dist (Nodes.NodeCoo[i], coo);

  (*pnode) = 1 + ut_array_1d_min_index (dist + 1, Nodes.NodeQty);

  ut_free_1d (&dist);

  return 0;
}

int
neut_nodes_rmorphans (struct NODES *pNodes, struct MESH *pMesh,
                      struct NSET *pNSet2D)
{
  int i, j, NodeQty;
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                                     (*pMesh).EltOrder);

  if ((*pMesh).EltQty == 0)
  {
    neut_nodes_reset (pNodes);
    return 1;
  }

  if ((*pMesh).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh, (*pNodes).NodeQty);

  int *old_new = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  int *new_old = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  NodeQty = 0;
  for (i = 1; i <= (*pNodes).NodeQty; i++)
    if ((*pMesh).NodeElts[i][0] != 0)
    {
      old_new[i] = ++NodeQty;
      new_old[old_new[i]] = i;
    }

  for (i = 1; i <= NodeQty; i++)
    ut_array_1d_memcpy ((*pNodes).NodeCoo[new_old[i]], 3,
                        (*pNodes).NodeCoo[i]);

  if ((*pNodes).NodeCl != NULL)
    for (i = 1; i <= NodeQty; i++)
      (*pNodes).NodeCl[i] = (*pNodes).NodeCl[new_old[i]];

  (*pNodes).NodeCoo =
    ut_realloc_2d_delline ((*pNodes).NodeCoo, (*pNodes).NodeQty + 1,
                           NodeQty + 1);
  (*pNodes).NodeCl = ut_realloc_1d ((*pNodes).NodeCl, NodeQty + 1);

  (*pNodes).NodeQty = NodeQty;

  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pMesh).EltNodes[i][j] = old_new[(*pMesh).EltNodes[i][j]];

  if (pNSet2D != NULL)
    for (i = 1; i <= (*pNSet2D).qty; i++)
    {
      for (j = 0; j < (*pNSet2D).NodeQty[i]; j++)
        (*pNSet2D).nodes[i][j] = old_new[(*pNSet2D).nodes[i][j]];

      (*pNSet2D).NodeQty[i] -=
        ut_array_1d_int_rmelt ((*pNSet2D).nodes[i], (*pNSet2D).NodeQty[i], 0,
                               (*pNSet2D).NodeQty[i]);
    }

  ut_free_1d_int (&old_new);
  ut_free_1d_int (&new_old);

  return 0;
}

void
neut_nodes_switch_pair (struct NODES *pNodes, int n1, int n2)
{
  ut_array_2d_switchlines ((*pNodes).NodeCoo, 3, n1, n2);
  ut_num_switch ((*pNodes).NodeCl + n1, (*pNodes).NodeCl + n2);

  return;
}

void
neut_nodes_switch (struct NODES *pNodes, int *node_nbs)
{
  int i;
  double **coo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    ut_array_1d_memcpy ((*pNodes).NodeCoo[i], 3, coo[i]);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    ut_array_1d_memcpy (coo[i], 3, (*pNodes).NodeCoo[node_nbs[i]]);

  ut_free_2d (&coo, (*pNodes).NodeQty);

  return;
}

void
neut_nodes_reverse (struct NODES *pNodes)
{
  int i;

  for (i = 1; i <= (*pNodes).NodeQty / 2; i++)
    neut_nodes_switch_pair (pNodes, i, (*pNodes).NodeQty - i + 1);

  return;
}

double
neut_nodes_dist_pair (struct NODES Nodes, int n1, int n2)
{
  return ut_space_dist (Nodes.NodeCoo[n1], Nodes.NodeCoo[n2]);
}

void
neut_nodes_bary (struct NODES Nodes, int *nodes, int NodeQty, double *coo,
                 double *pcl)
{
  int i;

  ut_array_1d_set (coo, 3, 0);
  for (i = 0; i < NodeQty; i++)
    ut_array_1d_add (coo, Nodes.NodeCoo[nodes[i]], 3, coo);
  ut_array_1d_scale (coo, 3, 1. / NodeQty);

  if (pcl && Nodes.NodeCl)
  {
    (*pcl) = 0;
    for (i = 0; i < NodeQty; i++)
      (*pcl) += Nodes.NodeCl[nodes[i]];
    (*pcl) /= NodeQty;
  }

  return;
}

void
neut_nodes_wbary (struct NODES Nodes, int *nodes, double *nodeweights,
                  int NodeQty, double *coo, double *pcl)
{
  int i, j;
  double sumweight;

  ut_array_1d_set (coo, 3, 0);
  for (i = 0; i < NodeQty; i++)
    for (j = 0; j < 3; j++)
      coo[j] += nodeweights[i] * Nodes.NodeCoo[nodes[i]][j];

  sumweight = ut_array_1d_sum (nodeweights, NodeQty);
  ut_array_1d_scale (coo, 3, 1. / sumweight);

  if (pcl && Nodes.NodeCl)
  {
    (*pcl) = 0;
    for (i = 0; i < NodeQty; i++)
      (*pcl) += nodeweights[i] * Nodes.NodeCl[nodes[i]];
    (*pcl) /= sumweight;
  }

  return;
}

/* node_nbs[...] = new pos */
/* RAM could be improved by more elegant exchange of lines (not through
 * a big copy coo array */
void
neut_nodes_renumber_switch (struct NODES *pNodes, int *node_nbs)
{
  int i;
  double **coo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    ut_array_1d_memcpy ((*pNodes).NodeCoo[i], 3, coo[i]);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    ut_array_1d_memcpy (coo[i], 3, (*pNodes).NodeCoo[node_nbs[i]]);

  ut_free_2d (&coo, (*pNodes).NodeQty);

  return;
}

int
neut_nodes_node_valid (struct NODES Nodes, int id)
{
  return (Nodes.NodeQty > 0 && id >= 1 && id <= Nodes.NodeQty);
}

int
neut_nodes_dim (struct NODES Nodes)
{
  int i, dim;
  double *bbox = ut_alloc_1d (6);
  double *l = ut_alloc_1d (3);

  neut_nodes_bbox_vect (Nodes, bbox);
  for (i = 0; i < 3; i++)
    l[i] = bbox[2 * i + 1] - bbox[2 * i];

  ut_array_1d_scale (l, 3, 1 / ut_array_1d_mean (l, 3));

  dim = 3;
  for (i = 2; i >= 1; i--)
    if (l[i] < 1e-6)
    {
      dim = i;
      break;
    }

  ut_free_1d (&bbox);
  ut_free_1d (&l);

  return dim;
}

void
neut_nodes_centre (struct NODES Nodes, double *centre)
{
  int i;
  double *bbox = ut_alloc_1d (6);

  neut_nodes_bbox_vect (Nodes, bbox);
  for (i = 0; i < 3; i++)
    centre[i] = .5 * (bbox[2 * i + 1] + bbox[2 * i]);

  ut_free_1d (&bbox);

  return;
}

void
neut_nodes_bboxcentre (struct NODES Nodes, double *centre)
{
  int i;
  double *bbox = ut_alloc_1d (6);

  neut_nodes_bbox_vect (Nodes, bbox);
  for (i = 0; i < 3; i++)
    centre[i] = .5 * (bbox[2 * i + 1] + bbox[2 * i]);

  ut_free_1d (&bbox);

  return;
}

void
neut_nodes_init_nodeslave (struct NODES *pNodes)
{
  int i, master, slave;

  (*pNodes).PerNodeSlaveQty = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  (*pNodes).PerNodeSlaveNb = ut_alloc_1d_pint ((*pNodes).NodeQty + 1);

  for (i = 1; i <= (*pNodes).PerNodeQty; i++)
  {
    slave = (*pNodes).PerNodeNb[i];
    master = (*pNodes).PerNodeMaster[slave];

    (*pNodes).PerNodeSlaveQty[master]++;
    (*pNodes).PerNodeSlaveNb[master] =
      ut_realloc_1d_int ((*pNodes).PerNodeSlaveNb[master],
                         (*pNodes).PerNodeSlaveQty[master] + 1);
    (*pNodes).PerNodeSlaveNb[master][(*pNodes).PerNodeSlaveQty[master]] =
      slave;
  }

  return;
}

void
neut_nodes_init_dupnodeslave (struct NODES *pNodes)
{
  int i, master, slave;

  if ((*pNodes).DupNodeSlaveQty)
    ut_free_1d_int (&(*pNodes).DupNodeSlaveQty);
  if ((*pNodes).DupNodeSlaveNb)
    ut_free_2d_int (&(*pNodes).DupNodeSlaveNb, (*pNodes).NodeQty + 1);

  (*pNodes).DupNodeSlaveQty = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  (*pNodes).DupNodeSlaveNb = ut_alloc_1d_pint ((*pNodes).NodeQty + 1);

  for (i = 1; i <= (*pNodes).DupNodeQty; i++)
  {
    slave = (*pNodes).DupNodeNb[i];
    master = (*pNodes).DupNodeMaster[slave];

    (*pNodes).DupNodeSlaveQty[master]++;
    (*pNodes).DupNodeSlaveNb[master] =
      ut_realloc_1d_int ((*pNodes).DupNodeSlaveNb[master],
                         (*pNodes).DupNodeSlaveQty[master] + 1);
    (*pNodes).DupNodeSlaveNb[master][(*pNodes).DupNodeSlaveQty[master]] =
      slave;
  }

  return;
}

void
neut_nodes_markasslave (struct NODES *pNodes, int slave, int master,
                        int *shift)
{
  (*pNodes).PerNodeQty++;
  (*pNodes).PerNodeNb =
    ut_realloc_1d_int ((*pNodes).PerNodeNb, (*pNodes).PerNodeQty + 1);

  (*pNodes).PerNodeNb[(*pNodes).PerNodeQty] = slave;
  (*pNodes).PerNodeMaster[slave] = master;
  ut_array_1d_int_memcpy (shift, 3, (*pNodes).PerNodeShift[slave]);

  return;
}

void
neut_nodes_markasdupslave (struct NODES *pNodes, int slave, int master)
{
  (*pNodes).DupNodeQty++;
  (*pNodes).DupNodeNb =
    ut_realloc_1d_int ((*pNodes).DupNodeNb, (*pNodes).DupNodeQty + 1);

  (*pNodes).DupNodeNb[(*pNodes).DupNodeQty] = slave;
  (*pNodes).DupNodeMaster[slave] = master;

  return;
}

void
neut_nodes_node_seed_dupnode (struct NODES Nodes, int node, int seed,
                              int *pdupnode)
{
  int i, slave;

  if (neut_nodes_node_isslave (Nodes, node))
    node = Nodes.DupNodeMaster[node];

  if (seed == Nodes.DupNodeSeed[node])
    (*pdupnode) = node;

  else if (Nodes.DupNodeSlaveQty[node])
  {
    (*pdupnode) = -1;
    for (i = 1; i <= Nodes.DupNodeSlaveQty[node]; i++)
    {
      slave = Nodes.DupNodeSlaveNb[node][i];
      if (Nodes.DupNodeSeed[slave] == seed)
      {
        (*pdupnode) = slave;
        break;
      }
    }
  }

  else
    (*pdupnode) = node;

  return;
}

int
neut_nodes_node_isslave (struct NODES Nodes, int node)
{
  return node > Nodes.NodeQty - Nodes.DupNodeQty;
}

int
neut_nodes_node_shift_pernode (struct NODES Nodes, int node, int *shift,
                               int *ppernode)
{
  int i, status, slavenode;
  int shift2[3];

  // input node must be a master node
  if (Nodes.PerNodeMaster[node] != 0)
  {
    for (i = 0; i < 3; i++)
      shift2[i] = shift[i] + Nodes.PerNodeShift[node][i];

    node = Nodes.PerNodeMaster[node];
  }
  else
    ut_array_1d_int_memcpy (shift, 3, shift2);

  (*ppernode) = -1;

  status = -1;

  if (shift2[0] == 0 && shift2[1] == 0 && shift2[2] == 0)
  {
    status = 0;
    (*ppernode) = node;
  }
  else
    for (i = 1; i <= Nodes.PerNodeSlaveQty[node]; i++)
    {
      slavenode = Nodes.PerNodeSlaveNb[node][i];
      if (ut_array_1d_int_equal (Nodes.PerNodeShift[slavenode], 3, shift2, 3))
      {
        (*ppernode) = slavenode;
        status = 0;
        break;
      }
    }

  return status;
}

void
neut_nodes_permasters (struct NODES Nodes, int **pmasters, int *pmasterqty)
{
  int i, node;

  (*pmasterqty) = Nodes.PerNodeQty;
  (*pmasters) = ut_alloc_1d_int (*pmasterqty);

  for (i = 1; i <= Nodes.PerNodeQty; i++)
  {
    node = Nodes.PerNodeNb[i];
    (*pmasters)[i - 1] = Nodes.PerNodeMaster[node];
  }

  ut_array_1d_int_sort_uniq (*pmasters, *pmasterqty, pmasterqty);

  (*pmasters) = ut_realloc_1d_int (*pmasters, *pmasterqty);

  return;
}

void
neut_nodes_init_parts (struct NODES *pNodes)
{
  int i, part;

  (*pNodes).PartQty = ut_array_1d_int_max ((*pNodes).NodePart + 1, (*pNodes).NodeQty);

  (*pNodes).Parts = ut_alloc_2d_int ((*pNodes).PartQty + 1, 1);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    part = (*pNodes).NodePart[i];

    (*pNodes).Parts[part][0]++;

    (*pNodes).Parts[part] = ut_realloc_1d_int ((*pNodes).Parts[part], (*pNodes).Parts[part][0] + 1);

    (*pNodes).Parts[part][(*pNodes).Parts[part][0]] = i;
  }

  return;
}

int
neut_nodes_isvoid (struct NODES Nodes)
{
  return Nodes.NodeQty == 0;
}
