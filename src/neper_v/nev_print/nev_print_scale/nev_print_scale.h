/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_scale (struct IN_V In, char *basename, struct PRINT Print, struct SIM Sim,
                 struct DATA *TessData, struct DATA *pTesrData, struct DATA *pNodeData,
                 struct DATA **MeshData, int PointQty, struct DATA *PointData);
extern void nev_print_png_scale (struct IN_V In, FILE * file, struct PRINT Print, struct DATA Data);
