/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_gen_.h"

int
neut_mesh_isvoid (struct MESH Mesh)
{
  return Mesh.EltQty == 0;
}

void
neut_mesh3d_slice (struct NODES Nodes, struct MESH Mesh, double *eq,
                   struct NODES *pSNodes, struct MESH *pSMesh,
                   int **pelt_newold, int ***pnode_newold,
                   double **pnode_fact)
{
  int i, id, elset;

  if (Mesh.Dimension != 3)
    ut_print_neperbug ();

  (*pSMesh).Dimension = 2;
  (*pSMesh).EltOrder = 1;
  (*pSMesh).EltType = ut_alloc_1d_char (4);
  strcpy ((*pSMesh).EltType, "tri");

  (*pelt_newold) = ut_alloc_1d_int (1);
  (*pnode_newold) = ut_alloc_2d_int (1, 2);
  (*pnode_fact) = ut_alloc_1d (1);

  if (!strcmp (Mesh.EltType, "tri"))
    for (i = 1; i <= Mesh.EltQty; i++)
      neut_mesh3d_elt_slice_tri (Nodes, Mesh, i, eq, pSNodes, pSMesh,
                                 pelt_newold, pnode_newold, pnode_fact);
  else if (!strcmp (Mesh.EltType, "quad"))
    for (i = 1; i <= Mesh.EltQty; i++)
      neut_mesh3d_elt_slice_quad (Nodes, Mesh, i, eq, pSNodes, pSMesh,
                                  pelt_newold, pnode_newold, pnode_fact);
  else
    ut_print_neperbug ();

  int ElsetQty, **Elsets = NULL;

  ElsetQty = Mesh.ElsetQty;
  Elsets = ut_alloc_2d_int (ElsetQty + 1, 1);

  for (i = 1; i <= (*pSMesh).EltQty; i++)
  {
    elset = Mesh.EltElset[(*pelt_newold)[i]];
    Elsets[elset][0]++;
    Elsets[elset] = ut_realloc_1d_int (Elsets[elset], Elsets[elset][0] + 1);
    Elsets[elset][Elsets[elset][0]] = i;
  }

  id = 0;
  (*pSMesh).ElsetId = ut_alloc_1d_int (Mesh.ElsetQty + 1);
  for (i = 1; i <= ElsetQty; i++)
    if (Elsets[i][0] > 0)
    {
      neut_mesh_addelset (pSMesh, Elsets[i] + 1, Elsets[i][0]);
      (*pSMesh).ElsetId[++id] = i;
    }

  neut_mesh_init_eltelset (pSMesh, NULL);

  ut_free_2d_int (&Elsets, ElsetQty + 1);

  return;
}

int
neut_mesh3d_elt_slice_tri (struct NODES Nodes, struct MESH Mesh, int elt,
                           double *eq, struct NODES *pSNodes,
                           struct MESH *pSMesh, int **pelt_newold,
                           int ***pnode_newold, double **pnode_fact)
{
  int i, j, node;
  int *side = ut_alloc_1d_int (4);
  int siden, intertype, nodemaster, id;
  int *pos = ut_alloc_1d_int (2);
  int *pos2 = ut_alloc_1d_int (2);
  double *coo = ut_alloc_1d (3);
  int *eltnodes = ut_alloc_1d_int (3);
  double dist, distmaster;

  // intertype = 0 -> no intersection
  //
  // intertype = 1 -> 1|3 vertices (two sides of the plane): the
  // intersection between the elt and the plane is a triangle.
  //
  // intertype = 2 -> 2|2 vertices (two sides of the plane): the
  // intersection between the elt and the plane is a quadrangle;
  // spliting it into two triangles.

  for (i = 0; i < 4; i++)
  {
    node = Mesh.EltNodes[elt][i];
    side[i] = ut_space_point_plane_side (Nodes.NodeCoo[node] - 1, eq);
  }
  siden = ut_array_1d_int_valnb (side, 4, -1);
  intertype = ut_num_min (siden, 4 - siden);

  if (intertype == 1)
  {
    // finding master node
    if (siden == 1)
      pos[0] = ut_array_1d_int_eltpos (side, 4, -1);
    else if (siden == 3)
      pos[0] = ut_array_1d_int_eltpos (side, 4, 1);
    else
      ut_print_neperbug ();

    nodemaster = Mesh.EltNodes[elt][pos[0]];
    distmaster = ut_space_point_plane_dist (Nodes.NodeCoo[nodemaster], eq);

    // finding intersection between the tet and the plane (triangle)
    id = 0;
    for (i = 0; i < 4; i++)
      if (i != pos[0])
      {
        node = Mesh.EltNodes[elt][i];
        ut_space_segment_plane_intersect (Nodes.NodeCoo[nodemaster],
                                          Nodes.NodeCoo[node], eq, coo);
        neut_nodes_addnode (pSNodes, coo, 0);

        dist = ut_space_point_plane_dist (Nodes.NodeCoo[node], eq);

        eltnodes[id++] = (*pSNodes).NodeQty;

        (*pnode_newold) =
          ut_realloc_2d_int_addline ((*pnode_newold), (*pSNodes).NodeQty + 1,
                                     2);
        (*pnode_fact) = ut_realloc_1d ((*pnode_fact), (*pSNodes).NodeQty + 1);

        (*pnode_newold)[(*pSNodes).NodeQty][0] = nodemaster;
        (*pnode_newold)[(*pSNodes).NodeQty][1] = node;
        (*pnode_fact)[(*pSNodes).NodeQty] = distmaster / (distmaster + dist);
      }

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;
  }

  else if (intertype == 2)
  {
    int id = 0;
    int id2 = 0;
    for (i = 0; i < 4; i++)
      if (side[i] == -1)
        pos[id++] = i;
      else
        pos2[id2++] = i;

    // Finding intersection between the tet and the plane
    // (quadrangle).  Splitting up the quadrangle into two triangles
    // for plotting.

    for (j = 0; j < 2; j++)
    {
      nodemaster = Mesh.EltNodes[elt][pos[j]];
      distmaster = ut_space_point_plane_dist (Nodes.NodeCoo[nodemaster], eq);

      for (i = 0; i < 2; i++)
      {
        node = Mesh.EltNodes[elt][pos2[i]];
        dist = ut_space_point_plane_dist (Nodes.NodeCoo[node], eq);

        ut_space_segment_plane_intersect (Nodes.NodeCoo[nodemaster],
                                          Nodes.NodeCoo[node], eq, coo);
        neut_nodes_addnode (pSNodes, coo, 0);

        (*pnode_newold) =
          ut_realloc_2d_int_addline ((*pnode_newold), (*pSNodes).NodeQty + 1,
                                     2);
        (*pnode_fact) = ut_realloc_1d ((*pnode_fact), (*pSNodes).NodeQty + 1);

        (*pnode_newold)[(*pSNodes).NodeQty][0] = nodemaster;
        (*pnode_newold)[(*pSNodes).NodeQty][1] = node;
        (*pnode_fact)[(*pSNodes).NodeQty] = distmaster / (distmaster + dist);
      }
    }

    eltnodes[0] = (*pSNodes).NodeQty - 3;
    eltnodes[1] = (*pSNodes).NodeQty - 2;
    eltnodes[2] = (*pSNodes).NodeQty - 1;

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;

    eltnodes[0] = (*pSNodes).NodeQty - 1;
    eltnodes[1] = (*pSNodes).NodeQty - 2;
    eltnodes[2] = (*pSNodes).NodeQty;

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;
  }

  ut_free_1d_int (&side);
  ut_free_1d (&coo);
  ut_free_1d_int (&pos);
  ut_free_1d_int (&pos2);
  ut_free_1d_int (&eltnodes);

  return intertype;
}

int
neut_mesh3d_elt_slice_quad (struct NODES Nodes, struct MESH Mesh, int elt,
                            double *eq, struct NODES *pSNodes,
                            struct MESH *pSMesh, int **pelt_newold,
                            int ***pnode_newold, double **pnode_fact)
{
  int i, j, sl_elt, sl_node, EltQty, NodeQty;
  struct NODES ENodes;
  struct MESH EMesh;
  struct MESH TEMesh;

  neut_nodes_set_zero (&ENodes);
  neut_mesh_set_zero (&EMesh);
  neut_mesh_set_zero (&TEMesh);

  neut_mesh_elt_mesh (Nodes, Mesh, elt, &ENodes, &EMesh);

  neut_mesh_quad_tri (ENodes, EMesh, &TEMesh);

  for (i = 1; i <= 6; i++)
  {
    NodeQty = (*pSNodes).NodeQty;
    EltQty = (*pSMesh).EltQty;
    neut_mesh3d_elt_slice_tri (ENodes, TEMesh, i, eq, pSNodes, pSMesh,
                               pelt_newold, pnode_newold, pnode_fact);

    // Renumbering added elts
    for (sl_elt = EltQty + 1; sl_elt <= (*pSMesh).EltQty; sl_elt++)
      (*pelt_newold)[sl_elt] = elt;

    // Renumbering added nodes
    for (sl_node = NodeQty + 1; sl_node <= (*pSNodes).NodeQty; sl_node++)
      for (j = 0; j < 2; j++)
        (*pnode_newold)[sl_node][j] =
          Mesh.EltNodes[elt][(*pnode_newold)[sl_node][j] - 1];
  }

  neut_nodes_free (&ENodes);
  neut_mesh_free (&EMesh);
  neut_mesh_free (&TEMesh);

  return 0;
}

