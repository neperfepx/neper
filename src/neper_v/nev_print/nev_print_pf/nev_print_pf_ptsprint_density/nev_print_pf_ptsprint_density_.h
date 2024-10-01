/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<float.h>

#include "ut.h"
#include "../../../structIn_v.h"
#include "neut_v.h"
#include "nev_print_pf_ptsprint_density.h"

#include "../../nev_print_utils/nev_print_utils.h"

#define NMARGINPF 0.192
#define SMARGINPF 0.192
#define WMARGINPF 0.265
#define EMARGINPF 0.265

extern void nev_print_pf_ptsprint_density_write (struct PF, struct PRINT, FILE *, char *, struct DATA, double **, double, double);

extern void nev_print_pf_ptsprint_density_grid (struct PF Pf,
                                                double ***pdensity,
                                                double ***pldensity);

extern void nev_print_pf_ptsprint_density_odf (struct PF Pf,
    double **pos, double *wgt, int pqty, double **density,
    double *pmin, double *pmax);

extern void nev_print_pf_ptsprint_density_ldensity (struct PF Pf, double **density,
                                           double **ldensity);

extern void nev_print_pf_ptsprint_density_write_data (struct PF Pf, char *filename, double **ldensity);

extern void nev_print_pf_ptsprint_density_write_text (struct PF Pf, FILE *file, char *filename2);

extern void nev_print_pf_ptsprint_density_write_mask (struct IN_V In, FILE *, struct PF);
