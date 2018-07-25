/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void neut_ori_volthetafct (struct FCT *pvolfct);
  extern void neut_ori_vol_theta (struct FCT *pvolfct, double vol,
                              double *ptheta);
  extern void neut_ori_n_avthetaeq (struct FCT *pvolfct, int n, char *crysym,
                                double *ptheta);
  extern void neut_ori_n_avdiameq (struct FCT *pvolfct, int n, char *crysym,
                               double *pdiameq);
  extern void neut_ori_n_avradeq (struct FCT *pvolfct, int n, char *crysym,
                              double *pradeq);

  extern void neut_ori_f_qdotnorm (double *f, double *pqdotnorm);
  extern void neut_ori_f_r (double *f, double *r);
  extern void neut_ori_f_qdot (double *f, double *qdot);

  extern void neut_ori_f_norm (double **f, int N, double *pnorm);

#ifdef __cplusplus
}
#endif