int
neut_mesh_elt_mesh (struct NODES Nodes, struct MESH Mesh, int elt,
                    struct NODES *pENodes, struct MESH *pEMesh)
{
  int i, node;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  // Setting *pENodes
  neut_nodes_set_zero (pENodes);

  (*pENodes).NodeQty = eltnodeqty;

  (*pENodes).NodeCoo = ut_alloc_2d ((*pENodes).NodeQty + 1, 3);

  for (i = 1; i <= eltnodeqty; i++)
  {
    node = Mesh.EltNodes[elt][i - 1];
    ut_array_1d_memcpy (Nodes.NodeCoo[node], 3, (*pENodes).NodeCoo[i]);
  }

  // Setting *pEMesh
  neut_mesh_set_zero (pEMesh);

  (*pEMesh).EltType = ut_alloc_1d_char (strlen (Mesh.EltType) + 1);
  strcpy ((*pEMesh).EltType, Mesh.EltType);

  (*pEMesh).Dimension = Mesh.Dimension;
  (*pEMesh).EltOrder = Mesh.EltOrder;

  (*pEMesh).EltQty = 1;
  (*pEMesh).EltNodes = ut_alloc_2d_int (2, eltnodeqty);

  (*pEMesh).EltElset = ut_alloc_1d_int (2);
  (*pEMesh).EltElset[1] = Mesh.EltElset[elt];

  for (i = 0; i < eltnodeqty; i++)
    (*pEMesh).EltNodes[1][i] = i + 1;

  return 0;
}

void
neut_mesh_elset_mesh (struct NODES Nodes, struct MESH Mesh, int elset,
                      struct NODES *pN, struct MESH *pM, int **pnode_nbs)
{
  int i, j, node, elt, eltnodeqty;
  double l;
  int *node_nbs = NULL;
  int *node_nbs_inv = NULL;

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  (*pM).Dimension = Mesh.Dimension;
  (*pM).EltOrder = 1;
  ut_string_string (Mesh.EltType, &(*pM).EltType);
  eltnodeqty =
    neut_elt_nodeqty ((*pM).EltType, (*pM).Dimension, (*pM).EltOrder);

  node_nbs = ut_alloc_1d_int (eltnodeqty * Mesh.Elsets[elset][0] + 1);

  l = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];

    neut_mesh_addelt (pM, Mesh.EltNodes[elt]);

    for (j = 0; j < eltnodeqty; j++)
    {
      node = Mesh.EltNodes[elt][j];
      if (ut_array_1d_int_eltpos (node_nbs + 1, node_nbs[0], node) == -1)
      {
        node_nbs[++node_nbs[0]] = node;

        if (!Nodes.NodeCl)
          neut_mesh_elt_length (Nodes, Mesh, elt, &l);
        else
          l = Nodes.NodeCl[node_nbs[node_nbs[0]]];
        neut_nodes_addnode (pN, Nodes.NodeCoo[node], l);
      }
    }
  }

  (*pM).ElsetQty = 1;
  (*pM).Elsets = ut_alloc_2d_int (2, (*pM).EltQty + 1);
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
    (*pM).Elsets[1][++(*pM).Elsets[1][0]] = i;

  neut_mesh_init_eltelset (pM, NULL);

  int qty;
  ut_array_1d_int_inv (node_nbs + 1, node_nbs[0], &node_nbs_inv, &qty);
  for (i = 1; i <= (*pM).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pM).EltNodes[i][j] = node_nbs_inv[(*pM).EltNodes[i][j]] + 1;

  if (pnode_nbs)
  {
    (*pnode_nbs) = ut_alloc_1d_int (node_nbs[0] + 1);
    ut_array_1d_int_memcpy (node_nbs, node_nbs[0] + 1, *pnode_nbs);
  }

  ut_free_1d_int (&node_nbs);
  ut_free_1d_int (&node_nbs_inv);

  return;
}

void
neut_mesh_face_boundmesh (struct MESH Mesh1D, struct TESS Tess, int face,
                          struct MESH *pBMesh)
{
  int e, i, j, edge, elt;
  int *nodes = ut_alloc_1d_int (3);
  int **seq = ut_alloc_2d_int (2, 2);
  int ori;
  int *elts = NULL;

  seq[0][0] = 0;
  seq[0][1] = 1;
  seq[1][0] = 1;
  seq[1][1] = 0;

  (*pBMesh).Dimension = Mesh1D.Dimension;
  (*pBMesh).EltOrder = Mesh1D.EltOrder;
  (*pBMesh).EltType = ut_alloc_1d_char (strlen (Mesh1D.EltType) + 1);
  strcpy ((*pBMesh).EltType, Mesh1D.EltType);

  for (e = 1; e <= Tess.FaceVerQty[face]; e++)
  {
    edge = Tess.FaceEdgeNb[face][e];
    ori = (Tess.FaceEdgeOri[face][e] == 1) ? 0 : 1;

    for (i = 1; i <= Mesh1D.Elsets[edge][0]; i++)
    {
      elt = Mesh1D.Elsets[edge][i];

      for (j = 0; j < 2; j++)
        nodes[j] = Mesh1D.EltNodes[elt][seq[ori][j]];

      neut_mesh_addelt (pBMesh, nodes);
    }
  }

  elts = ut_alloc_1d_int ((*pBMesh).EltQty);
  for (i = 0; i < (*pBMesh).EltQty; i++)
    elts[i] = i + 1;

  neut_mesh_addelset (pBMesh, elts, (*pBMesh).EltQty);

  neut_mesh_init_eltelset (pBMesh, NULL);

  /*----------------------------------------------------------------------
   * foot */

  ut_free_1d_int (&nodes);
  ut_free_1d_int (&elts);
  ut_free_2d_int (&seq, 2);

  return;
}

void
neut_mesh_face_boundnodes (struct MESH Mesh1D, struct TESS Tess, int face,
                           int **pnodes, int *pnodeqty)
{
  int i, j, edge, elt;
  // note: only the 1st-order nodes are recorded

  (*pnodeqty) = 0;
  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];
    (*pnodeqty) += Mesh1D.Elsets[edge][0];
  }

  (*pnodes) = ut_alloc_1d_int (*pnodeqty);

  (*pnodeqty) = 0;
  // for every edge
  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];

    if (Tess.FaceEdgeOri[face][i] == 1)
      for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
      {
        elt = Mesh1D.Elsets[edge][j];
        (*pnodes)[(*pnodeqty)++] = Mesh1D.EltNodes[elt][0];
      }
    else
      for (j = Mesh1D.Elsets[edge][0]; j >= 1; j--)
      {
        elt = Mesh1D.Elsets[edge][j];
        (*pnodes)[(*pnodeqty)++] = Mesh1D.EltNodes[elt][1];
      }
  }

  return;
}

void
neut_mesh_face_boundnodecoos (struct NODES Nodes, struct MESH Mesh1D,
                              struct TESS Tess, int face, double ***pnodecoos,
                              int *pnodeqty)
{
  int i, node, *nodes = NULL;

  neut_mesh_face_boundnodes (Mesh1D, Tess, face, &nodes, pnodeqty);

  (*pnodecoos) = ut_alloc_2d (*pnodeqty, 3);

  for (i = 0; i < *pnodeqty; i++)
  {
    node = nodes[i];
    ut_array_1d_memcpy (Nodes.NodeCoo[node], 3, (*pnodecoos)[i]);
  }

  ut_free_1d_int (&nodes);

  return;
}

void
neut_mesh_poly_boundmesh (struct TESS Tess, int poly, struct MESH Mesh2D,
                          struct MESH *pBMesh)
{
  int f, i, j, face, elt;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);
  int *nodes = ut_alloc_1d_int (eltnodeqty);
  int **seq = ut_alloc_2d_int (2, eltnodeqty);
  int ori;
  int *elts = NULL;

  if (strcmp (Mesh2D.EltType, "tri") != 0)
    ut_print_neperbug ();

  if (Mesh2D.EltOrder == 1)
  {
    ut_array_1d_int_set_id (seq[0], eltnodeqty);
    ut_array_1d_int_set_id (seq[1], eltnodeqty);
    ut_array_1d_int_reverseelts (seq[1], eltnodeqty);
  }
  else if (Mesh2D.EltOrder == 2)
  {
    ut_array_1d_int_set_id (seq[0], eltnodeqty);
    ut_array_1d_int_set_id (seq[1], eltnodeqty);
    ut_array_1d_int_reverseelts (seq[1], 3);
    ut_array_1d_int_reverseelts (seq[1] + 3, 3);
  }
  else
    ut_print_neperbug ();

  (*pBMesh).Dimension = Mesh2D.Dimension;
  (*pBMesh).EltOrder = Mesh2D.EltOrder;
  (*pBMesh).EltType = ut_alloc_1d_char (strlen (Mesh2D.EltType) + 1);
  strcpy ((*pBMesh).EltType, Mesh2D.EltType);

  for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
  {
    face = Tess.PolyFaceNb[poly][f];
    ori = (Tess.PolyFaceOri[poly][f] == 1) ? 0 : 1;

    for (i = 1; i <= Mesh2D.Elsets[face][0]; i++)
    {
      elt = Mesh2D.Elsets[face][i];

      for (j = 0; j < eltnodeqty; j++)
        nodes[j] = Mesh2D.EltNodes[elt][seq[ori][j]];

      neut_mesh_addelt (pBMesh, nodes);
    }
  }

  elts = ut_alloc_1d_int ((*pBMesh).EltQty);
  for (i = 0; i < (*pBMesh).EltQty; i++)
    elts[i] = i + 1;

  neut_mesh_addelset (pBMesh, elts, (*pBMesh).EltQty);

  neut_mesh_init_eltelset (pBMesh, NULL);

  /*----------------------------------------------------------------------
   * foot */

  ut_free_1d_int (&nodes);
  ut_free_1d_int (&elts);
  ut_free_2d_int (&seq, 2);

  return;
}

