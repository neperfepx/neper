/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"orilib.h"
#include"neut_v.h"
// #include"../../nefm/nefm.h"
#include "nev_print_utils.h"
#include "../nev_print_header/nev_print_header.h"
#include "../nev_print_foot/nev_print_foot.h"

extern void nev_print_triangle (FILE *, double *, double *, double *,
				char *, char *, char *);
extern void nev_print_triangle_edge (FILE *, double *, double *, double *,
				     char *, char *);
extern void nev_print_segment (FILE *, double *, double *, char *, char *);
extern void nev_print_tet_edge (FILE *, double *, double *, double *,
				double *, char *, char *);
extern void nev_print_tet (FILE *, double *, double *, double *, double *,
			   char *, char *, char *);

extern int nev_print_cylinder_test (double *coo1, double *coo2, char
    *edgerad);
