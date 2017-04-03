/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<orilib.h>

#include"neut.h"

#include"neut_tesr_fprintf.h"

extern void neut_tesr_fprintf_head (FILE *, char *, struct TESR);
extern void neut_tesr_fprintf_data (FILE *, char *, struct TESR);
extern void neut_tesr_fprintf_cell (FILE *, struct TESR);
extern void neut_tesr_fprintf_raw (FILE * file, char *format,
				   struct TESR Tesr);
extern void neut_tesr_fprintf_data_raw (FILE * file, char *rawname,
					char *format, struct TESR Tesr);
extern void neut_tesr_fprintf_foot (FILE *);
