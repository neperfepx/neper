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

extern void neut_tess_fscanf_ovm_header (FILE *file);
extern void neut_tess_fscanf_ovm_ver (FILE *file, struct TESS *pTess);
extern void neut_tess_fscanf_ovm_edge (FILE *file, struct TESS *pTess);
extern void neut_tess_fscanf_ovm_face (FILE *file, struct TESS *pTess);
extern void neut_tess_fscanf_ovm_poly (FILE *file, struct TESS *pTess);
