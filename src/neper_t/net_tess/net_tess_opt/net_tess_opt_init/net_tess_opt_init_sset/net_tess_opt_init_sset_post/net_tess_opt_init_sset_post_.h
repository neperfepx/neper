/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>

#include"ut.h"
#include"neut_t.h"
#include"structIn_t.h"

#include"neper_config.h"

#include"net_tess_opt_init_sset_post.h"

extern void net_tess_opt_init_sset_post_log (struct IN_T In, int level,
					     struct TOPT *pTOpt);

extern void net_tess_opt_init_sset_post_per (int *periodic, int level,
                                             struct TESS Tess, int poly,
                                             struct SEEDSET *pSSet);
