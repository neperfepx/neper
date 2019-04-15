/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_mis.h"

void
ol_g_lng (double** g, double** lng)
{
  double theta;

  ut_mat_skew (g, 3, lng);
  ol_g_theta_rad (g, &theta);
  if (theta != 0)
    ut_array_2d_scale (lng, 3, 3, theta / sin (theta));

  return;
}

void
ol_q_lnq (double* q, double* lnq)
{
  double theta;

  ol_q_rtheta_rad (q, lnq, &theta);
  ut_array_1d_scale (lnq, 3, theta / 2);

  return;
}

void
ol_lnq_q (double* lnq, double* q)
{
  double theta, *r = ol_r_alloc ();

  theta = 2 * ut_array_1d_norm (lnq, 3);
  ut_array_1d_memcpy (r, 3, lnq);
  ol_r_set_unit (r);

  ol_rtheta_q (r, theta, q);

  ol_r_free (r);

  return;
}
