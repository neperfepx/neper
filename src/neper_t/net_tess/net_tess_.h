/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<float.h>
#include<limits.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>

#include"ut.h"
#include"neut_t.h"

#include"neper_config.h"

#include"../structIn_t.h"
#include"net_tess_cube/net_tess_cube.h"
#include"net_tess_lam/net_tess_lam.h"
#include"net_tess_opt/net_tess_opt.h"
#include"net_utils/net_utils.h"

#include"net_tess.h"

extern int net_tess_finalize (struct TESS PTess, struct SEEDSET SSet, struct TESS *pTess);

extern int net_tess_3dto2d (struct TESS *pTess);
