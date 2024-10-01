/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<orilib.h>

#include"neut.h"

#include"neut_tesr_fprintf.h"

extern void neut_tesr_fprintf_head (FILE *, struct TESR);
extern void neut_tesr_fprintf_data (FILE *, char *, struct TESR);
extern void neut_tesr_fprintf_cell (FILE *, struct TESR);
extern void neut_tesr_fprintf_data_noheader (FILE * file, char *format,
				   char *wildcard, struct TESR Tesr, int *CellId);
extern void neut_tesr_fprintf_data_raw (FILE * file, char *rawname,
					char *format, char *wildcard, struct TESR Tesr);
extern void neut_tesr_fprintf_oridata (FILE *, char *, struct TESR);
extern void neut_tesr_fprintf_oridata_noheader (FILE * file, char
    *format, struct TESR Tesr);
extern void neut_tesr_fprintf_oridata_raw (FILE * file, char *rawname,
					   char *format, struct TESR Tesr);
extern void neut_tesr_fprintf_foot (FILE *file);

extern void neut_tesr_fprintf_oridef (FILE * file, char *format, struct TESR Tesr);

extern void neut_tesr_fprintf_oridef_noheader (FILE * file, char *format,
                                               char *wildcard, struct TESR Tesr);