int
neut_mesh_elset_valid (struct MESH Mesh, int elset)
{
  return (elset > 0 && elset <= Mesh.ElsetQty && Mesh.Elsets[elset][0] > 0);
}

void
neut_mesh_elts_mesh2ddist (struct TESS Tess, struct NODES Nodes,
                           struct MESH Mesh2D, struct MESH Mesh3D, double **p,
                           double *d, double **v, double **n)
{
  int i, elset;
  struct MESH *M = calloc (Tess.PolyQty + 1, sizeof (MESH));
  double *coo = ut_alloc_1d (3);
  double *eltcoo = ut_alloc_1d (3);

  for (i = 1; i <= Tess.PolyQty; i++)
    neut_mesh_set_zero (&(M[i]));

  if (Mesh2D.Dimension != 2 || Mesh3D.Dimension != 3)
    ut_print_neperbug ();

  // finding the poly to which the point belongs
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    neut_mesh_elt_centre (Nodes, Mesh3D, i, coo);
    elset = Mesh3D.EltElset[i];

    if (M[elset].EltQty == 0)
    {
      neut_mesh_poly_boundmesh (Tess, elset, Mesh2D, &(M[elset]));
      neut_mesh_init_nodeelts (&(M[elset]), Nodes.NodeQty);
    }

    neut_mesh_point_closestpoint (M[elset], Nodes, coo, &(d[i]), p[i], v[i],
                                  n[i]);
  }

  for (i = 1; i <= Tess.PolyQty; i++)
    neut_mesh_free (&(M[i]));
  free (M);

  ut_free_1d (&coo);
  ut_free_1d (&eltcoo);

  return;
}

void
neut_mesh_nodes_mesh2ddist (struct TESS Tess, struct NODES Nodes,
                            struct MESH Mesh2D, struct MESH Mesh3D,
                            double **p, double *d, double **v, double **n)
{
  int i, elset, elsetqty;
  int *elsets = NULL;
  struct MESH *M = calloc (Tess.PolyQty + 1, sizeof (MESH));

  for (i = 1; i <= Tess.PolyQty; i++)
    neut_mesh_set_zero (&(M[i]));

  if (Mesh2D.Dimension != 2 || Mesh3D.Dimension != 3)
    ut_print_neperbug ();

  // finding the poly to which the point belongs
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    neut_mesh_node_elsets (Mesh3D, i, &elsets, &elsetqty);

    if (elsetqty == 0)
      ut_print_neperbug ();

    elset = elsets[0];

    if (M[elset].EltQty == 0)
    {
      neut_mesh_poly_boundmesh (Tess, elset, Mesh2D, &(M[elset]));
      neut_mesh_init_nodeelts (&(M[elset]), Nodes.NodeQty);
    }

    // in the body of a grain
    if (elsetqty == 1)
      neut_mesh_point_closestpoint (M[elset], Nodes, Nodes.NodeCoo[i],
                                    &(d[i]), p[i], v[i], n[i]);
    // on the 2D mesh
    else
    {
      ut_array_1d_memcpy (Nodes.NodeCoo[i], 3, p[i]);
      d[i] = 0;
      ut_array_1d_zero (v[i], 3);
      neut_mesh_node_normal (M[elset], Nodes, i, n[i]);
    }

    ut_free_1d_int (&elsets);
  }

  for (i = 1; i <= Tess.PolyQty; i++)
    neut_mesh_free (&(M[i]));
  free (M);

  return;
}

void
neut_mesh_points_mesh2ddist (struct TESS Tess, struct NODES Nodes,
                             struct MESH Mesh2D, struct MESH Mesh3D,
                             double **coo, int qty, double **p, double *d,
                             double **v, double **n)
{
  int i, j, status, elset, pos;
  struct MESH *M = calloc (Tess.PolyQty + 1, sizeof (MESH));
  int *elsetlist = ut_alloc_1d_int (Mesh3D.ElsetQty);
  int elsetqty = Mesh3D.ElsetQty;

  for (i = 0; i < elsetqty; i++)
    elsetlist[i] = i + 1;

  for (i = 1; i <= Tess.PolyQty; i++)
    neut_mesh_set_zero (&(M[i]));

  if (Mesh2D.Dimension != 2 || Mesh3D.Dimension != 3)
    ut_print_neperbug ();

  // finding the poly to which the point belongs
  for (i = 0; i < qty; i++)
  {
    status =
      neut_mesh_point_elset (Mesh3D, Nodes, coo[i], elsetlist, elsetqty,
                             &elset);
    if (elset != elsetlist[0])
    {
      pos = ut_array_1d_int_eltpos (elsetlist, elsetqty, elset);
      for (j = pos; j > 0; j--)
        elsetlist[j] = elsetlist[j - 1];
      elsetlist[0] = elset;
    }

    if (status == -1)
      ut_print_neperbug ();

    if (M[elset].EltQty == 0)
    {
      neut_mesh_poly_boundmesh (Tess, elset, Mesh2D, &(M[elset]));
      neut_mesh_init_nodeelts (&(M[elset]), Nodes.NodeQty);
    }

    if (!neut_mesh_elset_valid (Mesh3D, elset))
      ut_print_neperbug ();

    neut_mesh_point_closestpoint (M[elset], Nodes, coo[i], &(d[i]), p[i],
                                  v[i], n[i]);
  }

  for (i = 1; i <= Tess.PolyQty; i++)
    neut_mesh_free (&(M[i]));
  free (M);

  ut_free_1d_int (&elsetlist);

  return;
}

void
neut_mesh_entity_qty (struct NODES Nodes, struct MESH Mesh0D,
                      struct MESH Mesh1D, struct MESH Mesh2D,
                      struct MESH Mesh3D, char *entity, int *pentityqty)
{
  (*pentityqty) = 0;

  if (!strcmp (entity, "elset3d"))
    (*pentityqty) = Mesh3D.ElsetQty;
  else if (!strcmp (entity, "elset2d"))
    (*pentityqty) = Mesh2D.ElsetQty;
  else if (!strcmp (entity, "elset1d"))
    (*pentityqty) = Mesh1D.ElsetQty;
  else if (!strcmp (entity, "elset0d"))
    (*pentityqty) = Mesh0D.ElsetQty;
  else if (!strcmp (entity, "elset"))
  {
    if (Mesh3D.ElsetQty > 0)
      (*pentityqty) = Mesh3D.ElsetQty;
    else if (Mesh2D.ElsetQty > 0)
      (*pentityqty) = Mesh2D.ElsetQty;
    else if (Mesh1D.ElsetQty > 0)
      (*pentityqty) = Mesh1D.ElsetQty;
    else if (Mesh0D.ElsetQty > 0)
      (*pentityqty) = Mesh0D.ElsetQty;
  }
  else if (!strcmp (entity, "elt3d"))
    (*pentityqty) = Mesh3D.EltQty;
  else if (!strcmp (entity, "elt2d"))
    (*pentityqty) = Mesh2D.EltQty;
  else if (!strcmp (entity, "elt1d"))
    (*pentityqty) = Mesh1D.EltQty;
  else if (!strcmp (entity, "elt0d"))
    (*pentityqty) = Mesh0D.EltQty;
  else if (!strcmp (entity, "elt"))
  {
    if (Mesh3D.EltQty > 0)
      (*pentityqty) = Mesh3D.EltQty;
    else if (Mesh2D.EltQty > 0)
      (*pentityqty) = Mesh2D.EltQty;
    else if (Mesh1D.EltQty > 0)
      (*pentityqty) = Mesh1D.EltQty;
    else if (Mesh0D.EltQty > 0)
      (*pentityqty) = Mesh0D.EltQty;
  }
  else if (!strcmp (entity, "node"))
    (*pentityqty) = Nodes.NodeQty;
  else if (!strncmp (entity, "mesh", 4))
    (*pentityqty) = 1;
  else
    ut_print_neperbug ();

  return;
}

