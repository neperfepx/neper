/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_real_png (struct IN_V In, char *basename, struct PRINT Print, struct SIM Sim,
                           struct TESS Tess,
                           struct DATA *TessData, struct TESR Tesr,
                           struct DATA *TesrData, struct NODES Nodes,
                           struct MESH *Mesh, int SQty, struct NODES *SNodes,
                           struct MESH **SMesh, struct DATA *pNodeData,
                           struct DATA **MeshData, struct DATA *pCsysData,
                           struct POINT *Points, int PointQty, struct DATA *pPointData,
                           struct DATA *SNodeData, struct DATA ***SMeshData,
                           int **SElt2dElt3d);

extern void nev_print_real_png_convert (char *, char *, int, int, int, int);

extern void nev_print_real_png_meshslices (FILE *file, struct PRINT Print, struct DATA **MeshData,
                                           int SQty, struct NODES *SNodes,
                                           struct MESH **SMesh, struct DATA *SNodeData,
                                           struct DATA ***SMeshData, int **SElt2dElt3d);

#include "nev_print_real_png_csys/nev_print_real_png_csys.h"
#include "nev_print_real_png_mesh/nev_print_real_png_mesh.h"
#include "nev_print_real_png_tess/nev_print_real_png_tess.h"
#include "nev_print_real_png_tesr/nev_print_real_png_tesr.h"
#include "nev_print_real_png_utils/nev_print_real_png_utils.h"
