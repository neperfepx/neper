/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>

#include"ut.h"
#include"neut_t.h"

#include"neper_config.h"

#include"net_tess_opt_init_sset_coo.h"
#include"net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp.h"

extern void net_tess_opt_init_sset_coo_cluster (int dim, int *activedim,
    gsl_rng * r2, int qty, double dist, double rad, struct POINT *pPoint);

extern void net_tess_opt_init_sset_coo_centre (struct TOPT *pTOpt,
    gsl_rng *r, char *var, int pos, char *cooexpr, struct POINT Point,
    struct POINT Point2, int cell, double *centre);

extern void net_tess_opt_init_sset_coo_record (struct TOPT *pTOpt, int
    cell, struct POINT *pPoint, double *centre, struct POINT *pPoint2);

extern void net_tess_opt_init_sset_coo_lllfp2011 (struct TOPT *pTOpt);

extern int net_tess_opt_init_sset_coo_bcc_expr (struct TESS Dom, char *cooexpr, double ***pcoo);

extern int net_tess_opt_init_sset_coo_centre_randpt (struct TOPT *pTOpt, struct POINT
                                    Point, gsl_rng *r, double rad,
                                    double penetration, double
                                    *coo, double *pdist);
extern int net_tess_opt_init_sset_coo_centre_randpt_pick (struct TOPT *pTOpt, struct POINT
    Point, int *activedim, gsl_rng *r, double *coo);
extern int net_tess_opt_init_sset_coo_centre_randpt_test (struct TOPT *pTOpt, struct POINT
    Point, double rad, double penetration, double *coo, double
    *pdist);
extern int net_tess_opt_init_sset_coo_centre_randpt_cluster (struct TOPT
    *pTOpt, struct POINT Point, gsl_rng *r, struct POINT
    Point2, double penetration, double *coo, double *pdist);
