/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<float.h>

#include"ut.h"
#include"neut_t.h"
#include"net_reg_merge/net_reg_merge.h"

#include"net_reg.h"

// tolerance on the angle of a straight corner, in degrees
#define NET_REG_STRAIGHTANGLE_TOL 1e-3

extern void net_reg_init (struct IN_T In, struct TESS *pTess, struct REG *pReg);
extern void net_reg_init_peredgesel (struct TESS Tess, double *edgesel);
extern void net_reg_init_facestraightvers (struct TESS Tess, struct REG *pReg);
extern void net_reg_periodic (struct TESS *pTess);
extern void net_reg_faceeq (struct TESS *pTess);
extern void net_reg_periodic_ver (struct TESS *pTess);
extern void net_reg_periodic_edge (struct TESS *pTess);
extern void net_reg_periodic_face (struct TESS *pTess);
