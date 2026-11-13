/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<time.h>
#include<float.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>

#include"ut.h"
#include"neut_t.h"

#include"neper_config.h"

#include"net_tess_opt_comp.h"

#include"net_tess_opt_comp_pre/net_tess_opt_comp_pre.h"
#include"net_tess_opt_comp_post/net_tess_opt_comp_post.h"
#include"net_tess_opt_comp_minimize/net_tess_opt_comp_minimize.h"
#include"net_tess_opt_comp_minimize/net_tess_opt_comp_minimize_objective/net_tess_opt_comp_minimize_objective.h"

extern void net_tess_opt_comp_once (struct TOPT *pTOpt);
extern void net_tess_opt_comp_rand (struct TOPT *pTOpt);
extern void net_tess_opt_comp_lloyd (struct TOPT *pTOpt);

extern void net_tess_opt_comp_rand_init (struct TOPT *pTOpt);
extern void net_tess_opt_comp_rand_read (struct TOPT TOpt, int *pseedqty, int
  *pdimqty, double *pmin, double *pmax, int *pid, int **palldims, int
  *palldimqty);

extern void net_tess_opt_comp_rand_shift (struct TOPT *pTOpt, int
    seedqty, int dimqty, double min, double max, int *alldims, int
    alldimqty, gsl_rng *r);

extern void net_tess_opt_comp_rand_revert (struct TOPT TOpt,
					   double *x_cpy);
