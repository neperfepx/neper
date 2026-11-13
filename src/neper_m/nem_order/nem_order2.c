/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

void
nem_order_pre (struct IN_M In, struct MESH *Mesh, int *domesh)
{
  int i, dim = neut_mesh_array_dim (Mesh);

  for (i = 0; i <= dim; i++)
    if (Mesh[i].EltQty > 0)
      domesh[i] = 1;

  if (!strcmp (In.interface, "cohesive"))
  {
    domesh[4] = 1;
    // disabling lower dimensions
    domesh[2] = 0;
    domesh[1] = 0;
    domesh[0] = 0;
  }

  if (Mesh[5].EltQty > 0)
    domesh[5] = 1;

  if (dim == 2 && !strcmp (Mesh[2].EltType, "tri") && Mesh[4].EltQty > 0
      && !strcmp (Mesh[4].EltType, "quad"))
    ut_string_string ("quad6", &Mesh[4].EltType);

  return;
}

void
nem_order_dim (struct IN_M In, struct NODES *pNodes, struct MESH *pMeshU,
               struct MESH *pMesh)
{
  int i, j, node_o2, nodeqty_o1, nodeqty_tot, nodes[2], **fir = NULL;
  double coo[3];

  (*pMesh).EltOrder = 2;
  if ((*pMesh).Dimension == 2 && !strcmp (In.elttype, "quad9"))
    ut_string_string (In.elttype, &((*pMesh).EltType)); // needed by quad9

  neut_elt_orderarrays ((*pMesh).EltType, (*pMesh).Dimension, &fir,
                        &nodeqty_tot, NULL, &nodeqty_o1);

  ut_realloc_2d_int_size2 ((*pMesh).EltNodes + 1, (*pMesh).EltQty, nodeqty_o1,
                           nodeqty_tot);

  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = nodeqty_o1; j < nodeqty_tot; j++)
      if (!(*pMesh).EltNodes[i][j])
      {
        nodes[0] = (*pMesh).EltNodes[i][fir[j][0]];
        nodes[1] = (*pMesh).EltNodes[i][fir[j][1]];

        if (pMeshU)
          neut_mesh_order1nodes_order2node (*pMeshU, nodes[0], nodes[1],
                                            &node_o2);

        if (pMeshU && node_o2 != -1)
          nem_order_dim_record (pMesh, nodes, node_o2);
        else
        {
          neut_nodes_bary (*pNodes, nodes, 2, coo, NULL);
          node_o2 = neut_nodes_addnode (pNodes, coo, -1);
          nem_order_dim_record (pMesh, nodes, node_o2);
        }
      }

  neut_mesh_init_nodeelts (pMesh, (*pNodes).NodeQty);

  ut_free_2d_int (&fir, nodeqty_tot);

  return;
}

// projecting the order-2 nodes of the elements of curved faces onto their
// domain faces, and those of the elements of curved edges onto their domain
// edges (3D: intersection of the domain faces), in this order so that the
// edge projection prevails
void
nem_order_curved (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh)
{
  int i, j, edge, face, domedge, nodeqty_o1, nodeqty_tot, *domfaces = NULL, domfaceqty;
  double *coo = NULL;

  if (Tess.Dim == 3 && Tess.DomFaceType && Mesh[2].EltOrder == 2
      && Mesh[2].ElsetQty == Tess.FaceQty)
  {
    nodeqty_o1 = neut_elt_nodeqty (Mesh[2].EltType, 2, 1);
    nodeqty_tot = neut_elt_nodeqty (Mesh[2].EltType, 2, 2);

    for (i = 1; i <= Mesh[2].EltQty; i++)
    {
      face = Mesh[2].EltElset[i];

      if (neut_tess_face_iscurved (Tess, face))
        for (j = nodeqty_o1; j < nodeqty_tot; j++)
        {
          coo = (*pNodes).NodeCoo[Mesh[2].EltNodes[i][j]];
          neut_primparms_point_proj (Tess.DomFaceType[Tess.FaceDom[face][1]],
                                     Tess.DomFaceParms[Tess.FaceDom[face][1]], coo, coo);
        }
    }
  }

  // (the domain edge types are not defined for a tessellation reconstructed
  // from a raster tessellation)
  if (Tess.EdgeDom && (Tess.Dim == 2 ? Tess.DomEdgeType != NULL : Tess.DomFaceType != NULL)
      && Mesh[1].EltOrder == 2
      && Mesh[1].ElsetQty == Tess.EdgeQty)
    for (i = 1; i <= Mesh[1].EltQty; i++)
    {
      edge = Mesh[1].EltElset[i];

      if (!neut_tess_edge_iscurved (Tess, edge))
        continue;

      coo = (*pNodes).NodeCoo[Mesh[1].EltNodes[i][2]];

      if (Tess.Dim == 2)
      {
        domedge = Tess.EdgeDom[edge][1];
        neut_primparms_point_proj (Tess.DomEdgeType[domedge], Tess.DomEdgeParms[domedge],
                                   coo, coo);
      }

      else
      {
        neut_tess_edge_domfaces (Tess, edge, &domfaces, &domfaceqty);
        neut_tess_domfaces_point_proj (Tess, domfaces, domfaceqty, coo, coo);
        ut_free_1d_int (&domfaces);
      }
    }

  return;
}

