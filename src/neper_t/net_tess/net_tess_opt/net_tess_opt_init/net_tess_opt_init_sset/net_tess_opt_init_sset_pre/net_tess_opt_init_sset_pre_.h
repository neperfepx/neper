/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
#include"net_ori/net_ori.h"

#include"neper_config.h"

#include"net_tess_opt_init_sset_pre.h"

extern void net_tess_opt_init_sset_pre_size (struct TESS *Tess, int
					     dtess, int dcell,
                                             struct TOPT *pTOpt);
