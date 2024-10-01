/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

#include"ut.h"
#include"orilib.h"
#include"neut_v.h"
// #include"../../nefm/nefm.h"
#include "nev_print_png_utils.h"
#include "../../nev_print_utils/nev_print_utils.h"
#include "../nev_print_png_header/nev_print_png_header.h"
#include "../nev_print_png_foot/nev_print_png_foot.h"

extern void nev_print_png_triangle (FILE *, double *, double *, double *,
				char *, char *, char *);
extern void nev_print_png_triangle_edge (FILE *, double *, double *, double *,
				     char *, char *);
extern void nev_print_png_segment (FILE *, double *, double *, double, char *);
extern void nev_print_png_tet_edge (FILE *, double *, double *, double *,
				double *, char *, char *);
extern void nev_print_png_tet (FILE *, double *, double *, double *, double *,
			   char *, char *, char *);

extern int nev_print_png_cylinder_test (double *coo1, double *coo2, double rad);
