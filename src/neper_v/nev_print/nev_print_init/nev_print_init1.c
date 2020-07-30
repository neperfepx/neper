/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_init_.h"

void
nev_print_init (struct PRINT *pPrint, struct TESS Tess,
                struct DATA *TessData, struct TESR Tesr,
                struct DATA *pTesrData, struct NODES Nodes, struct MESH *Mesh,
                struct DATA *pNodeData, struct DATA *MeshData, int *pSQty,
                struct NODES **pSNodes, struct MESH **pSMesh2D, struct DATA
                **pSNodeData, struct DATA ***pSMeshData, int ***pSElt2dElt3d,
                struct DATA *pCsysData, struct POINT Point,
                struct DATA *pPointData)
{
  int i;

  if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov")
   || ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov:objects")
   || ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "png"))
  {
    nev_print_init_data (*pPrint, Tess, TessData, Tesr, pTesrData, Nodes, Mesh,
                        Point, pNodeData, MeshData, pCsysData, pPointData);

    if ((*pPrint).slice)
      neut_mesh_slice (Nodes, Mesh[3], *pNodeData, MeshData, (*pPrint).slice,
                      pSQty, pSNodes, pSMesh2D, pSNodeData, pSMeshData,
                      pSElt2dElt3d);

    for (i = 0; i < *pSQty; i++)
    {
      nev_print_init_data_nodes ((*pSNodes)[i], (*pSNodes)[i].NodeQty, (*pSNodeData) + i);
      nev_print_init_data_mesh ((*pSMesh2D)[i], 1, Mesh[3].ElsetQty,
                               (*pSMeshData)[i] + 2);
    }

    nev_print_init_show (Tess, Tesr, Nodes, Mesh, *pSQty, Point, pPrint);

    nev_print_init_camera (Tess, Tesr, Nodes, Mesh, Point, *pNodeData, pPrint);
  }

  else if (ut_list_testelt ((*pPrint).format, NEUT_SEP_DEP, "vtk"))
    nev_print_init_data_nodes (Nodes, Nodes.NodeQty, pNodeData);

  return;
}
