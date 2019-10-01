/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<string.h>
#include<limits.h>
#include<libgen.h>
#include<orilib.h>

#include"neut.h"

#include"neut_tesr_fscanf.h"

extern void neut_tesr_fscanf (FILE *, char *, double *, double *, struct TESR *);
extern void neut_tesr_fscanf_version (FILE *, char *);
extern void neut_tesr_fscanf_head (struct TESR *pTesr, double *bounds,
				   int **pvoxbounds, char **format, FILE * file);
extern void neut_tesr_fscanf_cell (struct TESR *pTesr, FILE * file);
extern void neut_tesr_fscanf_data (struct TESR *pTesr, char *path, int *bounds,
				   double *scale, char *format, FILE * file);
extern void neut_tesr_fscanf_oridata (struct TESR *pTesr, char *path, int *bounds,
                                      double *scale, char *format, FILE * file);
extern void neut_tesr_fscanf_foot (FILE * file);

extern void neut_tesr_fscanf_data_default (struct TESR *pTesr, char *format,
					   FILE * file);
extern void neut_tesr_fscanf_data_scale (struct TESR *pTesr, double *scale,
					 char *format, FILE * file);
extern void neut_tesr_fscanf_data_bounds (struct TESR *pTesr, int *bounds,
					  char *format, FILE * file);

extern void neut_tesr_fscanf_oridata_default (struct TESR *pTesr, char *des, char *format,
					      FILE * file);
