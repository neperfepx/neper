/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_v.h"

#include "nev_print_data_tess.h"

extern void nev_print_data_tess_init (struct TESS Tess, struct DATA *TessData,
                                      char *entity, char *prop, char *argument,
			              int *pid, int *pentityqty,
			              char **ptype, char **pvalue, struct DATA **pData);
