/* This fil40e is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_.h"

void
nev_print_real (struct IN_V In, char *basename, struct PRINT Print, struct SIM Sim,
               struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
               struct DATA *TesrData, struct NODES Nodes, struct MESH *Mesh,
               int SQty, struct NODES *SNodes, struct MESH **SMesh,
               struct DATA *pNodeData, struct DATA **MeshData,
               struct DATA *pCsysData, struct POINT *Points, int PointQty,
               struct DATA *PointData, struct DATA *SNodeData,
               struct DATA ***SMeshData, int **SElt2dElt3d)
{
  if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
   || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
   || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
    nev_print_real_png (In, basename, Print, Sim, Tess, TessData, Tesr, TesrData, Nodes,
                   Mesh, SQty, SNodes, SMesh, pNodeData, MeshData, pCsysData,
                   Points, PointQty, PointData, SNodeData, SMeshData, SElt2dElt3d);

  if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "vtk"))
    nev_print_real_vtk (In, basename, Sim, Nodes, Mesh, pNodeData, MeshData);

  return;
}
