/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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

#include"neper_config.h"

#include"net_tess_opt_init_tesrobj.h"

extern void net_tess_opt_init_tesrobj_pre (struct TOPT *pTOpt);
extern void net_tess_opt_init_tesrobj_scale (struct TOPT *pTOpt);
extern void net_tess_opt_init_tesrobj_rasterscale (struct TOPT *pTOpt);
extern void net_tess_opt_init_tesrobj_region (char *region, struct TOPT *pTOpt);
extern void net_tess_opt_init_tesrobj_res (char *sample, struct TOPT *pTOpt);
extern void net_tess_opt_init_tesrobj_post (struct TOPT *pTOpt);
extern void net_tess_opt_init_tesrobj_res_size (char *sample, struct TOPT
    TOpt, int *size, double *vsize);
extern void net_tess_opt_init_tesrobj_res_cell (int *size, double *vsize, int cell, struct TOPT *pTOpt);
