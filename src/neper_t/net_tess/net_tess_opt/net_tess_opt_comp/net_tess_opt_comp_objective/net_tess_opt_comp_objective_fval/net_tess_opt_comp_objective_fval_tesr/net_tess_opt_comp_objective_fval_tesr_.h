/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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

#include"net_tess_opt_comp_objective_fval_tesr.h"

extern void net_tess_opt_comp_objective_fval_tesr_pre (struct TOPT TOpt,
    int **pcellcomped, int *pcellcompedqty);
extern void net_tess_opt_comp_objective_fval_tesr_dist (int *cells, int cellqty, struct TOPT *pTOpt);
extern void net_tess_opt_comp_objective_fval_tesr_cellval (int *cellcomped, int
    cellcompedqty, struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_tesr_obj (struct TOPT *pTOpt, int var);

#ifdef __cplusplus
}
#endif
