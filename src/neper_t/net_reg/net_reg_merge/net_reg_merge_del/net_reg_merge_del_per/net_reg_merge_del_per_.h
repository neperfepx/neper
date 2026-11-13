/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"
#include"neut_t.h"
#include"net_reg_merge_del_per.h"

extern void net_reg_merge_del_per_vermerge_reset (struct TESS *pTess, int ver);
extern void net_reg_merge_del_per_vermerge_ver (struct TESS *pTess, int ver, int ver2);
extern void net_reg_merge_del_per_vermerge_ver_edge (struct TESS *pTess, int edge, int edge2);
extern int net_reg_merge_del_per_vermerge_testedges (struct TESS Tess, int ver);
