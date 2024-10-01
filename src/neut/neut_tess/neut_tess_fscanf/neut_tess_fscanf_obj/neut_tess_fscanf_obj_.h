/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<string.h>
#include<orilib.h>

#include"neut.h"

extern void neut_tess_fscanf_obj_pre (struct TESS *pTess);
extern void neut_tess_fscanf_obj_scangroup (char *line, char ***pgroups, int *pgroupqty, struct TESS *pTess);
extern void neut_tess_fscanf_obj_scanver (char *line, struct TESS *pTess);
extern void neut_tess_fscanf_obj_scanface (char *line, int ***pSFaceVerNb, int *pSFaceVerQty, struct TESS *pTess);
