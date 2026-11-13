/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<float.h>
#include<unistd.h>

#include"ut.h"
#include"neut_t.h"

#include"net_reg_merge_del/net_reg_merge_del.h"
#include"net_reg_merge.h"

extern int net_reg_merge_nextedge (struct TESS, struct REG, double *);
extern double net_reg_merge_search_vernbs (struct TESS *pTess, struct REG Reg,
                                           int ver, double lratio,
                                           int invalid, int depth,
                                           int ***bufs,
                                           struct TESS *TessBufs, int *seq,
                                           int *pseqqty);
extern int net_reg_merge_search_edgeok (struct TESS Tess, int edge);
extern void net_reg_merge_print (int, double, int, int, int, double, double);
extern void net_reg_merge_buffer_per (struct TESS Tess, struct TESS *pTessBuf);
