/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>
#ifdef HAVE_ZEROMQ
#include <assert.h>
#include <zmq.h>
#endif // HAVE_ZEROMQ

#include"ut.h"
#include"neut_t.h"
#include"structIn_t.h"
#include"../../net_tess.h"

#include"neper_config.h"

#include"net_tess_opt_init.h"
#include"net_tess_opt_init_sset/net_tess_opt_init_sset.h"
#include"net_tess_opt_init_tesrobj/net_tess_opt_init_tesrobj.h"
#include"net_transform/net_transform_tess/net_transform_tess_cut/net_transform_tess_cut.h"

extern void net_tess_opt_init_general (struct IN_T In, int level, char *optitype,
                                       struct MTESS MTess, struct TESS *Tess,
                                       int dtess, int dcell, struct TOPT *pTOpt);

extern void net_tess_opt_init_parms (struct IN_T In, int level, struct MTESS MTess,
				     struct TESS *Tess, int dtess, int dcell, struct TOPT *pTOpt);

extern void net_tess_opt_init_target (struct IN_T In, int level, char *optistring,
                                      struct MTESS MTess, struct TESS *Tess, int domtess,
				      int dompoly, struct TOPT *pTOpt);

extern void net_tess_opt_init_crystal (struct IN_T In, int level, struct TOPT *pTOpt);

extern void net_tess_opt_init_general_domain (struct IN_T In, struct TESS PTess,
				      int cell, struct TOPT *pTOpt);

extern void net_tess_opt_init_general_cellqty (struct IN_T In, struct
                                               MTESS MTess, struct TESS *pTess,
                                               int poly, int *pCellQty);

extern void net_tess_opt_init_target_scale (struct TOPT *pTOpt, int *pos);

extern void net_tess_opt_init_current (struct TOPT *pTOpt);

extern void net_tess_opt_init_bounds (struct TOPT *pTOpt);
extern void net_tess_opt_init_bounds_seeds (struct TOPT *pTOpt);
extern void net_tess_opt_init_bounds_ori (struct TOPT *pTOpt);
extern void net_tess_opt_init_bounds_crystal (struct TOPT *pTOpt);

extern void net_tess_opt_init_parms_objective (char *distribobjective,
					       struct TOPT *pTOpt);

extern void net_tess_opt_init_parms_algo (struct IN_T, int level, struct MTESS MTess, struct TESS *Tess, int dtess, int dcell, struct TOPT *pTOpt);
extern void net_tess_opt_init_parms_algomaxiter (struct IN_T, int level, struct TOPT *pTOpt);

extern void net_tess_opt_init_target_bin (double xmin, double xmax,
					  double mean, int ptqty, double *x);

extern void net_tess_opt_init_ref (struct TOPT *pTOpt, double mean, int id);

extern void net_tess_opt_init_target_grid (struct IN_T In, int level,
					   struct MTESS MTess, struct TESS *Tess,
					   int dtess, int dcell,
					   int var, struct TOPT *pTOpt);

extern void net_tess_opt_init_target_cvl (struct IN_T In, int level,
					  struct MTESS MTess, struct TESS *Tess,
					  int dtess, int dcell,
					  int var, struct TOPT *pTOpt);

extern void net_tess_opt_init_post (struct IN_T In, struct TOPT *pTOpt);

#ifdef __cplusplus
}
#endif
