/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<float.h>
#include<limits.h>
#include<signal.h>

#include"ut.h"
#include"neut_t.h"
#include"net_utils/net_utils.h"

#include"neper_config.h"

extern double net_tess_opt_comp_minimize_objective_fval_penalty_setdist (struct TOPT *pTOpt, int *polys1, int polyqty1, int *polys2, int polyqty2);
extern double net_tess_opt_comp_minimize_objective_fval_penalty_noncontiguous (struct TOPT *pTOpt, int **setpoly, int *setpolyqty, int setqty);

#ifdef __cplusplus
}
#endif
