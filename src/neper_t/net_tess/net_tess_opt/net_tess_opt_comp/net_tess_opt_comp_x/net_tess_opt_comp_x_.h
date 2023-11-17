/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_t.h"

#include"net_tess_opt_comp_x.h"

extern void net_tess_opt_comp_x_morpho (struct TOPT *pTOpt, double **px);
extern void net_tess_opt_comp_x_ori (struct TOPT *pTOpt, double **px);
extern void net_tess_opt_comp_x_crystal (struct TOPT *pTOpt, double **px);
