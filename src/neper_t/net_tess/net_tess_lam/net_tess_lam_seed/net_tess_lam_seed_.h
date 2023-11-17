/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<gsl/gsl_rng.h>

#include"structIn_t.h"
#include"ut.h"
#include"orilib.h"
#include"neut_t.h"

#include"net_tess_lam_seed.h"
#include"net_ori/net_ori.h"

extern int net_tess_lam_seed_readargs (char *morpho,
				  struct MTESS MTess, struct TESS *Tess,
				  int domtess, int dompoly,
                                  char **pwtype, double **pw,
				  int *pwqty, char **pvtype, double **pv, int *pvqty,
                                  char **ppostype, char **ppos, double *ptol);

extern int net_tess_lam_seed_readargs_w (char *value,
				    struct MTESS MTess, struct TESS *Tess,
				    int domtess, int dompoly,
				    char **pwtype, double **pw, int *pwqty);
extern int net_tess_lam_seed_readargs_v (char *value,
				    struct MTESS MTess, struct TESS *Tess,
				    int domtess, int dompoly,
				    char **pvtype, double **pv, int *pvqty);
extern int net_tess_lam_seed_readargs_pos (char *value, struct MTESS MTess,
                                    struct TESS *Tess, int domtess,
                                    int dompoly, char **ppostype, char **ppos);

extern int net_tess_lam_seed_set (struct IN_T In, int level, struct MTESS
                                  MTess, struct TESS *Tess, int dtess, int
                                  dcell, struct TESS Dom, char *wtype, double
                                  *w, int wqty, char *vtype, double *v, int
                                  vqty, char *postype, char *pos, double reps,
                                  struct SEEDSET *SSet, struct SEEDSET *pSSet);

extern int net_tess_lam_seed_set_init (struct SEEDSET *pSSet);

extern int net_tess_lam_seed_set_normal (struct SEEDSET *SSet, int dtess, int dcell,
                                    gsl_rng *r, char *vtype, double *v,
				    int vqty, double *normal);

extern int net_tess_lam_seed_set_lam (struct TESS Dom, gsl_rng *r,
				 double *n, char *wtype, double *w,
				 int wqty, char *postype, char *pos,
                                 double reps, struct SEEDSET *pSSet);

extern int net_tess_lam_seed_set_finalize (struct SEEDSET *pSSet);

extern int net_tess_lam_seed_set_w_pre (gsl_rng *r, struct TESS Dom, double *n,
				   char *wtype, double *w, int wqty, char *postype,
                                   char *pos, double *plane, double reps,
                                   double *pdistmin, double *pdistmax);
extern int net_tess_lam_seed_set_addlam (double distcur, double *n, double w, int w_id,
				    struct SEEDSET *pSSet);
