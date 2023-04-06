/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_init_.h"

void
nev_print_init (struct IN_V In, struct PRINT *pPrint, struct PF *pPf,
                struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
                struct DATA *TesrData, struct NODES Nodes, struct MESH *Mesh,
                struct DATA *pNodeData, struct DATA **MeshData, int *pSQty,
                struct NODES **pSNodes, struct MESH **pSMesh2D, struct DATA
                **pSNodeData, struct DATA ****pSMeshData, int ***pSElt2dElt3d,
                struct DATA *pCsysData,
                struct POINT *Points, int PointQty, struct DATA *PointData)
{
  int i;
  char *crysym = NULL;

  if (PointQty)
  {
    (*pPrint).inputqty = PointQty;
    (*pPrint).inputs = ut_alloc_1d_pchar ((*pPrint).inputqty);
    for (i = 0; i < (*pPrint).inputqty; i++)
      ut_string_string (Points[i].Name, (*pPrint).inputs + i);
  }

  if (strcmp (In.imageformat, "vtk"))
  {
    nev_print_init_data (In, *pPrint, Tess, TessData, Tesr, TesrData, Nodes, Mesh,
                         Points, PointQty, pNodeData, MeshData, pCsysData, PointData,
                         &crysym);

    if (In.slice)
      neut_mesh_slice (Nodes, Mesh[3], *pNodeData, MeshData[3][0], In.slice,
                       pSQty, pSNodes, pSMesh2D, pSNodeData, pSMeshData,
                       pSElt2dElt3d);

    for (i = 0; i < *pSQty; i++)
    {
      nev_print_init_data_nodes (In, (*pSNodes)[i], crysym, (*pSNodes)[i].NodeQty,
                                 (*pSNodeData) + i);
      nev_print_init_data_mesh (In, Nodes, (*pSMesh2D)[i], crysym, 1, Mesh[3].ElsetQty,
                                "elt", 3, (*pSMeshData)[i][2]);
    }

    nev_print_init_show (Tess, Tesr, Nodes, Mesh, *pSQty, Points, PointQty, pPrint);

    nev_print_init_camera (In, Tess, Tesr, Nodes, Mesh, Points, PointQty, *pNodeData, pPrint);

    nev_print_init_light (In, Tess, Tesr, Mesh, pPrint);
  }

  else
    nev_print_init_data_nodes (In, Nodes, crysym, Nodes.NodeQty, pNodeData);

  if (!strcmp (In.space, "pf") || !strcmp (In.space, "ipf"))
    nev_print_init_pf (In, Tess, Tesr, Points, PointQty, pPf);

  ut_free_1d_char (&crysym);

  return;
}
