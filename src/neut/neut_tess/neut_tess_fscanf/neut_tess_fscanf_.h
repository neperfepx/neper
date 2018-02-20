/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<orilib.h>

#include"neut.h"

extern void neut_tess_fscanf_head (struct TESS *,FILE *);
extern void neut_tess_fscanf_version (FILE *, char *);
extern void neut_tess_fscanf_foot (FILE *);
extern void neut_tess_fscanf_cell (struct TESS *, FILE *);
extern void neut_tess_fscanf_ver (struct TESS *, FILE *);
extern void neut_tess_fscanf_edge (struct TESS *, FILE *);
extern void neut_tess_fscanf_face (struct TESS *, FILE *);
extern void neut_tess_fscanf_poly (struct TESS *, FILE *);
extern void neut_tess_fscanf_domain (struct TESS *, FILE *);
extern void neut_tess_fscanf_per (struct TESS *, FILE *);
extern void neut_tess_fscanf_per_gen (struct TESS *, FILE *);
extern void neut_tess_fscanf_per_seed (struct TESS *, FILE *);
extern void neut_tess_fscanf_per_face (struct TESS *, FILE *);
extern void neut_tess_fscanf_per_edge (struct TESS *, FILE *);
extern void neut_tess_fscanf_per_ver (struct TESS *, FILE *);
extern void neut_tess_fscanf_scale (struct TESS *, FILE *);
extern void neut_tess_fscanf_scale_gen (struct TESS *, FILE *);
extern void neut_tess_fscanf_scale_cellid (struct TESS *, FILE *);
