/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<float.h>

#include"ut.h"
#include"neut_v.h"
#include "nev_print_ori.h"

#include "../nev_print_utils/nev_print_utils.h"
#include "../nev_print_real/nev_print_real_png/nev_print_real_png_tess/nev_print_real_png_tess.h"
#include "../nev_print_real/nev_print_real_png/nev_print_real_png_csys/nev_print_real_png_csys.h"
#include "../nev_print_real/nev_print_real_png/nev_print_real_png_point/nev_print_real_png_point.h"
#include "nev_print_ori_ptsprint_density/nev_print_ori_ptsprint_density.h"

extern void nev_print_ori_space (struct IN_V In, struct ORI Ori, struct PRINT Print, FILE *file);

extern void nev_print_ori_header (struct ORI Ori, FILE *);
extern void nev_print_ori_inputs (struct PRINT Print, struct POINT *Points,
                                 int PointQty, struct ORI *pOri);

extern void nev_print_ori_csys (struct PRINT Print, struct ORI Ori, FILE *, struct DATA CsysData);

extern void nev_print_ori_pts (int input, struct ORI *pOri,
                       char *type, double **data, double *weights, int qty,
                       int *show, int clustering,
                       double ***ppts,
                       double **pptwgts, int **pptcells, int *pptqty,
                       double ***cellpts, int *cellptqty);

extern void nev_print_ori_file_data_compress (struct PRINT Print, double ***ppt,
					 double **pwgt, int *pptqty);

extern void nev_print_ori_ptsprint_symbols (struct IN_V In, struct PRINT Print, int input, struct ORI *pOri,
                               FILE *file, struct DATA Data, double **pts,
                               int *ptcells, int ptqty);

extern void nev_print_ori_convert (char *asymptote, char *filename, int
    imagewidth, int imageheight, char *format, int messagetag);

extern void nev_print_ori_file_data_ori (int input, struct ORI *pOri, double **q, double *weigths, int qty, int *show,
                            double ***ppts, double **pptwgts, int **pcell,
                            int *pptqty);

extern void nev_print_ori_file_data_pos (char *filename, double
				    ***ptmp, double **ptmp2, int *pqty);

extern void nev_print_ori_ptsprint (struct IN_V In, char *layout, int input, struct ORI *pOri, char *filename,
                       FILE *file, struct PRINT Print, struct DATA Data,
                       double **pts, double *ptwgts, int *ptcells, int ptqty, struct OL_SET *pOSet,
                       int *pdensity_plotted);

extern void nev_print_ori_pre_tess (struct PRINT Print, struct TESS Tess, struct DATA *TessData,
                       struct DATAINPUT *pDataInput, struct DATA **ppData,
                       int *pcellqty, double **pwgts, double ***pdata,
                       char **ptype, struct OL_SET *pOSet, int **pshow);

extern void nev_print_ori_pre_tesr (struct PRINT Print,
                       struct TESR Tesr, struct DATA *TesrData,
                       struct DATAINPUT *pDataInput, struct DATA **ppData,
                       int *pcellqty, double **pwgts, double ***pdata,
                       char **ptype, struct OL_SET *pOSet, int **pshow);

extern void nev_print_ori_pre_mesh (struct PRINT Print, struct NODES Nodes,
                                   struct MESH *Mesh, struct DATA **MeshData,
                                   struct DATAINPUT *pDataInput,
                                   struct DATA **ppData, int *pcellqty,
                                   double **pwgts, double ***pdata,
                                   char **ptype, struct OL_SET *pOSet, int **pshow);

extern void nev_print_ori_pre_points (struct PRINT Print,
                         struct POINT Points, struct DATA *PointData, int id,
                         struct DATAINPUT *pDataInput, struct DATA **ppData,
                         int *pcellqty, double **pwgts, double ***pdata,
                         char **ptype, struct OL_SET *pOSet, int **pshow);