void
nem_order_periodic (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh)
{
  // with -interface, the mesh nodes no longer match the tessellation
  if ((*pNodes).DupNodeQty > 0)
    nem_order_periodic_dup (pNodes, Mesh);

  else
  {
    nem_order_periodic_edges (Tess, pNodes, Mesh);

    if (Tess.Dim == 3)
      nem_order_periodic_faces (Tess, pNodes, Mesh);
  }

  neut_nodes_init_nodeslave (pNodes);

  return;
}

void
nem_order_str_per_2d (int NodeQty_prev, struct MESHPARA MeshPara, struct NODES *pNodes,
                      struct MESH *Mesh)
{
  int pri_n1, pri_n2, pri_no;
  int sec_n1, sec_n2, sec_no;
  int i, x, y, **NMap = NULL;
  int *size = MeshPara.msize; // do not free
  int *shift = ut_alloc_1d_int (3);
  int dim = neut_mesh_array_dim (Mesh);

  NMap = ut_alloc_2d_int (size[0] + 2, size[1] + 2);

  ut_array_2d_int_grid1 (NMap, size[0] + 1, size[1] + 1);

  (*pNodes).PerNodeMaster = ut_realloc_1d_int ((*pNodes).PerNodeMaster, (*pNodes).NodeQty + 1);
  (*pNodes).PerNodeShift = ut_realloc_1d_pint ((*pNodes).PerNodeShift, (*pNodes).NodeQty + 1);
  (*pNodes).PerNodeSlaveQty = ut_realloc_1d_int ((*pNodes).PerNodeSlaveQty, (*pNodes).NodeQty + 1);
  (*pNodes).PerNodeSlaveNb = ut_realloc_1d_pint ((*pNodes).PerNodeSlaveNb, (*pNodes).NodeQty + 1);
  for (i = NodeQty_prev + 1; i <= (*pNodes).NodeQty; i++)
  {
    (*pNodes).PerNodeShift[i] = ut_alloc_1d_int (3);
    (*pNodes).PerNodeSlaveNb[i] = ut_alloc_1d_int (1);
  }

  if ((*pNodes).Periodic[0])
  {
    ut_array_1d_int_zero (shift, 3);
    shift[0] = 1;

    // scanning segments || y
    for (y = 1; y <= size[1]; y++)
    {
      pri_n1 = NMap[1][y];
      pri_n2 = NMap[1][y + 1];
      neut_mesh_order1nodes_order2node (Mesh[dim], pri_n1, pri_n2, &pri_no);
      sec_n1 = NMap[size[0] + 1][y];
      sec_n2 = NMap[size[0] + 1][y + 1];
      neut_mesh_order1nodes_order2node (Mesh[dim], sec_n1, sec_n2, &sec_no);
      neut_nodes_markasslave (pNodes, sec_no, pri_no, shift);
    }
  }

