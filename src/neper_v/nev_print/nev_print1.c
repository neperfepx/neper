/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_.h"

void
nev_print (char **expargv, int *pi, struct PRINT *pPrint, struct SIM Sim,
           struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
           struct DATA *pTesrData, struct NODES Nodes, struct MESH *Mesh,
           struct DATA *pNodeData, struct DATA *MeshData,
           struct DATA *pCsysData, struct POINT *Points, int PointQty,
           struct DATA *PointData)
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
                  &SNodeData, &SMeshData, &SElt2dElt3d, pCsysData, Points,
                  PointQty, PointData);

  if (!strcmp ((*pPrint).space, "real") || !strcmp ((*pPrint).space, "rodrigues"))
  {
    if (ut_list_testelt ((*pPrint).imageformat, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt ((*pPrint).imageformat, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt ((*pPrint).imageformat, NEUT_SEP_NODEP, "png"))
      nev_print_png (basename, *pPrint, Sim, Tess, TessData, Tesr, pTesrData, Nodes,
                     Mesh, SQty, SNodes, SMesh2D, pNodeData, MeshData, pCsysData,
                     Points, PointQty, PointData, SNodeData, SMeshData, SElt2dElt3d);

    if (ut_list_testelt ((*pPrint).imageformat, NEUT_SEP_NODEP, "vtk"))
      nev_print_vtk (basename, *pPrint, Sim, Nodes, Mesh, pNodeData, MeshData);
  }

  else if (!strcmp ((*pPrint).space, "pf"))
    nev_print_pf (basename, *pPrint, Sim, Tess, Tesr, TessData, pTesrData,
                  *pCsysData, Points, PointQty, PointData);

  else if (!strcmp ((*pPrint).space, "tree"))
    nev_print_tree (basename, *pPrint, Sim);

  if ((*pPrint).showscale)
    nev_print_scale (basename, *pPrint, Sim, TessData, pTesrData, pNodeData,
                     MeshData, PointQty, PointData);

  ut_free_1d_char (&basename);

  return;
}
