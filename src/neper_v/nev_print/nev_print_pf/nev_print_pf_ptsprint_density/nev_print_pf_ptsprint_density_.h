/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<float.h>

#include "ut.h"
#include "neut_v.h"
#include "nev_print_pf_ptsprint_density.h"

#include "../../nev_print_utils/nev_print_utils.h"

#define NMARGINPF 0.192
#define SMARGINPF 0.192
#define WMARGINPF 0.265
#define EMARGINPF 0.265

extern void nev_print_pf_ptsprint_density_write (FILE *, char *, struct PRINT, struct DATA, double ***, double **, double, double);

extern void nev_print_pf_ptsprint_density_write_text0 (FILE *file, struct PRINT Print);

extern void nev_print_pf_ptsprint_density_grid (struct PRINT Print,
                                       double ****pgrid, double ***pdensity,
				       double ****pPgrid, double ****plgrid, double ***pldensity);

extern void nev_print_pf_ptsprint_density_odf (struct PRINT Print, double ***grid, double ***Pgrid,
    double **coo, int borderptqty, double **pos, double *wgt, int pqty, double **density,
    double *pmin, double *pmax);

extern void nev_print_pf_ptsprint_density_ldensity (struct PRINT Print, double **density, double ***lgrid,
                                           double **ldensity);

extern void nev_print_pf_ptsprint_density_write_data (char *filename, struct PRINT Print, double ***lgrid, double **ldensity);

extern void nev_print_pf_ptsprint_density_write_text (FILE *file, char *filename2, struct PRINT Print);

extern void nev_print_pf_ptsprint_density_write_mask (FILE *, struct PRINT, double **borderpt,
			   int borderptqty);
