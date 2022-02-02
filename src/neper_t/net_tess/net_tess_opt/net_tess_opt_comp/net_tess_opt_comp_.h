/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<time.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>

#include"ut.h"
#include"neut_t.h"

#include"neper_config.h"

#include"net_tess_opt_comp.h"

#include"net_tess_opt_comp_x/net_tess_opt_comp_x.h"
#include"net_tess_opt_comp_nlopt_init/net_tess_opt_comp_nlopt_init.h"
#include"net_tess_opt_comp_post/net_tess_opt_comp_post.h"
#include"net_tess_opt_comp_objective/net_tess_opt_comp_objective.h"

extern int net_tess_opt_comp_once (double *x, struct TOPT *pTOpt);
extern int net_tess_opt_comp_rand (double *x, struct TOPT *pTOpt);
extern int net_tess_opt_comp_lloyd (double *x, struct TOPT *pTOpt);
extern int net_tess_opt_comp_nlopt (double *x, struct TOPT *pTOpt);

extern void net_tess_opt_comp_rand_init (struct TOPT *pTOpt);
extern void net_tess_opt_comp_rand_read (struct TOPT TOpt, int *pseedqty, int
  *pdimqty, double *pmin, double *pmax, int *pid, int **palldims, int
  *palldimqty);

extern void net_tess_opt_comp_rand_shift (double *x, struct TOPT *pTOpt, int
    seedqty, int dimqty, double min, double max, int *alldims, int
    alldimqty, gsl_rng *r);

extern void net_tess_opt_comp_rand_shift_seeds (double *x, struct TOPT *pTOpt, int
    seedqty, int dimqty, double min, double max, int *alldims, int
    alldimqty, gsl_rng *r);

extern void net_tess_opt_comp_rand_shift_crystal (double *x, struct TOPT *pTOpt,
    int dimqty, double min, double max, int *alldims, int alldimqty, gsl_rng *r);

extern void net_tess_opt_comp_rand_revert (double *x, struct TOPT TOpt,
					   double *x_cpy);
