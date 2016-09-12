/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
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

#include"net_tess_opt_comp_objective_fval_cellval.h"

extern void net_tess_opt_comp_objective_fval_cellval_size (struct TOPT *pTOpt, int var, int cell);
extern void net_tess_opt_comp_objective_fval_cellval_diameq (struct TOPT *pTOpt, int
    var, int cell);
extern void net_tess_opt_comp_objective_fval_cellval_sphericity (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_objective_fval_cellval_convexity (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_objective_fval_cellval_centroid (struct TOPT *pTOpt,
    int var, int cell);
extern void net_tess_opt_comp_objective_fval_cellval_centroidsize (struct TOPT
    *pTOpt, int var, int cell);
extern int net_tess_opt_comp_objective_fval_cellval_tesr (struct TOPT *pTOpt, int
    var, int cell);

#ifdef __cplusplus
}
#endif