  if ((*pNodes).Periodic[1])
  {
    ut_array_1d_int_zero (shift, 3);
    shift[1] = 1;

    // scanning segments || x
    for (x = 1; x <= size[0]; x++)
    {
      pri_n1 = NMap[x][1];
      pri_n2 = NMap[x + 1][1];
      neut_mesh_order1nodes_order2node (Mesh[dim], pri_n1, pri_n2, &pri_no);
      sec_n1 = NMap[x][size[1] + 1];
      sec_n2 = NMap[x + 1][size[1] + 1];
      neut_mesh_order1nodes_order2node (Mesh[dim], sec_n1, sec_n2, &sec_no);
      neut_nodes_markasslave (pNodes, sec_no, pri_no, shift);
    }
  }

  neut_nodes_fixperslaves (pNodes);

  ut_free_2d_int (&NMap, size[0] + 2);
  ut_free_1d_int (&shift);

  return;
}

void
nem_order_str_per_3d (int NodeQty_prev, struct MESHPARA MeshPara, struct NODES *pNodes,
                      struct MESH *Mesh)
{
  int pri_n1, pri_n2, pri_no;
  int sec_n1, sec_n2, sec_no;
  int i, x, y, z, ***NMap = NULL;
  int *size = MeshPara.msize; // do not free
  int *shift = ut_alloc_1d_int (3);
  int dim = neut_mesh_array_dim (Mesh);

  NMap = ut_alloc_3d_int (size[0] + 2, size[1] + 2, size[2] + 2);

  ut_array_3d_int_grid1 (NMap, size[0] + 1, size[1] + 1, size[2] + 1);

  (*pNodes).PerNodeMaster = ut_realloc_1d_int ((*pNodes).PerNodeMaster, (*pNodes).NodeQty + 1);
  (*pNodes).PerNodeShift = ut_realloc_1d_pint ((*pNodes).PerNodeShift, (*pNodes).NodeQty + 1);
  (*pNodes).PerNodeSlaveQty = ut_realloc_1d_int ((*pNodes).PerNodeSlaveQty, (*pNodes).NodeQty + 1);
  (*pNodes).PerNodeSlaveNb = ut_realloc_1d_pint ((*pNodes).PerNodeSlaveNb, (*pNodes).NodeQty + 1);
  for (i = NodeQty_prev + 1; i <= (*pNodes).NodeQty; i++)
  {
    (*pNodes).PerNodeShift[i] = ut_alloc_1d_int (3);
    (*pNodes).PerNodeSlaveNb[i] = ut_alloc_1d_int (1);
  }

  if ((*pNodes).Periodic[0])
  {
    ut_array_1d_int_zero (shift, 3);
    shift[0] = 1;

    // scanning segments || y
    for (z = 1; z <= size[2] + 1; z++)
      for (y = 1; y <= size[1]; y++)
      {
        pri_n1 = NMap[1][y][z];
        pri_n2 = NMap[1][y + 1][z];
        neut_mesh_order1nodes_order2node (Mesh[dim], pri_n1, pri_n2, &pri_no);
        sec_n1 = NMap[size[0] + 1][y][z];
        sec_n2 = NMap[size[0] + 1][y + 1][z];
        neut_mesh_order1nodes_order2node (Mesh[dim], sec_n1, sec_n2, &sec_no);
        if ((*pNodes).PerNodeMaster[sec_no] == 0)
          neut_nodes_markasslave (pNodes, sec_no, pri_no, shift);
      }

    // scanning segments || z
    for (y = 1; y <= size[1] + 1; y++)
      for (z = 1; z <= size[2]; z++)
      {
        pri_n1 = NMap[1][y][z];
        pri_n2 = NMap[1][y][z + 1];
        neut_mesh_order1nodes_order2node (Mesh[dim], pri_n1, pri_n2, &pri_no);
        sec_n1 = NMap[size[0] + 1][y][z];
        sec_n2 = NMap[size[0] + 1][y][z + 1];
        neut_mesh_order1nodes_order2node (Mesh[dim], sec_n1, sec_n2, &sec_no);
        if ((*pNodes).PerNodeMaster[sec_no] == 0)
          neut_nodes_markasslave (pNodes, sec_no, pri_no, shift);
      }
  }

