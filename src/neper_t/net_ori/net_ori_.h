/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<gsl/gsl_rng.h>
#include<time.h>
#include<sys/time.h>

#include"structIn_t.h"
#include"ut.h"
#include"neut_t.h"
#include"orilib.h"

#include"net_ori.h"

#include"net_ori_uniform/net_ori_uniform.h"

extern void net_ori_qty (struct SEEDSET *pSSet, char **parts, int partqty, int **pqty);

extern void net_ori_fiber (struct SEEDSET *SSet, int dtess, int dcell, long random,
                           char *distrib, struct OL_SET *pOSet);

extern void net_ori_odf (long random, char *odf, struct OL_SET *pOSet);
extern void net_ori_odf_pre (char *odf, struct OL_SET *pOSet, struct ODF *pOdf);

extern void net_ori_oricrysym (struct OL_SET *pOSet);

extern void net_ori_mtess_params (struct IN_T In, int level, struct MTESS MTess,
                                  struct TESS *Tess, int dtess, int dcell,
                                  char **pori, char **porisampling,
                                  char **porispread, char **pcrysym);

extern int net_ori_mtess_randseed_rand (int *N, int *id, int *poly, int levelqty);

extern void net_ori_orispread (char *orispread, struct SEEDSET *pSSet);

extern void net_orispread_file (char *orispread, struct SEEDSET *pSSet);

extern void net_ori_addspread (char *spread, long random, struct OL_SET *pOSet);

extern void net_ori_ks (long random, char *label, struct SEEDSET *SSet, int dtess, int dcell, struct OL_SET *pOSet);

extern void net_ori_ti_beta2alpha (long random, char *label, struct SEEDSET *SSet, int dtess, int dcell, struct OL_SET *pOSet);

extern void net_ori_parent (char *label, struct SEEDSET *SSet, int dtess, int dcell,
                           struct OL_SET *pOSet);

extern void net_ori_label (char *label, struct SEEDSET *SSet, int dtess, int dcell,
                           struct OL_SET *pOSet);
