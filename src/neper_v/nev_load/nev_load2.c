/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_load_.h"

void
nev_load_init_data_tess (struct TESS Tess, int dim, struct DATA *pTessData)
{
  (*pTessData).Dim = dim;
  neut_tess_dim_entity (dim, &(*pTessData).Entity);
  neut_tess_entity_qty (Tess, (*pTessData).Entity, &(*pTessData).Qty);

  return;
}

void
nev_load_init_data_tesr (struct TESR Tesr, int id, struct DATA *pTesrData)
{
  if (id == 0)
  {
    ut_string_string ("cell", &(*pTesrData).Entity);
    (*pTesrData).Qty = Tesr.CellQty;
  }
  else if (id == 1)
  {
    ut_string_string ("voxel", &(*pTesrData).Entity);
    (*pTesrData).Qty = ut_array_1d_int_prod (Tesr.size, 3);
  }
  else
    abort ();

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
  (*pPointData).Qty = Point.Qty;

  return;
}

void
nev_load_init_data_mesh (struct MESH Mesh, char *entity, struct DATA *pMeshData)
{
  (*pMeshData).Dim = Mesh.Dimension;

  if (!strncmp (entity, "elt", 3))
    (*pMeshData).Qty = Mesh.EltQty;
  else if (!strncmp (entity, "elset", 5))
    (*pMeshData).Qty = Mesh.ElsetQty;
  else if (!strncmp (entity, "mesh", 4))
    (*pMeshData).Qty = 1;
  else
    abort ();

  return;
}
