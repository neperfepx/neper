/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_.h"

void
nev_print (char **expargv, int *pi, struct PRINT *pPrint, struct SIM Sim,
           struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
           struct DATA *pTesrData, struct NODES Nodes, struct MESH *Mesh,
           struct DATA *pNodeData, struct DATA *MeshData,
           struct DATA *pCsysData, struct POINT Point,
           struct DATA *pPointData)
{
  char *basename = NULL;
  int SQty = 0;
  struct NODES *SNodes = NULL;
  struct MESH *SMesh2D = NULL;
  struct DATA *SNodeData = NULL;
  struct DATA **SMeshData = NULL;
  int **SElt2dElt3d = NULL;

  ut_string_string (expargv[++(*pi)], &basename);

  nev_print_init (pPrint, Tess, TessData, Tesr, pTesrData, Nodes, Mesh,
                  pNodeData, MeshData, &SQty, &SNodes, &SMesh2D,
                  &SNodeData, &SMeshData, &SElt2dElt3d, pCsysData, Point,
                  pPointData);

  if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov")
   || ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov:objects")
   || ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "png"))
    nev_print_png (basename, pPrint, Sim, Tess, TessData, Tesr, pTesrData, Nodes,
                   Mesh, SQty, SNodes, SMesh2D, pNodeData, MeshData, pCsysData,
                   Point, pPointData, SNodeData, SMeshData, SElt2dElt3d);

  if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "vtk"))
    nev_print_vtk (basename, pPrint, Sim, Nodes, Mesh, pNodeData, MeshData);

  ut_free_1d_char (&basename);

  return;
}
