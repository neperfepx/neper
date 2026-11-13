/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"neut_t.h"

#include"net_transform_tess.h"

#include"net_transform_tess_cut/net_transform_tess_cut.h"
#include"net_ori/net_ori.h"
#include"net_tess/net_tess.h"
#include"net_tess/net_tess3d/net_tess3d.h"
#include"net_tess/net_tess3d/net_tess3d_domain/net_tess3d_domain.h"
#include"net_tess/net_tess3d/net_tess3d_periodic/net_tess3d_periodic.h"
#include"net_domain/net_domain.h"

extern void net_transform_tess_ori (char *ori, struct TESS *pTess);
extern void net_transform_tess_crysym (char *ori, struct TESS *pTess);
extern void net_transform_tess_renamedomface (char *expr, struct TESS *pTess);
extern void net_transform_tess_rmseed (struct IN_T In, struct TESS Dom, char *expr,
                                       struct TESS *pTess);

extern void net_transform_tess_rmseed_seedset (struct TESS Tess, int *old_new,
                                               struct TESS Dom, struct SEEDSET *pSSet);
extern void net_transform_tess_rmseed_tess (struct IN_T In, struct TESS Dom,
                                            struct SEEDSET SSet, struct TESS *pTess);
extern void net_transform_tess_rmseed_celldata (struct TESS Old, int *old_new,
                                                struct TESS *pTess);
