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
#include<sys/time.h>

#include"ut.h"
#include"neut_t.h"
#include"net_utils/net_utils.h"

#include"neper_config.h"

#include"net_tess_opt_comp_minimize_objective_fval_tar.h"
#include"net_tess_opt_comp_minimize_objective_fval_tar_celldata/net_tess_opt_comp_minimize_objective_fval_tar_celldata.h"
#include"net_tess_opt_comp_minimize_objective_fval_tar_stat/net_tess_opt_comp_minimize_objective_fval_tar_stat.h"
#include"net_tess_opt_comp_minimize_objective_fval_tar_odf/net_tess_opt_comp_minimize_objective_fval_tar_odf.h"

extern void net_tess_opt_comp_minimize_objective_fval_tar_size (struct TOPT *pTOpt, int var, int cell);
extern void net_tess_opt_comp_minimize_objective_fval_tar_diameq (struct TOPT *pTOpt, int
    var, int cell);
extern void net_tess_opt_comp_minimize_objective_fval_tar_sphericity (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_minimize_objective_fval_tar_convexity (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_minimize_objective_fval_tar_anisofact (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_minimize_objective_fval_tar_centroid (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_minimize_objective_fval_tar_centroidsize (struct TOPT
    *pTOpt, int var, int cell);
extern void net_tess_opt_comp_minimize_objective_fval_tar_ori (struct TOPT *pTOpt, int var, int cell);
extern void net_tess_opt_comp_minimize_objective_fval_tar_tesr (struct TOPT *pTOpt, int var, int cell);

#ifdef __cplusplus
}
#endif
