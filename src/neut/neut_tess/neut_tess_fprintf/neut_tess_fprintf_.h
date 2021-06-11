/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<orilib.h>

#include"neut.h"

extern void neut_tess_fprintf_head (struct TESS, FILE *);
extern void neut_tess_fprintf_foot (FILE *);
extern void neut_tess_fprintf_cell (struct TESS, FILE *);
extern void neut_tess_fprintf_ver (struct TESS, FILE *);
extern void neut_tess_fprintf_edge (struct TESS, FILE *);
extern void neut_tess_fprintf_face (struct TESS, FILE *);
extern void neut_tess_fprintf_poly (struct TESS, FILE *);
extern void neut_tess_fprintf_dom (struct TESS, FILE *);
extern void neut_tess_fprintf_per (struct TESS, FILE *);
extern void neut_tess_fprintf_per_gen (struct TESS, FILE *);
extern void neut_tess_fprintf_per_seed (struct TESS, FILE *);
extern void neut_tess_fprintf_per_ver (struct TESS, FILE *);
extern void neut_tess_fprintf_per_edge (struct TESS, FILE *);
extern void neut_tess_fprintf_per_face (struct TESS, FILE *);
extern void neut_tess_fprintf_scale (struct TESS, FILE *);
extern void neut_tess_fprintf_scale_gen (struct TESS, FILE *);
extern void neut_tess_fprintf_scale_cellid (struct TESS, FILE *);
