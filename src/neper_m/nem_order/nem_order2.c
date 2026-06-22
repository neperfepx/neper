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

void
nem_order_periodic (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh)
{
  nem_order_periodic_edges (Tess, pNodes, Mesh);

  if (Tess.Dim == 3)
    nem_order_periodic_faces (Tess, pNodes, Mesh);

  neut_nodes_init_nodeslave (pNodes);

  return;
}

void
nem_order_post (struct IN_M In, int NodeQty_before, struct NODES *pNodes)
{
  int i;

  if (strcmp (In.interface, "continuous"))
  {
    (*pNodes).DupNodeSlaveNb =
      ut_realloc_1d_pint ((*pNodes).DupNodeSlaveNb, (*pNodes).NodeQty + 1);
    for (i = NodeQty_before; i <= (*pNodes).NodeQty; i++)
      (*pNodes).DupNodeSlaveNb[i] = NULL;
  }

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