  if ((*pNodes).Periodic[1])
  {
    ut_array_1d_int_zero (shift, 3);
    shift[1] = 1;

    // scanning segments || z
    for (x = 1; x <= size[0] + 1; x++)
      for (z = 1; z <= size[0]; z++)
      {
        pri_n1 = NMap[x][1][z];
        pri_n2 = NMap[x][1][z + 1];
        neut_mesh_order1nodes_order2node (Mesh[dim], pri_n1, pri_n2, &pri_no);
        sec_n1 = NMap[x][size[1] + 1][z];
        sec_n2 = NMap[x][size[1] + 1][z + 1];
        neut_mesh_order1nodes_order2node (Mesh[dim], sec_n1, sec_n2, &sec_no);
        if ((*pNodes).PerNodeMaster[sec_no] == 0)
          neut_nodes_markasslave (pNodes, sec_no, pri_no, shift);
      }

    // scanning segments || x
    for (z = 1; z <= size[2] + 1; z++)
      for (x = 1; x <= size[0]; x++)
      {
        pri_n1 = NMap[x][1][z];
        pri_n2 = NMap[x + 1][1][z];
        neut_mesh_order1nodes_order2node (Mesh[dim], pri_n1, pri_n2, &pri_no);
        sec_n1 = NMap[x][size[1] + 1][z];
        sec_n2 = NMap[x + 1][size[1] + 1][z];
        neut_mesh_order1nodes_order2node (Mesh[dim], sec_n1, sec_n2, &sec_no);
        if ((*pNodes).PerNodeMaster[sec_no] == 0)
          neut_nodes_markasslave (pNodes, sec_no, pri_no, shift);
      }
  }

  if ((*pNodes).Periodic[2])
  {
    ut_array_1d_int_zero (shift, 3);
    shift[2] = 1;

    // scanning segments || x
    for (y = 1; y <= size[1] + 1; y++)
      for (x = 1; x <= size[0]; x++)
      {
        pri_n1 = NMap[x][y][1];
        pri_n2 = NMap[x + 1][y][1];
        neut_mesh_order1nodes_order2node (Mesh[dim], pri_n1, pri_n2, &pri_no);
        sec_n1 = NMap[x][y][size[2] + 1];
        sec_n2 = NMap[x + 1][y][size[2] + 1];
        neut_mesh_order1nodes_order2node (Mesh[dim], sec_n1, sec_n2, &sec_no);
        if ((*pNodes).PerNodeMaster[sec_no] == 0)
          neut_nodes_markasslave (pNodes, sec_no, pri_no, shift);
      }

    // scanning segments || y
    for (x = 1; x <= size[0] + 1; x++)
      for (y = 1; y <= size[1]; y++)
      {
        pri_n1 = NMap[x][y][1];
        pri_n2 = NMap[x][y + 1][1];
        neut_mesh_order1nodes_order2node (Mesh[dim], pri_n1, pri_n2, &pri_no);
        sec_n1 = NMap[x][y][size[2] + 1];
        sec_n2 = NMap[x][y + 1][size[2] + 1];
        neut_mesh_order1nodes_order2node (Mesh[dim], sec_n1, sec_n2, &sec_no);
        if ((*pNodes).PerNodeMaster[sec_no] == 0)
          neut_nodes_markasslave (pNodes, sec_no, pri_no, shift);
      }
  }

  neut_nodes_fixperslaves (pNodes);

  return;
}

void
nem_order_1_elts (struct MESH *pMesh)
{
  int eltnodeqty_o2, eltnodeqty_o1;

  eltnodeqty_o2 =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, 2);

  if (!strcmp ((*pMesh).EltType, "quad9")
      || !strcmp ((*pMesh).EltType, "quad6"))
    ut_string_string ("quad", &(*pMesh).EltType);

  eltnodeqty_o1 =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, 1);

  // the order-1 nodes come first
  ut_realloc_2d_int_size2 ((*pMesh).EltNodes + 1, (*pMesh).EltQty,
                           eltnodeqty_o2, eltnodeqty_o1);

  (*pMesh).EltOrder = 1;

  return;
}

