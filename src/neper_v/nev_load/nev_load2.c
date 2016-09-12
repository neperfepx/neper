/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_load_.h"

void
nev_load_init_tessdata (struct TESS Tess, struct TESSDATA *pTessData)
{
  (*pTessData).VerQty = Tess.VerQty;
  (*pTessData).EdgeQty = Tess.EdgeQty;
  (*pTessData).FaceQty = Tess.FaceQty;
  (*pTessData).PolyQty = Tess.PolyQty;
  (*pTessData).SeedQty = Tess.CellQty;

  return;
}

void
nev_load_init_tesrdata (struct TESR Tesr, struct TESRDATA *pTesrData)
{
  (*pTesrData).Qty = ut_array_1d_int_prod (Tesr.size, 3);

  return;
}

void
nev_load_init_nodedata (struct NODES Nodes, struct NODEDATA *pNodeData)
{
  (*pNodeData).NodeQty = Nodes.NodeQty;

  return;
}

void
nev_load_init_pointdata (struct POINT Point, struct POINTDATA *pPointData)
{
  (*pPointData).PointQty = Point.PointQty;

  return;
}

void
nev_load_init_meshdata (struct MESH Mesh, struct MESHDATA *pMeshData)
{
  (*pMeshData).EltQty = Mesh.EltQty;
  (*pMeshData).Dimension = Mesh.Dimension;

  return;
}
