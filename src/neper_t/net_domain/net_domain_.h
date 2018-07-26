/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<float.h>
#include<unistd.h>

#include"../structIn_t.h"
#include "ut.h"
#include"neut_t.h"
#include "orilib.h"

#include"net_utils/net_utils.h"
#include"net_domain_cube/net_domain_cube.h"
#include "../net_tess/net_tess3d/net_polycomp/net_polycomp_poly/net_polycomp_poly.h"

extern void net_domain_cubeparms (char *domain, double **size);
extern void net_domain_stdtriangle_planes (int, double **);
extern void net_domain_cylinder_planes (double, double, int, double **);
extern void net_domain_sphere_planes (double rad, int qty, double **eq);
extern void net_domain_rodrigues (struct POLY *pDomain, char *crysym);
extern void net_domain_clip (struct POLY *, double **, int);
extern void net_domain_tesspoly_planes (struct TESS, int, int *, double **);
extern void net_domain_transform (struct TESS *pDomain, char* string);

extern void net_domain_cylinder_string (char *domain, char *nstring, struct POLY *pDomain);
extern void net_domain_cylinderparms (char *domain, char *nstring, double *parms, int *ppseudodim, double *ppseudosize);
extern void net_domain_cylinder (double *parms, struct POLY *pDomain);

#include"net_domain.h"
