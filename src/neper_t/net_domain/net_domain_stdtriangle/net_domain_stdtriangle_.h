/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<float.h>
#include<unistd.h>

#include "ut.h"
#include"neut_t.h"
#include "orilib.h"

#include"net_utils/net_utils.h"
#include "../net_domain.h"

extern void net_domain_stdtriangle_parms (char *stdtriangle, struct PF *pPf);
extern void net_domain_stdtriangle_compute (struct PF Pf, struct POLY *pDomain);
extern void net_domain_stdtriangle_compute_planes (struct PF Pf, double **);

#include"net_domain_stdtriangle.h"
