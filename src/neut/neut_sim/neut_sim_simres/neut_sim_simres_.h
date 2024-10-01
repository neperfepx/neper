/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>
#include<ctype.h>

#include "neut_sim_simres.h"

#include"ut.h"
#include"neut.h"

extern void neut_sim_simres_parentres (char *res, struct SIMRES *pSimRes);

extern int neut_sim_simres_type (char *res, struct SIMRES *pSimRes);
