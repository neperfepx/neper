/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<float.h>

#include"ut.h"
#include"../../structIn_v.h"
#include"neut_v.h"
#include "nev_print_pf.h"

#include "../nev_print_utils/nev_print_utils.h"
#include "nev_print_pf_ptsprint_density/nev_print_pf_ptsprint_density.h"

#define NMARGINPF 0.192
#define SMARGINPF 0.192
#define WMARGINPF 0.265
#define EMARGINPF 0.265

extern void nev_print_pf_header (struct PF Pf, FILE *);
extern void nev_print_pf_background (struct IN_V In, struct PF Pf, FILE *, struct PRINT Print);
extern void nev_print_pf_border (FILE *, struct PF *pPf);
extern void nev_print_pf_inputs (struct PRINT Print, struct POINT *Points,
                                 int PointQty, struct PF *pPf);

extern void nev_print_pf_pole_proj (struct PF Pf, FILE *file, char *pole_string);
extern void nev_print_pf_csys (struct PF Pf, FILE *, struct DATA CsysData);

extern void nev_print_pf_pts (int input, struct PF *pPf,
                       char *type, double **data, double *weights, int qty,
                       int *show, int clustering,
                       double ***ppts,
                       double **pptwgts, int **pptcells, int *pptqty,
                       double ***cellpts, int *cellptqty);

extern void nev_print_pf_file_data_compress (struct PRINT Print, double ***ppt,
					 double **pwgt, int *pptqty);

extern void nev_print_pf_ptsprint_symbols (struct IN_V In, int input, struct PF *pPf,
                               FILE *file, struct DATA Data, double **pts,
                               int *ptcells, int ptqty);

extern void nev_print_pf_convert (char *asymptote, char *filename, int
    imagewidth, int imageheight, char *format, int messagetag);

extern void nev_print_pf_file_data_ori (int input, struct PF *pPf, double **q, double *weigths, int qty, int *show,
                            double ***ppts, double **pptwgts, int **pcell,
                            int *pptqty);
extern void nev_print_pf_file_data_vect (int input, struct PF *pPf, double **data, double *weights, int qty, int *show,
                             double ***ppts, double **pptwgts, int **pcell,
                             int *pptqty);

extern void nev_print_pf_file_data_pos (char *filename, double
				    ***ptmp, double **ptmp2, int *pqty);

extern void nev_print_pf_file_data_orthotropic (double ***ptmp, double
					    **pwght, int **pptcells, int *pqty);

extern void nev_print_pf_file_data_clustering (struct PF Pf, double ***ppts,
                                   double **pptwgts, int **pptcells, int *pptqty);

extern void nev_print_pf_file_data_crysym (struct PF Pf, double ***ppt,
				       double **pwgt, int **pptcells, int *pqty);

extern void nev_print_pf_ptsprint (struct IN_V In, int input, struct PF *pPf, FILE *file,
                       char *basename, struct PRINT Print, struct DATA Data,
                       double **pts, double *ptwgts, int *ptcells, int ptqty,
                       int *pdensity_plotted);

extern void nev_print_pf_polestring (struct PF Pf,
                                     struct DATA *pData, char **ppole_string);

extern void nev_print_pf_pre_tess (struct PRINT Print, struct TESS Tess, struct DATA *TessData,
                       struct DATAINPUT *pDataInput, struct DATA **ppData,
                       int *pcellqty, double **pwgts, double ***pdata,
                       char **ptype, int **pshow);

extern void nev_print_pf_pre_tesr (struct PRINT Print,
                       struct TESR Tesr, struct DATA *TesrData,
                       struct DATAINPUT *pDataInput, struct DATA **ppData,
                       int *pcellqty, double **pwgts, double ***pdata,
                       char **ptype, int **pshow);

extern void nev_print_pf_pre_mesh (struct PRINT Print,
                                   struct MESH *Mesh, struct DATA **MeshData,
                                   struct DATAINPUT *pDataInput,
                                   struct DATA **ppData, int *pcellqty,
                                   double **pwgts, double ***pdata,
                                   char **ptype, int **pshow);

extern void nev_print_pf_pre_points (struct PRINT Print,
                         struct POINT Points, struct DATA *PointData, int id,
                         struct DATAINPUT *pDataInput, struct DATA **ppData,
                         int *pcellqty, double **pwgts, double ***pdata,
                         char **ptype, int **pshow);

extern void nev_print_pf_pre_clustering (struct PF Pf, int cellqty, int *pclustering);

extern void nev_print_pf_file_data_ori_preipf (int input,
                                   struct OL_SET Set, double *uvect,
                                   struct PF *pPf);

extern void nev_print_pf_file_data_vect_preipf (int input,
                                    double **data, int qty, struct PF *pPf);
