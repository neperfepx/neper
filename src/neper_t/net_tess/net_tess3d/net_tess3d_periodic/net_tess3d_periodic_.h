/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_t.h"

#include"../../../net_utils/net_utils.h"
#include"net_tess3d_periodic.h"

extern void net_tess3d_periodic_face (struct TESS *pTess);
extern void net_tess3d_periodic_edge (struct TESS *pTess);
extern void net_tess3d_periodic_ver (struct TESS *pTess);

extern void net_tess3d_periodic_ver_shrinkmaster (struct TESS *);
extern void net_tess3d_periodic_edge_shrinkmaster (struct TESS *);
extern void net_tess3d_periodic_edge_peredgeori (struct TESS *pTess);
