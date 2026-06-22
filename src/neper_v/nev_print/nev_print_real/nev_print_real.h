/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_real (struct IN_V In, char *basename, struct PRINT Print, struct SIM Sim,
               struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
               struct DATA *TesrData, struct NODES Nodes, struct MESH *Mesh,
               int SQty, struct NODES *SNodes, struct MESH **SMesh,
               struct DATA *pNodeData, struct DATA **MeshData,
               struct DATA *pCsysData, struct POINT *Points, int PointQty,
               struct DATA *PointData, struct DATA *SNodeData,
               struct DATA ***SMeshData, int **SElt2dElt3d);
