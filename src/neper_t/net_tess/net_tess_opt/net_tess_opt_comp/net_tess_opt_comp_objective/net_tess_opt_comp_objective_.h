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
#include<sys/time.h>

#include"ut.h"
#include"neut_t.h"
#include"net_utils/net_utils.h"

#include"neper_config.h"

#include"net_tess_opt_comp_objective.h"

extern void net_tess_opt_comp_objective_x_morpho (const double *x,
					      struct TOPT *pTOpt);
extern void net_tess_opt_comp_objective_x_ori (const double *x, struct TOPT *pTOpt);
extern void net_tess_opt_comp_objective_x_crystal (const double *x,
					      struct TOPT *pTOpt);
extern void net_tess_opt_comp_objective_x_domain (const double *x,
					      struct TOPT *pTOpt);
extern void net_tess_opt_comp_objective_centroidal_update (struct TOPT *pTOpt);

extern int net_tess_opt_comp_objective_poly (struct TOPT *pTOpt);

extern void net_tess_opt_comp_objective_debugtest (struct TOPT TOpt);

#include"net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval.h"
#include"net_tess_opt_comp_objective_log/net_tess_opt_comp_objective_log.h"

#ifdef __cplusplus
}
#endif
