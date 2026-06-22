/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"neut_t.h"

#include"net_transform_tess.h"

#include"net_transform_tess_cut/net_transform_tess_cut.h"
#include"net_ori/net_ori.h"

extern void net_transform_tess_ori (char *ori, struct TESS *pTess);
extern void net_transform_tess_crysym (char *ori, struct TESS *pTess);
