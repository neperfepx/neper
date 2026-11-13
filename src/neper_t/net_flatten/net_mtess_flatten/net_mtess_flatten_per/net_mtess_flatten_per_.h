/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut_t.h"
#include"../../../net_tess/net_tess3d/net_tess3d_periodic/net_tess3d_periodic.h"
#include"net_mtess_flatten_per.h"

extern int net_mtess_flatten_per_ver (struct TESS Tess, int ver, int *shift);
extern int net_mtess_flatten_per_edge (struct TESS Tess, int edge,
                                       int *shift);
extern int net_mtess_flatten_per_face (struct TESS Tess, int face,
                                       int *shift);
