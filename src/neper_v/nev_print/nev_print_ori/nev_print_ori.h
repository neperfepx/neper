/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_ori (struct IN_V In, struct ORI *pOri, char *basename, struct PRINT Print,
               int *steps, int stepqty, int steppos,
               struct SIM Sim, struct TESS Tess, struct TESR Tesr,
               struct DATA *TessData, struct DATA *TesrData,
               struct NODES Nodes, struct MESH *Mesh, struct DATA **MeshData,
               struct DATA CsysData,
               struct POINT *Points, int PointQty, struct DATA *PointData);

extern void nev_print_ori2 (struct IN_V In, struct ORI Ori, char *basename, struct PRINT Print, struct SIM Sim);
