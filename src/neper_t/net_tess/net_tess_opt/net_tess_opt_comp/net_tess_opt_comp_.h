/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<time.h>

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
