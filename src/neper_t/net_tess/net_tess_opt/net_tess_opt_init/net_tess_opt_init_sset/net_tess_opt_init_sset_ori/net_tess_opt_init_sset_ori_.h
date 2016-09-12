/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gsl/gsl_rng.h>

#include"structIn_t.h"
#include"ut.h"
#include"neut_t.h"
#include"orilib.h"

#include"net_tess_opt_init_sset_ori.h"

extern void net_tess_opt_init_sset_ori_3d (struct SEEDSET *pSSet);
extern void net_tess_opt_init_sset_ori_fibre (char *distrib,
					      struct SEEDSET *pSSet);
