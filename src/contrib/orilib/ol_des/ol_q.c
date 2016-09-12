/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_q.h"

double *
ol_q_alloc ()
{
  double *q = ut_alloc_1d (4);

  q[0] = 1;

  return q;
}

void
ol_q_free (double *q)
{
  if (!q)
    return;
  else
    free (q);

  return;
}

void
ol_q_set_zero (double *q)
{
  ut_array_1d_zero (q, 4);

  return;
}

void
ol_q_set_id (double *q)
{
  q[0] = 1;
  ut_array_1d_zero (q + 1, 3);

  return;
}

void
ol_q_set_unit (double *q)
{
  double norm = sqrt (q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
  ut_array_1d_scale (q, 4, 1 / norm);

  return;
}


void
ol_q_set_this (double *q, double rho, double lambda, double mu, double nu)
{
  q[0] = rho;
  q[1] = lambda;
  q[2] = mu;
  q[3] = nu;

  return;
}

void
ol_q_memcpy (double *qsrc, double *qdest)
{
  ut_array_1d_memcpy (qdest, 4, qsrc);

  return;
}

void
ol_g_q (double **g, double *q)
{
  int i, m;

  q[0] = 0.5 * ut_num_sqrt (g[0][0] + g[1][1] + g[2][2] + 1);

  if (fabs (q[0]) > OL_EPS)
  {
    q[1] = (g[1][2] - g[2][1]) / (4 * q[0]);
    q[2] = (g[2][0] - g[0][2]) / (4 * q[0]);
    q[3] = (g[0][1] - g[1][0]) / (4 * q[0]);
  }
  else
  {
    for (i = 0; i <= 2; i++)
      q[i + 1] = ut_num_sqrt ((g[i][i] + 1) / 2);

    m = 1 + ut_array_1d_max_index (q + 1, 3);
    for (i = 1; i <= 3; i++)
      if (i != m)
	q[i] *= ut_num_sgn (g[i - 1][m - 1]);
  }

  return;
}

void
ol_q_g (double *q, double **g)
{
  g[0][0] =
    q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
  g[1][1] =
    q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3];
  g[2][2] =
    q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];

  g[0][1] = 2 * (q[1] * q[2] + q[0] * q[3]);
  g[0][2] = 2 * (q[1] * q[3] - q[0] * q[2]);

  g[1][0] = 2 * (q[1] * q[2] - q[0] * q[3]);
  g[1][2] = 2 * (q[2] * q[3] + q[0] * q[1]);

  g[2][0] = 2 * (q[1] * q[3] + q[0] * q[2]);
  g[2][1] = 2 * (q[2] * q[3] - q[0] * q[1]);

  return;
}

void
ol_rtheta_q (double *r, double theta, double *q)
{
  ol_rtheta_q_rad (r, theta * OL_PI / 180, q);

  return;
}

void
ol_rtheta_q_rad (double *r, double theta, double *q)
{
  int i;

  q[0] = cos (theta / 2);

  for (i = 0; i <= 2; i++)
    q[i + 1] = r[i] * sin (theta / 2);

  return;
}

void
ol_q_rtheta (double *q, double *r, double *ptheta)
{
  ol_q_rtheta_rad (q, r, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_q_rtheta_rad (double *q, double *r, double *ptheta)
{
  int i;

  (*ptheta) = 2 * ut_num_acos (q[0]);

  if ((*ptheta) < OL_EPS_DEG)
  {
    r[0] = 1;
    r[1] = 0;
    r[2] = 0;
  }
  else
    for (i = 0; i <= 2; i++)
      r[i] = q[i + 1] / sin ((*ptheta) / 2);

  return;
}

void
ol_q_r (double *q, double *r)
{
  double theta;

  ol_q_rtheta_rad (q, r, &theta);
}

void
ol_q_R (double *q, double *R)
{
  double *r = ol_r_alloc ();
  double theta;

  ol_q_rtheta_rad (q, r, &theta);
  ol_rtheta_R_rad (r, theta, R);

  ol_r_free (r);

  return;
}

void
ol_R_q (double *R, double *q)
{
  double *r = ol_r_alloc ();
  double theta;

  ol_R_rtheta_rad (R, r, &theta);
  ol_rtheta_q_rad (r, theta, q);

  ol_r_free (r);

  return;
}

void
ol_q_theta_rad (double *q, double *ptheta)
{
  (*ptheta) = 2 * ut_num_acos (q[0]);

  return;
}

void
ol_q_theta (double *q, double *ptheta)
{
  ol_q_theta_rad (q, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_e_q (double *e, double *q)
{
  double *e2;

  e2 = ol_e_alloc ();
  ol_e_deg2rad (e, e2);

  ol_e_q_rad (e2, q);

  ol_e_free (e2);

  return;
}

void
ol_e_q_rad (double *e, double *q)
{
  q[0] = cos (e[1] / 2) * cos ((e[0] + e[2]) / 2);
  q[1] = sin (e[1] / 2) * cos ((e[0] - e[2]) / 2);
  q[2] = sin (e[1] / 2) * sin ((e[0] - e[2]) / 2);
  q[3] = cos (e[1] / 2) * sin ((e[0] + e[2]) / 2);

  return;
}

void
ol_q_e (double *q, double *e)
{
  ol_q_e_rad (q, e);

  ol_e_rad2deg (e, e);

  return;
}

void
ol_q_e_rad (double *q, double *e)
{
  e[1] = 2 * atan2 (sqrt (q[1] * q[1] + q[2] * q[2]),
		    sqrt (q[0] * q[0] + q[3] * q[3]));

  if (fabs (e[1]) > OL_EPS_DEG && fabs (OL_PI - e[1]) > OL_EPS_DEG)
  {
    e[0] = atan2 (q[3], q[0]) + atan2 (q[2], q[1]);
    e[2] = atan2 (q[3], q[0]) - atan2 (q[2], q[1]);
  }
  else if (fabs (e[1]) < OL_EPS_DEG)
  {
    e[0] = 2 * atan2 (q[3], q[0]);
    e[2] = 0;
  }
  else
  {
    e[0] = 2 * atan2 (q[2], q[1]);
    e[2] = 0;
  }


  return;
}

void
ol_q_q (double *q1, double *q2)
{
  int i;

  ol_q_memcpy (q1, q2);

  for (i = 0; i < 4; i++)
    if (fabs (q2[i]) > OL_EPS)
    {
      if (q2[i] < 0)
	ut_array_1d_scale (q2, 4, -1);

      break;
    }

  return;
}

int
ol_q_fprintf (FILE * file, double *q, char *format)
{
  int i;

  for (i = 0; i <= 3; i++)
  {
    fprintf (file, format, q[i]);
    fprintf (file, " ");
  }
  return fprintf (file, "\n");

}

int
ol_q_fscanf (FILE * file, double *q)
{
  if (fscanf (file, "%lf%lf%lf%lf", &q[0], &q[1], &q[2], &q[3]) == 4)
    return 1;
  else
    return 0;
}