void
nem_order_1_nodes (struct NODES *pNodes, struct MESH *Mesh, struct NSET *NSet)
{
  int i, j, k, eltnodeqty, node, NodeQty, PerNodeQty;
  int *old_new = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  if ((*pNodes).DupNodeQty > 0)
    ut_print_neperbug ();

  // recording the nodes of the elts
  for (i = 0; i <= 5; i++)
  {
    eltnodeqty =
      neut_elt_nodeqty (Mesh[i].EltType, Mesh[i].Dimension, Mesh[i].EltOrder);

    for (j = 1; j <= Mesh[i].EltQty; j++)
      for (k = 0; k < eltnodeqty; k++)
        old_new[Mesh[i].EltNodes[j][k]] = 1;
  }

  NodeQty = 0;
  for (i = 1; i <= (*pNodes).NodeQty; i++)
    if (old_new[i])
      old_new[i] = ++NodeQty;

  // nodes (the new id of a node is lower than or equal to its old id)
  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    node = old_new[i];
    if (!node)
      continue;

    ut_array_1d_memcpy ((*pNodes).NodeCoo[i], 3, (*pNodes).NodeCoo[node]);

    if ((*pNodes).NodeCl)
      (*pNodes).NodeCl[node] = (*pNodes).NodeCl[i];

    if ((*pNodes).NodePart)
      (*pNodes).NodePart[node] = (*pNodes).NodePart[i];

    if ((*pNodes).PerNodeMaster)
    {
      (*pNodes).PerNodeMaster[node] = ((*pNodes).PerNodeMaster[i] > 0) ?
        old_new[(*pNodes).PerNodeMaster[i]] : 0;
      ut_array_1d_int_memcpy ((*pNodes).PerNodeShift[i], 3,
                              (*pNodes).PerNodeShift[node]);
    }
  }

  (*pNodes).NodeCoo =
    ut_realloc_2d_delline ((*pNodes).NodeCoo, (*pNodes).NodeQty + 1,
                           NodeQty + 1);

  if ((*pNodes).NodeCl)
    (*pNodes).NodeCl = ut_realloc_1d ((*pNodes).NodeCl, NodeQty + 1);

  // periodicity
  if ((*pNodes).PerNodeQty > 0)
  {
    PerNodeQty = 0;
    for (i = 1; i <= (*pNodes).PerNodeQty; i++)
      if (old_new[(*pNodes).PerNodeNb[i]])
        (*pNodes).PerNodeNb[++PerNodeQty] = old_new[(*pNodes).PerNodeNb[i]];
    (*pNodes).PerNodeQty = PerNodeQty;
    (*pNodes).PerNodeNb =
      ut_realloc_1d_int ((*pNodes).PerNodeNb, PerNodeQty + 1);

    (*pNodes).PerNodeMaster =
      ut_realloc_1d_int ((*pNodes).PerNodeMaster, NodeQty + 1);
    (*pNodes).PerNodeShift =
      ut_realloc_2d_int_delline ((*pNodes).PerNodeShift,
                                 (*pNodes).NodeQty + 1, NodeQty + 1);

    if ((*pNodes).PerNodeSlaveQty)
    {
      ut_free_1d_int (&(*pNodes).PerNodeSlaveQty);
      ut_free_2d_int (&(*pNodes).PerNodeSlaveNb, (*pNodes).NodeQty + 1);
      (*pNodes).NodeQty = NodeQty;
      neut_nodes_init_nodeslave (pNodes);
    }
  }

  // partitions
  if ((*pNodes).NodePart)
  {
    (*pNodes).NodePart = ut_realloc_1d_int ((*pNodes).NodePart, NodeQty + 1);
    ut_free_2d_int (&(*pNodes).Parts, (*pNodes).PartQty + 1);
    (*pNodes).NodeQty = NodeQty;
    neut_nodes_init_parts (pNodes);
  }

  (*pNodes).NodeQty = NodeQty;

  // elts
  for (i = 0; i <= 5; i++)
  {
    eltnodeqty =
      neut_elt_nodeqty (Mesh[i].EltType, Mesh[i].Dimension, Mesh[i].EltOrder);

    for (j = 1; j <= Mesh[i].EltQty; j++)
      for (k = 0; k < eltnodeqty; k++)
        Mesh[i].EltNodes[j][k] = old_new[Mesh[i].EltNodes[j][k]];

    if (Mesh[i].NodeElts)
    {
      ut_free_2d_int (&Mesh[i].NodeElts, Mesh[i].NodeQty + 1);
      neut_mesh_init_nodeelts (Mesh + i, NodeQty);
    }
  }

  // nsets (if any)
  if (NSet)
    neut_nset_renumber (NSet, old_new);

  ut_free_1d_int (&old_new);

  return;
}
