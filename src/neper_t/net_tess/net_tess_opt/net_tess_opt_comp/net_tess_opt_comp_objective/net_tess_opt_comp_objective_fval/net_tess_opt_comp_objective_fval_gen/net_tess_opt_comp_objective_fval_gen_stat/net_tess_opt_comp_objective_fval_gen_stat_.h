/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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
#include"orilib.h"
#include"neut_t.h"
#include"net_utils/net_utils.h"

#include"neper_config.h"

#include"net_tess_opt_comp_objective_fval_gen_stat.h"

extern void net_tess_opt_comp_objective_fval_gen_stat_alloc (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_smoothed_comp (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_smoothed_update (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_smoothed_gen_legacy (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_smoothed_update_legacy (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_unsmoothed_comp (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_smoothed_update_cell (struct TOPT *pTOpt, int var, int cell);

extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate (struct TOPT
						  *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate_chi2 (struct TOPT
						       *pTOpt,
						       int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate_ks (struct TOPT
						     *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate_kuiper (struct TOPT
							 *pTOpt,
							 int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate_cvm (struct TOPT
						      *pTOpt,
						      int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate_ad (struct TOPT
						     *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate_FL2 (struct TOPT
						     *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate_FL2w (struct TOPT
						     *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_stat_evaluate_FL2wu (struct TOPT
						     *pTOpt, int var);

#ifdef __cplusplus
}
#endif
