/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_load_.h"

void
nev_load_init_data_tess (struct TESS Tess, int dim, struct DATA *pTessData)
{
  (*pTessData).Dim = dim;
  neut_tess_dim_entity (dim, &(*pTessData).type);
  neut_tess_entity_qty (Tess, (*pTessData).type, &(*pTessData).Qty);

  return;
}

void
nev_load_init_data_tesr (struct TESR Tesr, struct DATA *pTesrData)
{
  (*pTesrData).Qty = ut_array_1d_int_prod (Tesr.size, 3);

  return;
}

void
nev_load_init_data_node (struct NODES Nodes, struct DATA *pData)
{
  (*pData).Qty = Nodes.NodeQty;

  return;
}

void
nev_load_init_data_point (struct POINT Point, struct DATA *pPointData)
{
  (*pPointData).Qty = Point.PointQty;

  return;
}

void
nev_load_init_data_mesh (struct MESH Mesh, struct DATA *pMeshData)
{
  (*pMeshData).Qty = Mesh.EltQty;
  (*pMeshData).Dim = Mesh.Dimension;

  return;
}