int
neut_mesh_var_val (struct NODES Nodes, struct MESH Mesh0D, struct MESH Mesh1D,
                   struct MESH Mesh2D, struct MESH Mesh3D, struct MESH MeshCo,
                   struct TESS Tess, int *showelt0d, int *showelt1d,
                   int *showelt2d, int *showelt3d, double cl, char *entity_in,
                   int id, char *var, double **pvals, int *pvalqty,
                   char **ptype)
{
  int j, status;
  double rrmean, rrmin, rrmax;
  char *typetmp = ut_alloc_1d_char (10);
  char *entity = NULL;

  ut_string_string ("%f", &typetmp);

  if (!strcmp (entity_in, "nodes"))
    ut_string_string ("node", &entity);
  else if (!strcmp (entity_in, "elt") || !strcmp (entity_in, "elts"))
  {
    if (!neut_mesh_isvoid (Mesh3D))
      ut_string_string ("elt3d", &entity);
    else if (!neut_mesh_isvoid (Mesh2D))
      ut_string_string ("elt2d", &entity);
    else if (!neut_mesh_isvoid (Mesh1D))
      ut_string_string ("elt1d", &entity);
    else if (!neut_mesh_isvoid (Mesh0D))
      ut_string_string ("elt0d", &entity);
    else
      abort ();
  }
  else if (!strcmp (entity_in, "elset") || !strcmp (entity_in, "elsets"))
  {
    if (!neut_mesh_isvoid (Mesh3D))
      ut_string_string ("elset3d", &entity);
    else if (!neut_mesh_isvoid (Mesh2D))
      ut_string_string ("elset2d", &entity);
    else if (!neut_mesh_isvoid (Mesh1D))
      ut_string_string ("elset1d", &entity);
    else if (!neut_mesh_isvoid (Mesh0D))
      ut_string_string ("elset0d", &entity);
    else
      abort ();
  }
  else if (!strcmp (entity_in, "mesh"))
  {
    if (!neut_mesh_isvoid (Mesh3D))
      ut_string_string ("mesh3d", &entity);
    else if (!neut_mesh_isvoid (Mesh2D))
      ut_string_string ("mesh2d", &entity);
    else if (!neut_mesh_isvoid (Mesh1D))
      ut_string_string ("mesh1d", &entity);
    else if (!neut_mesh_isvoid (Mesh0D))
      ut_string_string ("mesh0d", &entity);
    else
      abort ();
  }
  else
    ut_string_string (entity_in, &entity);

  (*pvals) = ut_realloc_1d (*pvals, 1);

  if (pvalqty)
    (*pvalqty) = 1;

  status = -1;
  if (!strcmp (entity, "elset3d"))
  {
    neut_mesh_elset_rr (Nodes, Mesh3D, id, &rrmean, &rrmin, &rrmax);

    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = Mesh3D.ElsetId ? Mesh3D.ElsetId[id] : id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = Mesh3D.Elsets[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elts"))
    {
      (*pvalqty) = Mesh3D.Elsets[id][0];
      (*pvals) = ut_alloc_1d (Mesh3D.Elsets[id][0]);
      ut_array_1d_memcpy_fromint (Mesh3D.Elsets[id] + 1, Mesh3D.Elsets[id][0], *pvals);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      int qty;
      neut_mesh_elset_nodes (Mesh3D, id, NULL, &qty);
      (*pvals)[0] = qty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodes"))
    {
      int qty, *tmp = NULL;
      neut_mesh_elset_nodes (Mesh3D, id, &tmp, &qty);
      (*pvalqty) = qty;
      (*pvals) = ut_alloc_1d (qty);
      ut_array_1d_memcpy_fromint (tmp, qty, *pvals);
      strcpy (typetmp, "%d");
      ut_free_1d_int (&tmp);
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elset_centre_x (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elset_centre_y (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elset_centre_z (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_elset_centre (Nodes, Mesh3D, id, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "vol"))
      neut_mesh_elset_volume (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "diameq"))
      neut_mesh_elset_diameq (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "radeq"))
      neut_mesh_elset_radeq (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "rrav"))
      (*pvals)[0] = rrmean;
    else if (!strcmp (var, "rrmin"))
      (*pvals)[0] = rrmin;
    else if (!strcmp (var, "rrmax"))
      (*pvals)[0] = rrmax;
    else if (!strcmp (var, "true"))
    {
      (*pvals)[0] = Tess.CellTrue[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "lam"))
    {
      (*pvals)[0] = Tess.CellLamId[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = Tess.CellBody[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "Osize"))
    {
      if (cl > 0)
        neut_mesh_elset_Osize (Nodes, Mesh3D, id, cl, *pvals);
      else
        (*pvals)[0] = -1;
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Mesh3D.ElsetGroup? Mesh3D.ElsetGroup[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strncmp (var, "fiber", 5))
    {
      double theta;
      double *dirc = ut_alloc_1d (3);
      double *dirs = ut_alloc_1d (3);
      double *ori = Mesh3D.ElsetOri[id];

      neut_ori_fiber_sscanf (var, dirc, dirs, &theta, NULL);
      (*pvals)[0] = neut_ori_fiber_in (ori, Tess.CellCrySym, dirc, dirs, theta);
      strcpy (typetmp, "%d");

      ut_free_1d (&dirc);
      ut_free_1d (&dirs);
    }
    else if (Tess.Dim == 3 && neut_ori_des_isvalid (var)) // orientation descriptor
    {
      (*pvalqty) = ol_des_size (var);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      neut_ori_des_ori (Mesh3D.ElsetOri[id], var, *pvals);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "elset2d"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = Mesh2D.ElsetId ? Mesh2D.ElsetId[id] : id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elset_centre_x (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elset_centre_y (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elset_centre_z (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "area"))
      neut_mesh_elset_area (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "diameq"))
      neut_mesh_elset_diameq (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "radeq"))
      neut_mesh_elset_radeq (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = Mesh2D.Elsets[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elts"))
    {
      (*pvalqty) = Mesh2D.Elsets[id][0];
      (*pvals) = ut_alloc_1d (Mesh2D.Elsets[id][0]);
      ut_array_1d_memcpy_fromint (Mesh2D.Elsets[id] + 1, Mesh2D.Elsets[id][0], *pvals);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      int qty;
      neut_mesh_elset_nodes (Mesh2D, id, NULL, &qty);
      (*pvals)[0] = qty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodes"))
    {
      int qty, *tmp = NULL;
      neut_mesh_elset_nodes (Mesh2D, id, &tmp, &qty);
      (*pvalqty) = qty;
      (*pvals) = ut_alloc_1d (qty);
      ut_array_1d_memcpy_fromint (tmp, qty, *pvals);
      strcpy (typetmp, "%d");
      ut_free_1d_int (&tmp);
    }
    else if (!strcmp (var, "true"))
    {
      (*pvals)[0] = neut_tess_face_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = neut_tess_face_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "domtype"))
    {
      (*pvals)[0] = Tess.FaceDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Mesh2D.ElsetGroup? Mesh2D.ElsetGroup[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "per"))
    {
      (*pvals)[0] = neut_tess_face_isper (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "theta"))
    {
      int i, elt, elt3dqty, *elt3d = NULL;
      double tmp;

      (*pvals)[0] = 0;
      for (i = 1; i <= Mesh2D.Elsets[id][0]; i++)
      {
        elt = Mesh2D.Elsets[id][i];

        neut_mesh_elt2d_elts3d (Mesh2D, elt, Mesh3D, &elt3d, &elt3dqty);

        if (elt3dqty < 2)
        {
          (*pvals)[0] = -1;
          break;
        }
        else
        {
          if (Mesh3D.EltOri)
            ol_q_q_disori (Mesh3D.EltOri[elt3d[0]], Mesh3D.EltOri[elt3d[1]], Tess.CellCrySym, &tmp);
          else
            ol_q_q_disori (Mesh3D.ElsetOri[Mesh3D.EltElset[elt3d[0]]],
                           Mesh3D.ElsetOri[Mesh3D.EltElset[elt3d[1]]], Tess.CellCrySym, &tmp);

          (*pvals)[0] += tmp;
        }
      }

      if ((*pvals)[0] > 0)
        (*pvals)[0] /= Mesh2D.Elsets[id][0];

      ut_free_1d_int (&elt3d);
    }
    else if (Tess.Dim == 2 && neut_ori_des_isvalid (var)) // orientation descriptor
    {
      (*pvalqty) = ol_des_size (var);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      neut_ori_des_ori (Mesh2D.ElsetOri[id], var, *pvals);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "elset1d"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = Mesh1D.ElsetId ? Mesh1D.ElsetId[id] : id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = Mesh1D.Elsets[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elts"))
    {
      (*pvalqty) = Mesh1D.Elsets[id][0];
      (*pvals) = ut_alloc_1d (Mesh1D.Elsets[id][0]);
      ut_array_1d_memcpy_fromint (Mesh1D.Elsets[id] + 1, Mesh1D.Elsets[id][0], *pvals);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      int qty;
      neut_mesh_elset_nodes (Mesh1D, id, NULL, &qty);
      (*pvals)[0] = qty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodes"))
    {
      int qty, *tmp = NULL;
      neut_mesh_elset_nodes (Mesh1D, id, &tmp, &qty);
      (*pvalqty) = qty;
      (*pvals) = ut_alloc_1d (qty);
      ut_array_1d_memcpy_fromint (tmp, qty, *pvals);
      strcpy (typetmp, "%d");
      ut_free_1d_int (&tmp);
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elset_centre_x (Nodes, Mesh1D, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elset_centre_y (Nodes, Mesh1D, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elset_centre_z (Nodes, Mesh1D, id, *pvals);
    else if (!strcmp (var, "length"))
      neut_mesh_elset_length (Nodes, Mesh1D, id, *pvals);
    else if (!strcmp (var, "true"))
    {
      (*pvals)[0] = neut_tess_edge_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = neut_tess_edge_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "domtype"))
    {
      (*pvals)[0] = Tess.EdgeDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "cyl"))
    {
      (*pvals)[0] = neut_tess_edge_fake (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Mesh1D.ElsetGroup? Mesh1D.ElsetGroup[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "per"))
    {
      (*pvals)[0] = neut_tess_edge_isper (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "theta"))
    {
      int i, elt, elt2dqty, *elt2d = NULL;
      double tmp;

      (*pvals)[0] = 0;
      for (i = 1; i <= Mesh1D.Elsets[id][0]; i++)
      {
        elt = Mesh1D.Elsets[id][i];

        neut_mesh_elt1d_elts2d (Mesh1D, elt, Mesh2D, &elt2d, &elt2dqty);

        if (elt2dqty < 2)
        {
          (*pvals)[0] = -1;
          break;
        }
        else
        {
          if (Mesh2D.EltOri)
            ol_q_q_disori (Mesh2D.EltOri[elt2d[0]], Mesh2D.EltOri[elt2d[1]], Tess.CellCrySym, &tmp);
          else
            ol_q_q_disori (Mesh2D.ElsetOri[Mesh2D.EltElset[elt2d[0]]],
                           Mesh2D.ElsetOri[Mesh2D.EltElset[elt2d[1]]], Tess.CellCrySym, &tmp);

          (*pvals)[0] += tmp;
        }
      }

      if ((*pvals)[0] > 0)
        (*pvals)[0] /= Mesh1D.Elsets[id][0];

      ut_free_1d_int (&elt2d);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "elset0d"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = Mesh0D.ElsetId ? Mesh0D.ElsetId[id] : id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = Mesh0D.Elsets[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elts"))
    {
      (*pvalqty) = Mesh0D.Elsets[id][0];
      (*pvals) = ut_alloc_1d (Mesh0D.Elsets[id][0]);
      ut_array_1d_memcpy_fromint (Mesh0D.Elsets[id] + 1, Mesh0D.Elsets[id][0], *pvals);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      int qty;
      neut_mesh_elset_nodes (Mesh0D, id, NULL, &qty);
      (*pvals)[0] = qty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodes"))
    {
      int qty, *tmp = NULL;
      neut_mesh_elset_nodes (Mesh0D, id, &tmp, &qty);
      (*pvalqty) = qty;
      (*pvals) = ut_alloc_1d (qty);
      ut_array_1d_memcpy_fromint (tmp, qty, *pvals);
      strcpy (typetmp, "%d");
      ut_free_1d_int (&tmp);
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elset_centre_x (Nodes, Mesh0D, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elset_centre_y (Nodes, Mesh0D, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elset_centre_z (Nodes, Mesh0D, id, *pvals);
    else if (!strcmp (var, "true"))
    {
      (*pvals)[0] = neut_tess_ver_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = neut_tess_ver_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "domtype"))
    {
      (*pvals)[0] = Tess.VerDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Mesh0D.ElsetGroup? Mesh0D.ElsetGroup[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "per"))
    {
      (*pvals)[0] = neut_tess_ver_isper (Tess, id);
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "elsetco"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = MeshCo.ElsetId ? MeshCo.ElsetId[id] : id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = MeshCo.Elsets[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elts"))
    {
      (*pvalqty) = MeshCo.Elsets[id][0];
      (*pvals) = ut_alloc_1d (MeshCo.Elsets[id][0]);
      ut_array_1d_memcpy_fromint (MeshCo.Elsets[id] + 1, MeshCo.Elsets[id][0], *pvals);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      int qty;
      neut_mesh_elset_nodes (MeshCo, id, NULL, &qty);
      (*pvals)[0] = qty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodes"))
    {
      int qty, *tmp = NULL;
      neut_mesh_elset_nodes (MeshCo, id, &tmp, &qty);
      (*pvalqty) = qty;
      (*pvals) = ut_alloc_1d (qty);
      ut_array_1d_memcpy_fromint (tmp, qty, *pvals);
      strcpy (typetmp, "%d");
      ut_free_1d_int (&tmp);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "elt3d"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elt_centre_x (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elt_centre_y (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elt_centre_z (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_elt_centre (Nodes, Mesh3D, id, *pvals);
      strcpy (typetmp, "%f");
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "vol"))
      neut_mesh_elt_volume (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "diameq"))
      neut_mesh_elt_diameq (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "radeq"))
      neut_mesh_elt_radeq (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "elsetvol"))
      neut_mesh_elset_volume (Nodes, Mesh3D, Mesh3D.EltElset[id], *pvals);
    else if (!strcmp (var, "length"))
      neut_mesh_elt_length (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "lengths"))
    {
      (*pvals) = ut_realloc_1d (*pvals, 4);
      if (pvalqty)
        (*pvalqty) = 4;
      neut_mesh_elt_lengths (Nodes, Mesh3D, id, *pvals);
    }
    else if (!strcmp (var, "rr"))
      neut_mesh_elt_rr (Nodes, Mesh3D, id, *pvals);
    else if (!strcmp (var, "elset3d"))
    {
      (*pvals)[0] = Mesh3D.EltElset[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "true"))
    {
      (*pvals)[0] = neut_tess_cell_true (Tess, Mesh3D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = neut_tess_cell_body (Tess, Mesh3D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "part"))
    {
      (*pvals)[0] = Mesh3D.EltPart ? Mesh3D.EltPart[id] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Mesh3D.ElsetGroup? Mesh3D.ElsetGroup[Mesh3D.EltElset[id]] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strncmp (var, "fiber", 5))
    {
      double theta;
      double *dirc = ut_alloc_1d (3);
      double *dirs = ut_alloc_1d (3);
      double *ori = Mesh3D.EltOri ? Mesh3D.EltOri[id] : Mesh3D.ElsetOri[Mesh3D.EltElset[id]];

      neut_ori_fiber_sscanf (var, dirc, dirs, &theta, NULL);
      (*pvals)[0] = neut_ori_fiber_in (ori, Tess.CellCrySym, dirc, dirs, theta);
      strcpy (typetmp, "%d");

      ut_free_1d (&dirc);
      ut_free_1d (&dirs);
    }
    else if (Tess.Dim == 3 && neut_ori_des_isvalid (var)) // orientation descriptor
    {
      (*pvalqty) = ol_des_size (var);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);

      double *q = ol_q_alloc ();
      neut_mesh_elt_ori (Mesh3D, id, q);
      neut_ori_des_ori (q, var, *pvals);
      ol_q_free (q);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "elt2d"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elt_centre_x (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elt_centre_y (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elt_centre_z (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_elt_centre (Nodes, Mesh2D, id, *pvals);
      strcpy (typetmp, "%f");
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "area"))
      neut_mesh_elt_area (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "diameq"))
      neut_mesh_elt_diameq (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "radeq"))
      neut_mesh_elt_radeq (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "elsetarea"))
      neut_mesh_elset_area (Nodes, Mesh2D, Mesh2D.EltElset[id], *pvals);
    else if (!strcmp (var, "elset2d"))
    {
      (*pvals)[0] = Mesh2D.EltElset[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "true"))
    {
      (*pvals)[0] = neut_tess_face_true (Tess, Mesh2D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = neut_tess_face_body (Tess, Mesh2D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "domtype"))
    {
      (*pvals)[0] = Tess.FaceDom[Mesh2D.EltElset[id]][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elt3d_shown"))
    {
      int *elts = NULL;
      int EltQty;
      neut_mesh_elt0d_elts3d (Mesh0D, id, Mesh1D, Mesh2D, Mesh3D, &elts,
                              &EltQty);

      (*pvals)[0] = 0;
      for (j = 0; j < EltQty; j++)
        if (showelt3d[elts[j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
      ut_free_1d_int (&elts);
    }
    else if (!strcmp (var, "part"))
    {
      (*pvals)[0] = Mesh3D.EltPart ? Mesh3D.EltPart[id] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "length"))
      neut_mesh_elt_length (Nodes, Mesh2D, id, *pvals);
    else if (!strcmp (var, "lengths"))
    {
      (*pvals) = ut_realloc_1d (*pvals, 3);
      if (pvalqty)
        (*pvalqty) = 3;
      neut_mesh_elt_length (Nodes, Mesh2D, id, *pvals);
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Mesh2D.ElsetGroup? Mesh2D.ElsetGroup[Mesh2D.EltElset[id]] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "per"))
    {
      (*pvals)[0] = neut_tess_face_isper (Tess, Mesh2D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "theta"))
    {
      int elt3dqty, *elt3d = NULL;

      neut_mesh_elt2d_elts3d (Mesh2D, id, Mesh3D, &elt3d, &elt3dqty);

      if (elt3dqty < 2)
        (*pvals)[0] = -1;
      else
      {
        if (Mesh3D.EltOri)
          ol_q_q_disori (Mesh3D.EltOri[elt3d[0]], Mesh3D.EltOri[elt3d[1]], Tess.CellCrySym, *pvals);
        else
          ol_q_q_disori (Mesh3D.ElsetOri[Mesh3D.EltElset[elt3d[0]]],
                         Mesh3D.ElsetOri[Mesh3D.EltElset[elt3d[1]]], Tess.CellCrySym, *pvals);
      }

      ut_free_1d_int (&elt3d);
    }
    else if (Tess.Dim == 2 && neut_ori_des_isvalid (var)) // orientation descriptor
    {
      (*pvalqty) = ol_des_size (var);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);

      double *q = ol_q_alloc ();
      neut_mesh_elt_ori (Mesh2D, id, q);
      neut_ori_des_ori (q, var, *pvals);
      ol_q_free (q);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "elt1d"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elt_centre_x (Nodes, Mesh1D, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elt_centre_y (Nodes, Mesh1D, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elt_centre_z (Nodes, Mesh1D, id, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_elt_centre (Nodes, Mesh1D, id, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "length"))
      neut_mesh_elt_length (Nodes, Mesh1D, id, *pvals);
    else if (!strcmp (var, "elsetlength"))
      neut_mesh_elset_length (Nodes, Mesh1D, Mesh1D.EltElset[id], *pvals);
    else if (!strcmp (var, "elset1d"))
    {
      (*pvals)[0] = Mesh1D.EltElset[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "true"))
    {
      (*pvals)[0] = neut_tess_edge_true (Tess, Mesh1D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = neut_tess_edge_body (Tess, Mesh1D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "domtype"))
    {
      (*pvals)[0] = Tess.EdgeDom[Mesh1D.EltElset[id]][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elt3d_shown"))
    {
      int *elts = NULL;
      int EltQty;
      neut_mesh_elt1d_elts3d (Mesh1D, id, Mesh2D, Mesh3D, &elts, &EltQty);

      (*pvals)[0] = 0;
      for (j = 0; j < EltQty; j++)
        if (showelt3d[elts[j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
      ut_free_1d_int (&elts);
    }
    else if (!strcmp (var, "elt2d_shown"))
    {
      int *elts = NULL;
      int EltQty;
      neut_mesh_elt1d_elts2d (Mesh1D, id, Mesh2D, &elts, &EltQty);

      (*pvals)[0] = 0;
      for (j = 0; j < EltQty; j++)
        if (showelt2d[elts[j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
      ut_free_1d_int (&elts);
    }
    else if (!strcmp (var, "cyl"))
    {
      (*pvals)[0] = neut_tess_edge_fake (Tess, Mesh1D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "part"))
    {
      (*pvals)[0] = Mesh1D.EltPart ? Mesh1D.EltPart[id] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Mesh1D.ElsetGroup? Mesh1D.ElsetGroup[Mesh1D.EltElset[id]] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "per"))
    {
      (*pvals)[0] = neut_tess_edge_isper (Tess, Mesh1D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "theta"))
    {
      int elt2dqty, *elt2d = NULL;

      neut_mesh_elt1d_elts2d (Mesh1D, id, Mesh2D, &elt2d, &elt2dqty);

      if (elt2dqty < 2)
        (*pvals)[0] = -1;
      else
      {
        if (Mesh2D.EltOri)
          ol_q_q_disori (Mesh2D.EltOri[elt2d[0]], Mesh2D.EltOri[elt2d[1]], Tess.CellCrySym, *pvals);
        else
          ol_q_q_disori (Mesh2D.ElsetOri[Mesh2D.EltElset[elt2d[0]]],
                         Mesh2D.ElsetOri[Mesh2D.EltElset[elt2d[1]]], Tess.CellCrySym, *pvals);
      }

      ut_free_1d_int (&elt2d);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "elt0d"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elt_centre_x (Nodes, Mesh0D, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elt_centre_y (Nodes, Mesh0D, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elt_centre_z (Nodes, Mesh0D, id, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_elt_centre (Nodes, Mesh0D, id, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "elset0d"))
    {
      (*pvals)[0] = Mesh0D.EltElset[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "true"))
    {
      (*pvals)[0] = neut_tess_ver_true (Tess, Mesh0D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = neut_tess_ver_body (Tess, Mesh0D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "domtype"))
    {
      (*pvals)[0] = Tess.VerDom[Mesh0D.EltElset[id]][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "length"))
      (*pvals)[0] = 0;
    else if (!strcmp (var, "elt3d_shown"))
    {
      int *elts = NULL;
      int EltQty;
      neut_mesh_elt0d_elts3d (Mesh0D, id, Mesh1D, Mesh2D, Mesh3D, &elts,
                              &EltQty);

      (*pvals)[0] = 0;
      for (j = 0; j < EltQty; j++)
        if (showelt3d[elts[j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
      ut_free_1d_int (&elts);
    }
    else if (!strcmp (var, "elt2d_shown"))
    {
      int *elts = NULL;
      int EltQty;
      neut_mesh_elt0d_elts2d (Mesh0D, id, Mesh1D, Mesh2D, &elts, &EltQty);

      (*pvals)[0] = 0;
      for (j = 0; j < EltQty; j++)
        if (showelt2d[elts[j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
      ut_free_1d_int (&elts);
    }
    else if (!strcmp (var, "elt1d_shown"))
    {
      int *elts = NULL;
      int EltQty;
      neut_mesh_elt0d_elts1d (Mesh0D, id, Mesh1D, &elts, &EltQty);

      (*pvals)[0] = 0;
      for (j = 0; j < EltQty; j++)
        if (showelt1d[elts[j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
      ut_free_1d_int (&elts);
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Mesh0D.ElsetGroup? Mesh0D.ElsetGroup[Mesh0D.EltElset[id]] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "per"))
    {
      (*pvals)[0] = neut_tess_ver_isper (Tess, Mesh0D.EltElset[id]);
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "eltco"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
      neut_mesh_elt_centre_x (Nodes, MeshCo, id, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_elt_centre_y (Nodes, MeshCo, id, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_elt_centre_z (Nodes, MeshCo, id, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_elt_centre (Nodes, MeshCo, id, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "elsetco"))
    {
      (*pvals)[0] = MeshCo.EltElset[id];
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "mesh0d"))
  {
    status = 0;
    if (!strcmp (var, "x"))
      neut_mesh_centre_x (Nodes, Mesh0D, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_centre_y (Nodes, Mesh0D, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_centre_z (Nodes, Mesh0D, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_centre (Nodes, Mesh0D, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "size"))
      neut_mesh_size (Nodes, Mesh0D, *pvals);
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = Mesh0D.EltQty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      (*pvals)[0] = Mesh0D.NodeQty;
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "mesh1d"))
  {
    status = 0;
    if (!strcmp (var, "x"))
      neut_mesh_centre_x (Nodes, Mesh1D, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_centre_y (Nodes, Mesh1D, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_centre_z (Nodes, Mesh1D, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_centre (Nodes, Mesh1D, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "length"))
      neut_mesh_length (Nodes, Mesh1D, *pvals);
    else if (!strcmp (var, "size"))
      neut_mesh_size (Nodes, Mesh1D, *pvals);
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = Mesh1D.EltQty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      (*pvals)[0] = Mesh1D.NodeQty;
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "mesh2d"))
  {
    status = 0;
    if (!strcmp (var, "x"))
      neut_mesh_centre_x (Nodes, Mesh2D, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_centre_y (Nodes, Mesh2D, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_centre_z (Nodes, Mesh2D, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_centre (Nodes, Mesh2D, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "area"))
      neut_mesh_area (Nodes, Mesh2D, *pvals);
    else if (!strcmp (var, "size"))
      neut_mesh_size (Nodes, Mesh2D, *pvals);
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = Mesh2D.EltQty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      (*pvals)[0] = Mesh2D.NodeQty;
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "mesh3d"))
  {
    status = 0;
    if (!strcmp (var, "x"))
      neut_mesh_centre_x (Nodes, Mesh3D, *pvals);
    else if (!strcmp (var, "y"))
      neut_mesh_centre_y (Nodes, Mesh3D, *pvals);
    else if (!strcmp (var, "z"))
      neut_mesh_centre_z (Nodes, Mesh3D, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_mesh_centre (Nodes, Mesh3D, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "vol"))
      neut_mesh_volume (Nodes, Mesh3D, *pvals);
    else if (!strcmp (var, "size"))
      neut_mesh_size (Nodes, Mesh3D, *pvals);
    else if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = Mesh3D.EltQty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "nodenb"))
    {
      (*pvals)[0] = Mesh3D.NodeQty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "partnb"))
    {
      (*pvals)[0] = Mesh3D.PartQty;
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "meshco"))
  {
    status = 0;
    if (!strcmp (var, "eltnb"))
    {
      (*pvals)[0] = MeshCo.EltQty;
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "node"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
      (*pvals)[0] = Nodes.NodeCoo[id][0];
    else if (!strcmp (var, "y"))
      (*pvals)[0] = Nodes.NodeCoo[id][1];
    else if (!strcmp (var, "z"))
      (*pvals)[0] = Nodes.NodeCoo[id][2];
    else if (!strcmp (var, "coo"))
    {
      ut_array_1d_memcpy (Nodes.NodeCoo[id], 3, *pvals);
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else if (!strcmp (var, "dim"))
    {
      (*pvals)[0] = neut_mesh_node_dim (Mesh0D, Mesh1D, Mesh2D, Mesh3D, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elt3d_shown"))
    {
      (*pvals)[0] = 0;
      for (j = 1; j <= Mesh3D.NodeElts[id][0]; j++)
        if (showelt3d[Mesh3D.NodeElts[id][j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elt2d_shown"))
    {
      (*pvals)[0] = 0;
      for (j = 1; j <= Mesh2D.NodeElts[id][0]; j++)
        if (showelt2d[Mesh2D.NodeElts[id][j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elt1d_shown"))
    {
      (*pvals)[0] = 0;
      for (j = 1; j <= Mesh1D.NodeElts[id][0]; j++)
        if (showelt1d[Mesh1D.NodeElts[id][j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "elt0d_shown"))
    {
      (*pvals)[0] = 0;
      for (j = 1; j <= Mesh0D.NodeElts[id][0]; j++)
        if (showelt0d[Mesh0D.NodeElts[id][j]])
        {
          (*pvals)[0] = 1;
          break;
        }

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "part"))
    {
      (*pvals)[0] = Nodes.NodePart ? Nodes.NodePart[id] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "col_rodrigues"))
    {
      neut_ori_rodriguescol_R (Nodes.NodeCoo[id], NULL, *pvals);
      (*pvalqty) = 3;
    }
    else if (!strcmp (var, "col_stdtriangle"))
    {
      int *col = ut_alloc_1d_int (3);
      double *xy = ut_alloc_1d (2), *v = ut_alloc_1d (3), *weight = ut_alloc_1d (2);
      ut_array_1d_set_2 (xy, Nodes.NodeCoo[id][1], Nodes.NodeCoo[id][0]);
      ol_stprojxy_vect (xy, v);
      ol_vect_ipfweight (v, weight);
      ol_ipfweight_rgb (weight, col);
      ut_array_1d_memcpy_fromint (col, 3, *pvals);
      (*pvalqty) = 3;
      strcpy (typetmp, "%d");
      ut_free_1d_int (&col);
      ut_free_1d (&v);
      ut_free_1d (&xy);
      ut_free_1d (&weight);
    }
    else
      status = -1;
  }
  else
    status = -1;

  if (status == -1 && Nodes.pSim)
    status = neut_sim_entity_id_res_val (*(Nodes.pSim), entity, id, var, *pvals);

  if (ptype)
  {
    (*ptype) = ut_realloc_1d_char (*ptype, strlen (typetmp) + 1);
    strcpy (*ptype, typetmp);
  }

  ut_free_1d_char (&typetmp);
  ut_free_1d_char (&entity);

  return status;
}

int
neut_mesh_var_val_one (struct NODES Nodes, struct MESH Mesh0D,
                       struct MESH Mesh1D, struct MESH Mesh2D,
                       struct MESH Mesh3D, struct MESH MeshCo,
                       struct TESS Tess,
                       int *showelt0d, int *showelt1d, int *showelt2d,
                       int *showelt3d, double cl, char *entity, int id,
                       char *var, double *pval, char **ptype)
{
  int status, qty;
  double *tmp = NULL;

  status = neut_mesh_var_val (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, MeshCo, Tess,
                              showelt0d, showelt1d, showelt2d, showelt3d, cl, entity,
                              id, var, &tmp, &qty, ptype);

  if (status == 0 && qty == 1)
  {
    (*pval) = tmp[0];
    status = 0;
  }
  else
    status = -1;

  ut_free_1d (&tmp);

  return status;
}

int
neut_mesh_array_dim (struct MESH *Mesh)
{
  int i;

  for (i = 3; i >= 0; i--)
    if (Mesh[i].EltQty > 0)
      return i;

  return -1;
}

int
neut_mesh_all_dim (struct MESH Mesh0D, struct MESH Mesh1D,
                   struct MESH Mesh2D, struct MESH Mesh3D,
                   struct MESH MeshCo)
{
  if (MeshCo.EltQty)
    return 4;
  else if (Mesh3D.EltQty)
    return 3;
  else if (Mesh2D.EltQty)
    return 2;
  else if (Mesh1D.EltQty)
    return 1;
  else if (Mesh0D.EltQty)
    return 0;
  else
    abort ();

  return -1;
}

void
neut_mesh_entity_expr_val (struct NODES Nodes, struct MESH Mesh0D,
                           struct MESH Mesh1D, struct MESH Mesh2D,
                           struct MESH Mesh3D, struct MESH MeshCo,
                           struct TESS Tess, int *showelt0d, int *showelt1d,
                           int *showelt2d, int *showelt3d, char *entity,
                           char *expr, double *val, char **ptype)
{
  int j, entityqty, varqty;
  char **vars = NULL;
  FILE *file = NULL;
  char *type = NULL;

  if (ptype)
    ut_string_string ("%d", ptype);

  neut_mesh_entity_qty (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, entity,
                        &entityqty);

  ut_math_vars (expr, &vars, &varqty);

  if (ut_string_isfilename (expr))
  {
    file = ut_file_open (expr, "R");
    ut_array_1d_fscanf (file, val + 1, entityqty);
    ut_file_close (file, expr, "R");
  }

  else if (varqty == 1 && !strcmp (expr, vars[0]))
    for (j = 1; j <= entityqty; j++)
      neut_mesh_var_val_one (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, MeshCo,
                             Tess, showelt0d, showelt1d, showelt2d,
                             showelt3d, 0, entity, j, expr, val + j,
                             ptype);

  else
  {
#pragma omp parallel for
    for (j = 1; j <= entityqty; j++)
    {
      int k, status;
      double *vals = ut_alloc_1d (varqty);

      for (k = 0; k < varqty; k++)
      {
        if (!strcmp (vars[k], "default"))
          vals[k] = val[j];

        neut_mesh_var_val_one (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, MeshCo,
                               Tess, showelt0d, showelt1d, showelt2d,
                               showelt3d, 0, entity, j, vars[k], vals + k,
                               &type);

        // write only once
        if (j == entityqty && k== varqty - 1 && ptype && !strcmp (type, "%f"))
          ut_string_string ("%f", ptype);
      }

      status = ut_math_eval (expr, varqty, vars, vals, val + j);
      if (status == -1)
        abort ();

      ut_free_1d (&vals);
    }
  }

  ut_free_2d_char (&vars, varqty);
  ut_free_1d_char (&type);

  return;
}

void
neut_mesh_entity_expr_val_int (struct NODES Nodes, struct MESH Mesh0D,
                               struct MESH Mesh1D, struct MESH Mesh2D,
                               struct MESH Mesh3D, struct MESH MeshCo,
                               struct TESS Tess, int *showelt0d, int *showelt1d,
                               int *showelt2d, int *showelt3d, char *entity,
                               char *expr, int *val)
{
  int qty;
  double *tmp = NULL;

  neut_mesh_entity_qty (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, entity,
                        &qty);
  tmp = ut_alloc_1d (qty + 1);

  neut_mesh_entity_expr_val (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, MeshCo,
                             Tess, showelt0d, showelt1d, showelt2d, showelt3d,
                             entity, expr, tmp, NULL);
  ut_array_1d_round (tmp + 1, qty, val + 1);

  ut_free_1d (&tmp);

  return;
}

void
neut_mesh_elset_bodynodes (struct TESS Tess, struct MESH MeshL, struct MESH MeshU,
                           int elset, int **pnodes, int *pnodeqty)
{
  int i, j, dim = MeshU.Dimension, boundqty, bound;
  int nodeqty2, *nodes2 = NULL;

  if (MeshL.Dimension + 1 != MeshU.Dimension)
  {
    printf ("MeshL.Dimension = %d\n", MeshL.Dimension);
    printf ("MeshU.Dimension = %d\n", MeshU.Dimension);
    abort ();
  }

  if (dim == 1)
    boundqty = 2;
  else if (dim == 2)
    boundqty = Tess.FaceVerQty[elset];
  else if (dim == 3)
    boundqty = Tess.PolyFaceQty[elset];
  else
    abort ();

  neut_mesh_elset_nodes (MeshU, elset, pnodes, pnodeqty);

  for (i = 1; i <= boundqty; i++)
  {
    if (dim == 1)
      bound = Tess.EdgeVerNb[elset][i - 1];
    else if (dim == 2)
      bound = Tess.FaceEdgeNb[elset][i];
    else if (dim == 3)
      bound = Tess.PolyFaceNb[elset][i];
    else
      abort ();

    neut_mesh_elset_nodes (MeshL, bound, &nodes2, &nodeqty2);

    for (j = 0; j < nodeqty2; j++)
      (*pnodeqty) -= ut_array_1d_int_rmelt (*pnodes, *pnodeqty, nodes2[j], 1);
  }

  (*pnodes) = ut_realloc_1d_int (*pnodes, *pnodeqty);

  ut_free_1d_int (&nodes2);

  return;
}

void
neut_mesh_elset_bodynodes_1d (struct TESS Tess, struct MESH Mesh0D, struct MESH Mesh1D,
                             int elset, int **pnodes, int *pnodeqty)

{
  int i, j, ver, nodeqty2, *nodes2 = NULL;

  neut_mesh_elset_nodes (Mesh1D, elset, pnodes, pnodeqty);

  for (i = 0; i < 2; i++)
  {
    ver = Tess.EdgeVerNb[elset][i];

    neut_mesh_elset_nodes (Mesh0D, ver, &nodes2, &nodeqty2);

    for (j = 0; j < nodeqty2; j++)
      (*pnodeqty) -= ut_array_1d_int_rmelt (*pnodes, *pnodeqty, nodes2[j], 1);
  }

  (*pnodes) = ut_realloc_1d_int (*pnodes, *pnodeqty);

  ut_free_1d_int (&nodes2);

  return;
}

void
neut_mesh_elset_bodynodes_2d (struct TESS Tess, struct MESH Mesh1D, struct MESH Mesh2D, int elset,
                             int **pnodes, int *pnodeqty)
{
  int i, j, edge, nodeqty2, *nodes2 = NULL;

  neut_mesh_elset_nodes (Mesh2D, elset, pnodes, pnodeqty);

  for (i = 1; i <= Tess.FaceVerQty[elset]; i++)
  {
    edge = Tess.FaceEdgeNb[elset][i];

    neut_mesh_elset_nodes (Mesh1D, edge, &nodes2, &nodeqty2);

    for (j = 0; j < nodeqty2; j++)
      (*pnodeqty) -= ut_array_1d_int_rmelt (*pnodes, *pnodeqty, nodes2[j], 1);
  }

  (*pnodes) = ut_realloc_1d_int (*pnodes, *pnodeqty);

  ut_free_1d_int (&nodes2);

  return;
}

void
neut_mesh_elset_bodynodes_3d (struct TESS Tess, struct MESH Mesh2D, struct MESH Mesh3D, int elset,
                             int **pnodes, int *pnodeqty)
{
  int i, j, face, nodeqty2, *nodes2 = NULL;

  neut_mesh_elset_nodes (Mesh3D, elset, pnodes, pnodeqty);

  for (i = 1; i <= Tess.PolyFaceQty[elset]; i++)
  {
    face = Tess.PolyFaceNb[elset][i];

    neut_mesh_elset_nodes (Mesh2D, face, &nodes2, &nodeqty2);

    for (j = 0; j < nodeqty2; j++)
      (*pnodeqty) -= ut_array_1d_int_rmelt (*pnodes, *pnodeqty, nodes2[j], 1);
  }

  (*pnodes) = ut_realloc_1d_int (*pnodes, *pnodeqty);

  ut_free_1d_int (&nodes2);

  return;
}

void
neut_mesh1d_mesh2d (struct NODES *pNodes, struct MESH Mesh1D,
                    struct MESH *pMesh2D)
{
  int i;
  double *centre = ut_alloc_1d (3);
  double **coo = ut_alloc_2d (Mesh1D.EltQty + 1, 3);

  // taking the centroid of all 1D elements as the centre
  for (i = 1; i <= Mesh1D.EltQty; i++)
  {
    neut_mesh_elt_centre (*pNodes, Mesh1D, i, coo[i]);
    ut_array_1d_add (centre, coo[i], 3, centre);
  }
  ut_array_1d_scale (centre, 3, 1. / Mesh1D.EltQty);

  neut_nodes_addnode (pNodes, centre, 0);

  neut_mesh_free (pMesh2D);
  (*pMesh2D) = neut_mesh_alloc (2, "tri", 1, Mesh1D.EltQty, 1);

  ut_array_2d_int_memcpy (Mesh1D.EltNodes + 1, Mesh1D.EltQty, 2,
                          (*pMesh2D).EltNodes + 1);
  for (i = 1; i <= Mesh1D.EltQty; i++)
    (*pMesh2D).EltNodes[i][2] = (*pNodes).NodeQty;

  (*pMesh2D).EltElset = ut_alloc_1d_int ((*pMesh2D).EltQty + 1);
  ut_array_1d_int_set ((*pMesh2D).EltElset + 1, (*pMesh2D).EltQty, 1);
  neut_mesh_init_elsets (pMesh2D);

  ut_free_1d (&centre);
  ut_free_2d (&coo, Mesh1D.EltQty + 1);

  return;
}

void
neut_mesh2d_mesh3d (struct NODES *pNodes, struct MESH Mesh2D,
                    struct MESH *pMesh3D)
{
  int i;
  double *centre = ut_alloc_1d (3);
  double **coo = ut_alloc_2d (Mesh2D.EltQty + 1, 3);

  // taking the centroid of all 2D elements as the centre
  for (i = 1; i <= Mesh2D.EltQty; i++)
  {
    neut_mesh_elt_centre (*pNodes, Mesh2D, i, coo[i]);
    ut_array_1d_add (centre, coo[i], 3, centre);
  }
  ut_array_1d_scale (centre, 3, 1. / Mesh2D.EltQty);

  neut_nodes_addnode (pNodes, centre, 0);

  neut_mesh_free (pMesh3D);
  (*pMesh3D) = neut_mesh_alloc (3, "tri", 1, Mesh2D.EltQty, 1);

  ut_array_2d_int_memcpy (Mesh2D.EltNodes + 1, Mesh2D.EltQty, 3,
                          (*pMesh3D).EltNodes + 1);
  for (i = 1; i <= Mesh2D.EltQty; i++)
    (*pMesh3D).EltNodes[i][3] = (*pNodes).NodeQty;

  (*pMesh3D).EltElset = ut_alloc_1d_int ((*pMesh3D).EltQty + 1);
  ut_array_1d_int_set ((*pMesh3D).EltElset + 1, (*pMesh3D).EltQty, 1);
  neut_mesh_init_elsets (pMesh3D);

  ut_free_1d (&centre);
  ut_free_2d (&coo, Mesh2D.EltQty + 1);

  return;
}

void
neut_mesh_eltdata_elsetdata (struct NODES Nodes, struct MESH Mesh,
                             int **elsets, int elsetqty,
                             double **eltdata, int size,
                             double **elsetdata)
{
  int i;

  ut_array_2d_zero (elsetdata + 1, elsetqty, size);

#pragma omp parallel for private(i)
  for (i = 1; i <= elsetqty; i++)
  {
    int j, k, elt;
    double vol, totvol;

    totvol = 0;
    for (j = 1; j <= elsets[i][0]; j++)
    {
      elt = elsets[i][j];

      if (Mesh.Dimension == 2)
        neut_mesh_elt_area (Nodes, Mesh, elt, &vol);
      else if (Mesh.Dimension == 3)
        neut_mesh_elt_volume (Nodes, Mesh, elt, &vol);
      else
        abort ();

      totvol += vol;
      for (k = 0; k < size; k++)
        elsetdata[i][k] += vol * eltdata[elt][k];
    }

    ut_array_1d_scale (elsetdata[i], size, 1 / totvol);
  }

  return;
}

void
neut_mesh_eltdata_elsetdata_ori (struct NODES Nodes, struct MESH Mesh,
                                 int **elsets, int elsetqty, double **eltdata,
                                 char *crysym, double **elsetdata)
{
  int i;

  ut_array_2d_zero (elsetdata + 1, elsetqty, 4);

#pragma omp parallel for private(i) schedule(dynamic)
  for (i = 1; i <= elsetqty; i++)
    neut_mesh_elts_orimean (Nodes, Mesh, elsets[i] + 1, elsets[i][0],
                            eltdata, crysym, elsetdata[i]);

  return;
}

void
neut_mesh_eltdata_elsetdata_oridis (struct NODES Nodes, struct MESH Mesh,
                                    int **elsets, int elsetqty,
                                    double **eltdata, char *crysym, double ***elsetevect,
                                    double **elseteval)
{
  int i;
  double ***evect = ut_alloc_3d (elsetqty + 1, 3, 3);
  double **eval = ut_alloc_2d (elsetqty + 1, 3);

#pragma omp parallel for private(i) schedule(dynamic)
  for (i = 1; i <= elsetqty; i++)
    neut_mesh_elts_orianiso (Nodes, Mesh, elsets[i] + 1, elsets[i][0],
                             eltdata, crysym, evect[i], eval[i]);

  if (elseteval)
    ut_array_2d_memcpy (eval + 1, elsetqty, 3, elseteval + 1);
  if (elsetevect)
    ut_array_3d_memcpy (evect + 1, elsetqty, 3, 3, elsetevect + 1);

  ut_free_2d (&eval, elsetqty + 1);
  ut_free_3d (&evect, elsetqty + 1, 3);

  return;
}

void
neut_mesh_aselsets (struct MESH Mesh, int ***pelsets, int *pelsetqty)
{
  *pelsetqty = 1;
  (*pelsets) = ut_alloc_1d_pint (2);
  (*pelsets)[1] = ut_alloc_1d_int (Mesh.EltQty + 1);
  ut_array_1d_int_set_id ((*pelsets)[1], Mesh.EltQty + 1);
  (*pelsets)[1][0] = Mesh.EltQty;

  return;
}

void
neut_mesh_entity_expr_matches (struct TESS Tess, struct NODES Nodes,
                               struct MESH Mesh0D, struct MESH Mesh1D,
                               struct MESH Mesh2D, struct MESH Mesh3D,
                               struct MESH MeshCo, char *entity,
                               char *expr, int **pmatches, int *pmatchqty)
{
  int i, entityqty;
  double *tmp = NULL;

  neut_mesh_entity_qty (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, entity,
                        &entityqty);

  tmp = ut_alloc_1d (entityqty + 1);

  neut_mesh_entity_expr_val (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D,
                             MeshCo, Tess, NULL, NULL, NULL, NULL,
                             entity, expr, tmp, NULL);

  (*pmatchqty) = 0;
  (*pmatches) = ut_alloc_1d_int (entityqty);

  for (i = 1; i <= entityqty; i++)
    if (ut_num_equal (tmp[i], 1, 1e-6))
      (*pmatches)[(*pmatchqty)++] = i;

  (*pmatches) = ut_realloc_1d_int (*pmatches, *pmatchqty);

  ut_free_1d (&tmp);

  return;
}

int
neut_mesh_entity_known (char *entity)
{
  if (!strcmp (entity, "elt")
   || !strcmp (entity, "elt0d")
   || !strcmp (entity, "elt1d")
   || !strcmp (entity, "elt2d")
   || !strcmp (entity, "elt3d")
   || !strcmp (entity, "elts")
   || !strcmp (entity, "elset")
   || !strcmp (entity, "elset0d")
   || !strcmp (entity, "elset1d")
   || !strcmp (entity, "elset2d")
   || !strcmp (entity, "elset3d")
   || !strcmp (entity, "elsets")
   || !strcmp (entity, "mesh")
   || !strcmp (entity, "mesh0d")
   || !strcmp (entity, "mesh1d")
   || !strcmp (entity, "mesh2d")
   || !strcmp (entity, "mesh3d")
   || !strcmp (entity, "nodes"))
    return 1;
  else
    return 0;
}
