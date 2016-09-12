/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_t.h"

#include"neper_config.h"

#ifdef HAVE_NLOPT
#include"net_tess_opt_comp_nlopt_init.h"
#include"../net_tess_opt_comp_objective/net_tess_opt_comp_objective.h"

extern void net_tess_opt_comp_nlopt_init_stop (struct TOPT *pTOpt);
extern void net_tess_opt_comp_nlopt_init_bounds (struct TOPT *pTOpt);
extern void net_tess_opt_comp_nlopt_init (struct TOPT *pTOpt);
extern void net_tess_opt_comp_nlopt_init_objective (struct TOPT *pTOpt);
#endif
