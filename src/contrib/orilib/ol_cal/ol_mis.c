/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_mis.h"

void
ol_g_lng (double **g, double **lng)
{
  double theta;

  ut_mat_skew (g, 3, lng);
  ol_g_theta_rad (g, &theta);
  if (theta != 0)
    ut_array_2d_scale (lng, 3, 3, theta / sin (theta));

  return;
}

void
ol_lng_g (double **lng, double **g)
{
  // g = exp (lng) = I + sin (theta) / theta lng + (1 - cos (theta)) / theta^2 lng^2
  // (Rodrigues' formula), where theta is the rotation angle
  int i, j, k;
  double theta, a, b, W[3][3], W2[3][3];

  theta = 0;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      W[i][j] = lng[i][j];
      theta += lng[i][j] * lng[i][j];
    }
  theta = sqrt (theta / 2);

  if (theta > OL_EPS_RAD)
  {
    a = sin (theta) / theta;
    b = (1 - cos (theta)) / (theta * theta);
  }
  else
  {
    a = 1;
    b = 0.5;
  }

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      W2[i][j] = 0;
      for (k = 0; k < 3; k++)
        W2[i][j] += W[i][k] * W[k][j];
    }

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      g[i][j] = (i == j) + a * W[i][j] + b * W2[i][j];

  return;
}

void
ol_q_lnq (double *q, double *lnq)
{
  /* quaternion logarithm: q and -q are different (|lnq| in [0, pi]) */
  double ang = ut_num_acos (q[0]);
  double norm = ut_array_1d_norm (q + 1, 3);

  if (norm > 0)
  {
    ut_array_1d_memcpy (q + 1, 3, lnq);
    ut_array_1d_scale (lnq, 3, ang / norm);
  }
  else
    ut_array_1d_set_3 (lnq, ang, 0, 0);     /* q = +/-1: zero or pi, about any axis */

  return;
}

void
ol_lnq_q (double *lnq, double *q)
{
  double *rtheta = ol_rtheta_alloc ();

  if (ut_array_1d_norm (lnq, 3) > OL_EPS_RAD)
  {
    ut_array_1d_memcpy (lnq, 3, rtheta);
    ol_r_set_unit (rtheta);
    rtheta[3] = 2 * ut_array_1d_norm (lnq, 3);

    ol_rtheta_q_rad (rtheta, q);
  }
  else
    ol_q_set_id (q);

  ol_rtheta_free (&rtheta);

  return;
}
