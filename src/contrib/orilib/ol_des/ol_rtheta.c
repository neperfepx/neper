/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_rtheta.h"

double *
ol_r_alloc ()
{
  double *r = ut_alloc_1d (3);

  r[0] = 1;

  return r;
}

void
ol_r_free (double *r)
{
  if (!r)
    return;
  else
    free (r);

  return;
}

void
ol_r_set_zero (double *r)
{
  ut_array_1d_zero (r, 3);

  return;
}

void
ol_r_set_id (double *r)
{
  r[0] = 1;
  r[1] = 0;
  r[2] = 0;

  return;
}

void
ol_r_set_unit (double *r)
{
  double norm = ut_vector_norm (r);
  ut_array_1d_scale (r, 3, 1 / norm);

  return;
}


void
ol_r_set_this (double *r, double r1, double r2, double r3)
{
  r[0] = r1;
  r[1] = r2;
  r[2] = r3;

  return;
}

void
ol_rtheta_set_zero (double *r, double *ptheta)
{
  ut_array_1d_zero (r, 3);
  (*ptheta) = 0;

  return;
}

void
ol_rtheta_set_id (double *r, double *ptheta)
{
  r[0] = 1;
  r[1] = 0;
  r[2] = 0;
  (*ptheta) = 0;

  return;
}

void
ol_rtheta_set_this (double *r, double *ptheta, double r1, double r2,
                    double r3, double thetaval)
{
  r[0] = r1;
  r[1] = r2;
  r[2] = r3;
  (*ptheta) = thetaval;

  return;
}

void
ol_theta_deg2rad (double theta, double *ptheta2)
{
  (*ptheta2) = theta * .01745329251994329576;   /* OL_PI/180 */

  return;
}

void
ol_theta_rad2deg (double theta, double *ptheta2)
{
  (*ptheta2) = theta * 57.29577951308232087721; /* 180/OL_PI */

  return;
}

void
ol_r_memcpy (double *rsrc, double *rdest)
{
  ut_array_1d_memcpy (rsrc, 3, rdest);

  return;
}

void
ol_rtheta_memcpy (double *rsrc, double thetasrc, double *rdest,
                  double *pthetadest)
{
  ut_array_1d_memcpy (rsrc, 3, rdest);
  (*pthetadest) = thetasrc;

  return;
}

