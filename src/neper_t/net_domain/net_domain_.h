/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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
extern void net_domain_clip (struct POLY *, double **, int);
extern void net_domain_tesspoly_planes (struct TESS, int, int *, double **);
extern void net_domain_transform (struct TESS *pDomain, int dim, char* string);

extern void net_domain_cylinder_string (char *domain, char *nstring, struct POLY *pDomain);
extern void net_domain_cylinderparms (char *domain, char *nstring, double *parms, int *ppseudodim, double *ppseudosize);
extern void net_domain_cylinder (double *parms, struct POLY *pDomain);

extern void net_domain_stdtriangle_string (char *domain, struct POLY *pDomain);
extern void net_domain_stdtriangleparms (char *domain, double *parms);
extern void net_domain_stdtriangle (double *parms, struct POLY *pDomain);

extern void net_domain_sphere_string (char *domain, struct POLY *pDomain);
extern void net_domain_sphereparms (char *domain, double *parms);
extern void net_domain_sphere (double *parms, struct POLY *pDomain);

extern void net_domain_rodrigues_string (char *domain, struct POLY *pDomain);
extern void net_domain_rodriguesparms (char *domain, char **pcrysym);
extern void net_domain_rodrigues (char *crysym, struct POLY *pDomain);

extern void net_domain_planes_string (char *domain, int dim, struct POLY *pDomain);
extern void net_domain_planesparms (char *domain, int dim, double ***peqs, int *peqqty);
extern void net_domain_planes (double **eqs, int eqqty, struct POLY *pDomain);

extern void net_domain_cell_string (char *domain, struct POLY *pDomain);
extern void net_domain_cellparms (char *domain, char **pfilename, int *pcell);
extern void net_domain_cell (char *filename, int cell, struct POLY *pDomain);

#include"net_domain.h"
