/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include<gsl/gsl_rng.h>

#include"structIn_t.h"
#include"ut.h"
#include"float.h"
#include"neut_t.h"
#include"orilib.h"

#include"net_ori_uniform.h"
#include"../net_ori.h"

#include "neut/neut_structs/neut_qcloud_struct.hpp"
#include "neut/neut_oset/neut_oset.hpp"

extern void net_ori_uniform_log (struct IN_T In, int iter, struct OL_SET OSet, struct OOPT OOpt);
extern void net_ori_uniform_log_var (struct IN_T In, int iter, struct OL_SET OSet, struct OOPT OOpt);

extern void net_ori_uniform_init (struct IN_T In, int level, struct MTESS MTess,
                      struct TESS *Tess, int dtess, int dcell, long random,
                      struct OL_SET *pOSet, struct OOPT *pOOpt,
                      int verbositylevel);

extern int net_ori_uniform_opt (struct IN_T In, struct OOPT *pOOpt, struct OL_SET *pOSet, int verbositylevel);

extern void net_ori_uniform_opt_forces (struct OL_SET *pOSet,
                                double **f, double *E, struct OOPT *pOOpt,
                                struct QCLOUD *pqcloud, my_kd_tree_t *qindex);
extern void net_ori_uniform_opt_forces_ser (struct OL_SET *pOSet, double **f, double *E);
extern void net_ori_uniform_opt_forces_ser_neigh (struct OL_SET *pOSet, double **f,
                                          double *E, struct OOPT *pOOpt,
                                          struct QCLOUD *pqcloud,
                                          my_kd_tree_t *qindex);

extern int net_ori_uniform_opt_energy (int iter, OL_SET * pOSet,
                               double *E, double **preps,
                               struct OOPT *pOOpt);

extern void net_ori_uniform_opt_verbosity (struct OOPT OOpt, int iter,
                                   char *prevmessage, char *message,
                                   int verbositylevel);

extern void net_ori_uniform_opt_rot (struct OOPT *pOOpt, double **f, double alpha,
                             struct OL_SET *pOSet, double **dq);
extern void net_ori_uniform_opt_rot_ori (double **f, double alpha, int i,
                                 struct OL_SET *pOSet, double **dq);

extern void net_ori_uniform_opt_alpha (struct OOPT *pOOpt, struct OL_SET OSet, double **dq,
                               int iter, double **f, double **fc, double *palpha);

extern int net_ori_uniform_opt_stop (struct OOPT *pOOpt, int iter);

extern void net_ori_uniform_opt_forces_comp (struct OL_SET *pOSet, int i,
                                     int j, double *fij, double *pEij);

extern void net_ori_uniform_opt_forces_comp_neigh (struct OL_SET *pOSet, int i,
                                           int orioptineigh, double *fij,
                                           double *pEij,
                                           struct QCLOUD *pqcloud);
