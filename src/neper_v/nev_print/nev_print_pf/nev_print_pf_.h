/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<float.h>

#include"ut.h"
#include"neut_v.h"
#include "nev_print_pf.h"

#include "../nev_print_utils/nev_print_utils.h"
#include "nev_print_pf_ptsprint_density/nev_print_pf_ptsprint_density.h"

#define NMARGINPF 0.192
#define SMARGINPF 0.192
#define WMARGINPF 0.265
#define EMARGINPF 0.265

extern void nev_print_pf_header (FILE *, struct PRINT);
extern void nev_print_pf_background (FILE *, struct PRINT Print);
extern void nev_print_pf_border (FILE *, struct PRINT Print, double ***ppt, int *pqty);

extern void nev_print_pf_pole_proj (FILE *file, struct PRINT Print, char *pole_string);
extern void nev_print_pf_csys (FILE *, struct PRINT Print, struct DATA CsysData);

extern void nev_print_pf_pts (char *type, double **data, double *weights, int qty,
                       int *show, struct PRINT Print, char **modes, int modeqty,
                       double ***ppts,
                       double **pptwgts, int **pptcells, int *pptqty);
extern void nev_print_pf_file_data_compress (struct PRINT Print, double ***ppt,
					 double **pwgt, int *pptqty);

extern void nev_print_pf_ptsprint_symbols (FILE *, struct PRINT, struct DATA, double **borderpt, int borderptqty,
			 double **, int *, int);

extern void nev_print_pf_convert (char *asymptote, char *filename, int
    imagewidth, int imageheight, char *format, int messagetag);

extern void nev_print_pf_file_data_ori (double **q, double *weigths, int qty, int *show,
                            struct PRINT Print,
                            double ***ppts, double **pptwgts, int **pcell,
                            int *pptqty);
extern void nev_print_pf_file_data_vect (double **data, double *weights, int qty, int *show,
                             struct PRINT Print,
                             double ***ppts, double **pptwgts, int **pcell,
                             int *pptqty);

extern void nev_print_pf_file_data_pos (char *filename, double
				    ***ptmp, double **ptmp2, int *pqty);

extern void nev_print_pf_file_data_orthotropic (double ***ptmp, double
					    **pwght, int **pptcells, int *pqty);

extern void nev_print_pf_file_data_clustering (struct PRINT Print, double ***ppts,
                                   double **pptwgts, int **pptcells, int *pptqty);

extern void nev_print_pf_file_data_crysym (struct PRINT Print, double ***ppt,
				       double **pwgt, int *pqty);

extern void nev_print_pf_ptsprint (FILE *file, char *basename, struct PRINT
    Print, struct DATA Data, double **ipfpts, int ipfptqty, double **pts, double *ptwgts,
    int *ptcells, int ptqty, char **modes, int modeqty, int *pdensity_plotted);
