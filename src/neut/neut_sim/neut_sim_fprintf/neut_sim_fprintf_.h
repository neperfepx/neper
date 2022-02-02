/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>
#include<ctype.h>

#include "neut_sim_fprintf.h"

#include"ut.h"
#include"neut.h"

extern void neut_sim_fprintf_entities (FILE *file, struct SIM Sim);

extern void neut_sim_fprintf_results (FILE *fp, char *entity, char **res,
                                      char **resexpr, int resqty);

extern void neut_sim_fprintf_restart (FILE *file, struct SIM Sim);

extern void neut_sim_verbose_results (char *entity, char **res, int resqty);
