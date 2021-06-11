/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut_t.h"

#include"../../net_utils/net_utils.h"

#include"net_tess_tocta.h"
#include"net_tess_tocta_seed/net_tess_tocta_seed.h"
#include"../net_tess_cube/net_tess_cube.h"

extern int net_tess_tocta_cellid (struct SEEDSET SSet, struct TESS *pTess);
