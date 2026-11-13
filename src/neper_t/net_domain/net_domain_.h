/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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
#include"net_domain_cube/net_domain_cube.h"
#include"net_domain_stdtriangle/net_domain_stdtriangle.h"
#include "../net_tess/net_tess3d/net_tess3d_polycomp/net_tess3d_polycomp_poly/net_tess3d_polycomp_poly.h"
#include "../net_transform/net_transform_tess/net_transform_tess_cut/net_transform_tess_cut.h"

extern void net_domain_fromfile (char *filename, struct TESS *pDomain);

extern int net_domain_fromtess_3d (struct TESS Tess, double ***peqs, int *peqqty,
                                   struct PRIM **pPrim, int *pPrimQty);
extern int net_domain_fromtess_2d (struct TESS Tess, double ***peqs, int *peqqty,
                                   struct POLY *pPoly);
extern int net_domain_fromtess_convex (struct TESS Tess, double **eqs, int eqqty);
extern double net_domain_fromtess_bbox (struct TESS Tess, double **bbox);
extern int net_domain_fromtess_prim (struct TESS Tess, int domface, struct PRIM *pPrim);
extern void net_domain_fromtess_addeq (double *eq, double ***peqs, int *peqqty);
extern void net_domain_fromtess_hull (struct TESS Tess, int domface, struct PRIM Prim,
                                      double ***peqs, int *peqqty);

extern void net_domain_cubeparms (char *domain, double **size);
extern void net_domain_cylinder_planes (double, double, int, double **);
extern void net_domain_sphere_planes (double rad, int qty, double **eq);
extern void net_domain_tesspoly_planes (struct TESS, int, int *, double **);
extern void net_domain_transform (struct TESS *pDomain, int dim, char* string);

extern int net_domain_curved (struct IN_T In, char **strings, int stringqty,
                              char *domain);
extern void net_domain_cylinder_string (char *domain, char *nstring, int curved,
                                        struct POLY *pDomain, struct PRIM **pPrim,
                                        int *pPrimQty);
extern void net_domain_cylinderparms (char *domain, char *nstring, double *parms, int *ppseudodim, double *ppseudosize);
extern void net_domain_cylinder (double *parms, int curved, struct POLY *pDomain);
extern void net_domain_cylinder_hullplanes (double h, double Rad, int qty, double **eq);
extern void net_domain_cylinder_prim (double *parms, struct PRIM **pPrim, int *pPrimQty);

extern void net_domain_hourglass_string (char *domain, char *nstring,
                                         struct POLY *pDomain,
                                         struct PRIM **pPrim, int *pPrimQty);
extern void net_domain_hourglassparms (char *domain, char *nstring, double *parms);
extern void net_domain_hourglass_prim (double *parms, struct PRIM **pPrim, int *pPrimQty);
extern void net_domain_sphere_string (char *domain, int curved, struct POLY *pDomain,
                                      struct PRIM **pPrim, int *pPrimQty);
extern void net_domain_sphereparms (char *domain, double *parms);
extern void net_domain_sphere (double *parms, int curved, struct POLY *pDomain);
extern void net_domain_sphere_hullplanes (double rad, int qty, double **eq);
extern void net_domain_sphere_prim (double *parms, struct PRIM **pPrim, int *pPrimQty);

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
