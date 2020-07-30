/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_png (char *basename, struct PRINT *pPrint, struct SIM Sim,
                           struct TESS Tess,
                           struct DATA *TessData, struct TESR Tesr,
                           struct DATA *pTesrData, struct NODES Nodes,
                           struct MESH *Mesh, int SQty, struct NODES *SNodes,
                           struct MESH *SMesh2D, struct DATA *pNodeData,
                           struct DATA *MeshData, struct DATA *pCsysData,
                           struct POINT Point, struct DATA *pPointData,
                           struct DATA *SNodeData, struct DATA **SMeshData,
                           int **SElt2dElt3d);

#include "nev_print_png_foot/nev_print_png_foot.h"
#include "nev_print_png_header/nev_print_png_header.h"
#include "nev_print_png_csys/nev_print_png_csys.h"
#include "nev_print_png_mesh/nev_print_png_mesh.h"
#include "nev_print_png_tess/nev_print_png_tess.h"
#include "nev_print_png_tesr/nev_print_png_tesr.h"
#include "nev_print_png_utils/nev_print_png_utils.h"
