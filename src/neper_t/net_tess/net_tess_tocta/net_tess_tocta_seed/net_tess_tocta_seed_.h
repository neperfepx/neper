/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<gsl/gsl_rng.h>

#include"structIn_t.h"
#include"ut.h"
#include"orilib.h"
#include"neut_t.h"
#include"net_ori/net_ori.h"

#include"net_tess_tocta_seed.h"

extern int net_tess_tocta_seed_readargs (char *morpho, int *pn);

extern int net_tess_tocta_seed_set (struct IN_T In, struct MTESS MTess, struct TESS *Tess, int dtess, int dcell, struct TESS Dom, int n, struct SEEDSET *SSet, struct SEEDSET *pSSet);

extern int net_tess_tocta_seed_set_finalize (struct SEEDSET *pSSet);
