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
#include"orilib.h"
#include"neut_t.h"
#include"net_utils/net_utils.h"

#include"neper_config.h"

#include"net_tess_opt_comp_objective_fval_gen_odf.h"

extern void net_tess_opt_comp_objective_fval_gen_odf_smoothed (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_odf_smoothed_update (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_odf_unsmoothed_comp (struct TOPT *pTOpt, int var);
extern void net_tess_opt_comp_objective_fval_gen_odf_smoothed_update_cell (struct TOPT *pTOpt, int var, int cell);

extern void net_tess_opt_comp_objective_fval_gen_odf_evaluate (struct TOPT *pTOpt, struct ODF Odf, int var);

#ifdef __cplusplus
}
#endif
