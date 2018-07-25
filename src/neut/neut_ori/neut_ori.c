/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_ori_.h"

void
neut_ori_volthetafct (struct FCT *pvolfct)
{
  int i;
  double *ptmp = NULL;

  ut_fct_set_numerical (pvolfct, 0, M_PI, 100000, "bounds");

  for (i = 0; i < 100000; i++)
    (*pvolfct).y[i] = M_PI * ((*pvolfct).x[i] - sin ((*pvolfct).x[i]));
  ptmp = (*pvolfct).x;
  (*pvolfct).x = (*pvolfct).y;
  (*pvolfct).y = ptmp;

  ut_fct_set_init_interp (pvolfct);

  return;
}

void
neut_ori_vol_theta (struct FCT *pvolfct, double vol, double *ptheta)
{
  (*ptheta) = ut_fct_eval (*pvolfct, vol);

  return;
}

void
neut_ori_n_avthetaeq (struct FCT *pvolfct, int N, char *crysym, double *ptheta)
{
  double vol = pow (M_PI, 2) / (N * ol_crysym_qty (crysym));

  if (pvolfct)
    (*ptheta) = ut_fct_eval (*pvolfct, vol);
  else
  {
    struct FCT thetafct;
    ut_fct_set_zero (&thetafct);
    neut_ori_volthetafct (&thetafct);
    (*ptheta) = ut_fct_eval (thetafct, vol);
    ut_fct_free (&thetafct);
  }

  return;
}

void
neut_ori_n_avradeq (struct FCT *pvolfct, int N, char *crysym, double *pradeq)
{
  neut_ori_n_avthetaeq (pvolfct, N, crysym, pradeq);
  (*pradeq) /= 2;

  return;
}

void
neut_ori_n_avdiameq (struct FCT *pvolfct, int N, char *crysym, double *pdiameq)
{
  neut_ori_n_avthetaeq (pvolfct, N, crysym, pdiameq);

  return;
}

void
neut_ori_f_qdotnorm (double *f, double *pqdotnorm)
{
  (*pqdotnorm) = ut_vector_norm (f);

  return;
}

void
neut_ori_f_qdot (double *f, double *qdot)
{
  double qdotnorm;

  neut_ori_f_qdotnorm (f, &qdotnorm);
  qdot[0] = 0;
  neut_ori_f_r (f, qdot + 1);
  ut_array_1d_scale (qdot + 1, 3, qdotnorm);

  return;
}

void
neut_ori_f_r (double *f, double *r)
{
  if (ut_vector_norm (f) != 0)
  {
    ut_array_1d_memcpy (r, 3, f);
    ut_array_1d_scale (r, 3, 1. / ut_vector_norm (r));
  }
  else
    ut_array_1d_set_3 (r, 1, 0, 0);

  return;
}

void
neut_ori_f_norm (double **f, int N, double *pnorm)
{
  int i;

  (*pnorm) = 0;
#pragma omp parallel for
  for (i = 0; i < N; i++)
#pragma omp atomic
    (*pnorm) += ut_vector_norm_squared (f[i]);
  (*pnorm) = sqrt (*pnorm);

  return;
}
