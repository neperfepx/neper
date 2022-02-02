/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<signal.h>

#include"ut.h"
#include"neut_t.h"
#include"net_utils/net_utils.h"

#include"neper_config.h"

#include"net_tess_opt_comp_objective_fval_gen_celldata.h"

extern void net_tess_opt_comp_objective_fval_gen_celldata_scalar (struct TOPT *pTOpt, int id);
extern void net_tess_opt_comp_objective_fval_gen_celldata_centroid (struct TOPT *pTOpt, int id);
extern void net_tess_opt_comp_objective_fval_gen_celldata_centroidtol (struct TOPT *pTOpt, int id);
extern void net_tess_opt_comp_objective_fval_gen_celldata_centroiddiameq (struct TOPT *pTOpt, int id);

#ifdef __cplusplus
}
#endif