int
ol_g_rtheta (double **g, double *r, double *ptheta)
{
  int status;

  status = ol_g_rtheta_rad (g, r, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return status;
}

int
ol_g_r (double **g, double *r)
{
  double theta;

  return ol_g_rtheta_rad (g, r, &theta);
}


int
ol_g_rtheta_rad (double **g, double *r, double *ptheta)
{
  int i, m;
  int status;

  (*ptheta) = ut_num_acos ((g[0][0] + g[1][1] + g[2][2] - 1) / 2);

  status = 0;
  if ((*ptheta) < OL_EPS_RAD)
  {
    r[0] = 1;
    r[1] = 0;
    r[2] = 0;
  }
  else if ((*ptheta) < (1 - OL_EPS_RAD) * OL_PI)
  {
    if (ut_num_equal (g[1][2], g[2][1], OL_EPS) != 1
        || ut_num_equal (g[1][2], g[2][1], OL_EPS) != 1
        || ut_num_equal (g[1][2], g[2][1], OL_EPS) != 1)
      status = -1;

    r[0] = (g[1][2] - g[2][1]) / (2 * sin (*ptheta));
    r[1] = (g[2][0] - g[0][2]) / (2 * sin (*ptheta));
    r[2] = (g[0][1] - g[1][0]) / (2 * sin (*ptheta));
  }
  else
  {
    r[0] = ut_num_sqrt ((g[0][0] + 1) / 2);
    r[1] = ut_num_sqrt ((g[1][1] + 1) / 2);
    r[2] = ut_num_sqrt ((g[2][2] + 1) / 2);

    m = ut_array_1d_max_index (r, 3);
    for (i = 0; i < 3; i++)
      if (i != m)
        r[i] *= ut_num_sgn (g[i][m]);
  }

  ol_r_set_unit (r);

  return status;                // -1;
}

void
ol_g_theta (double **g, double *ptheta)
{
  ol_g_theta_rad (g, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_g_theta_rad (double **g, double *ptheta)
{
  (*ptheta) = ut_num_acos ((g[0][0] + g[1][1] + g[2][2] - 1) / 2);

  return;
}

void
ol_rtheta_g (double *r, double theta, double **g)
{
  ol_rtheta_g_rad (r, theta * OL_PI / 180, g);

  return;
}

void
ol_rtheta_g_rad (double *r, double theta, double **g)
{
  double C, S;
  C = cos (theta);
  S = sin (theta);

  g[0][0] = r[0] * r[0] * (1 - C) + C;
  g[0][1] = r[0] * r[1] * (1 - C) + r[2] * S;
  g[0][2] = r[0] * r[2] * (1 - C) - r[1] * S;

  g[1][0] = r[1] * r[0] * (1 - C) - r[2] * S;
  g[1][1] = r[1] * r[1] * (1 - C) + C;
  g[1][2] = r[1] * r[2] * (1 - C) + r[0] * S;

  g[2][0] = r[2] * r[0] * (1 - C) + r[1] * S;
  g[2][1] = r[2] * r[1] * (1 - C) - r[0] * S;
  g[2][2] = r[2] * r[2] * (1 - C) + C;

  return;
}

int
ol_rtheta_fprintf (FILE * file, double *r, double theta, char *format)
{
  fprintf (file, format, r[0]);
  fprintf (file, " ");
  fprintf (file, format, r[1]);
  fprintf (file, " ");
  fprintf (file, format, r[2]);
  fprintf (file, " ");
  fprintf (file, format, theta);
  return fprintf (file, "\n");
}

int
ol_r_fprintf (FILE * file, double *r, char *format)
{
  fprintf (file, format, r[0]);
  fprintf (file, " ");
  fprintf (file, format, r[1]);
  fprintf (file, " ");
  fprintf (file, format, r[2]);
  return fprintf (file, "\n");

}

int
ol_theta_fprintf (FILE * file, double theta, char *format)
{
  fprintf (file, format, theta);
  return fprintf (file, "\n");
}

int
ol_rtheta_fscanf (FILE * file, double *r, double *ptheta)
{
  if (fscanf (file, "%lf%lf%lf%lf", &r[0], &r[1], &r[2], ptheta) == 4)
    return 1;
  else
    return 0;
}

int
ol_r_fscanf (FILE * file, double *r)
{
  if (fscanf (file, "%lf%lf%lf", &r[0], &r[1], &r[2]) == 3)
    return 1;
  else
    return 0;
}

int
ol_theta_fscanf (FILE * file, double *ptheta)
{
  if (fscanf (file, "%lf", ptheta) == 1)
    return 1;
  else
    return 0;
}

void
ol_rtheta_rtheta_rad (double *r1, double theta, double *r2, double *ptheta2)
{
  ol_r_memcpy (r1, r2);

  (*ptheta2) = theta;

  /* ranging theta within [0,(2*OL_PI)] */
  while (*ptheta2 < 0)
    *ptheta2 += 2 * OL_PI;
  while (*ptheta2 > 2 * OL_PI)
    *ptheta2 -= 2 * OL_PI;

  /* ranging theta within [0, OL_PI] */
  if ((*ptheta2) > OL_PI)
  {
    (*ptheta2) -= OL_PI;
    ut_array_1d_scale (r2, 3, -1);
  }

  return;
}

void
ol_rtheta_rtheta (double *r1, double theta, double *r2, double *ptheta2)
{
  ol_rtheta_rtheta_rad (r1, theta * OL_PI / 180, r2, ptheta2);
  ol_theta_rad2deg (*ptheta2, ptheta2);

  return;
}

void
ol_rtheta_e (double *r, double theta, double *e)
{
  double *q = ol_q_alloc ();

  ol_rtheta_q (r, theta, q);
  ol_q_e (q, e);

  ol_q_free (q);
}

void
ol_rtheta_e_rad (double *r, double theta, double *e)
{
  double *q = ol_q_alloc ();

  ol_rtheta_q_rad (r, theta, q);
  ol_q_e_rad (q, e);

  ol_q_free (q);
}
