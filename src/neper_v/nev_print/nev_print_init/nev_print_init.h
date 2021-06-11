/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_init (struct PRINT *pPrint, struct TESS Tess, struct DATA
    *TessData, struct TESR Tesr, struct DATA *pTesrData, struct NODES Nodes,
    struct MESH *Mesh, struct DATA *pNodeData, struct DATA *MeshData, int
    *pSQty, struct NODES **pSNodes, struct MESH **pSMesh2D, struct DATA
    **pSNodeData, struct DATA ***pSMeshData, int ***pSElt2dElt3d, struct DATA
    *pCsysData, struct POINT Point, struct DATA *pPointData);
