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
#include<float.h>
#include<limits.h>
#include<signal.h>
#include<sys/time.h>

#include"ut.h"
#include"neut_t.h"
#include"net_utils/net_utils.h"

#include"neper_config.h"

#include"net_tess_opt_comp_objective_fval_gen.h"
#include"net_tess_opt_comp_objective_fval_gen_celldata/net_tess_opt_comp_objective_fval_gen_celldata.h"
#include"net_tess_opt_comp_objective_fval_gen_stat/net_tess_opt_comp_objective_fval_gen_stat.h"
#include"net_tess_opt_comp_objective_fval_gen_odf/net_tess_opt_comp_objective_fval_gen_odf.h"

extern void net_tess_opt_comp_objective_fval_gen_size (struct TOPT *pTOpt, int var, int cell);
extern void net_tess_opt_comp_objective_fval_gen_diameq (struct TOPT *pTOpt, int
    var, int cell);
extern void net_tess_opt_comp_objective_fval_gen_sphericity (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_objective_fval_gen_convexity (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_objective_fval_gen_centroid (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_objective_fval_gen_centroidsize (struct TOPT
    *pTOpt, int var, int cell);
extern void net_tess_opt_comp_objective_fval_gen_ori (struct TOPT *pTOpt, int var, int cell);

#ifdef __cplusplus
}
#endif
