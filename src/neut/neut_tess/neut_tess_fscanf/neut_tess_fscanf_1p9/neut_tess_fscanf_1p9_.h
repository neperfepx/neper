/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

extern void neut_tess_fscanf_head_1p9 (struct TESS *, FILE *);
extern void neut_tess_fscanf_foot_1p9 (FILE *);
extern void neut_tess_fscanf_ver_1p9 (struct TESS *, FILE *);
extern void neut_tess_fscanf_edge_1p9 (struct TESS *, FILE *);
extern void neut_tess_fscanf_face_1p9 (struct TESS *, FILE *);
extern void neut_tess_fscanf_poly_1p9 (struct TESS *, FILE *);
