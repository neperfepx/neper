/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_rtheta.h"

double *
ol_r_alloc (void)
{
  double *r = ut_alloc_1d (3);

  r[0] = 1;

  return r;
}

void
ol_r_free (double **pr)
{
  ut_free_1d (pr);

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

double *
ol_rtheta_alloc (void)
{
  double *rtheta = ut_alloc_1d (4);

  rtheta[0] = 1;

  return rtheta;
}

void
ol_rtheta_free (double **prtheta)
{
  ut_free_1d (prtheta);

  return;
}

void
ol_rtheta_set_zero (double *rtheta)
{
  ut_array_1d_zero (rtheta, 4);

  return;
}

void
ol_rtheta_set_id (double *rtheta)
{
  rtheta[0] = 1;
  rtheta[1] = 0;
  rtheta[2] = 0;
  rtheta[3] = 0;

  return;
}

void
ol_rtheta_set_this (double *rtheta, double r1, double r2, double r3,
                    double theta)
{
  rtheta[0] = r1;
  rtheta[1] = r2;
  rtheta[2] = r3;
  rtheta[3] = theta;

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
ol_rtheta_memcpy (double *rthetasrc, double *rthetadest)
{
  ut_array_1d_memcpy (rthetasrc, 4, rthetadest);

  return;
}

int
ol_g_rtheta (double **g, double *rtheta)
{
  int status;

  status = ol_g_rtheta_rad (g, rtheta);
  ol_theta_rad2deg (rtheta[3], rtheta + 3);

  return status;
}

int
ol_g_r (double **g, double *r)
{
  int status;
  double rtheta[4];

  status = ol_g_rtheta_rad (g, rtheta);
  ol_r_memcpy (rtheta, r);

  return status;
}


int
ol_g_rtheta_rad (double **g, double *rtheta)
{
  int i, m;
  int status;
  double *r = rtheta, *ptheta = rtheta + 3;

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
    /* a symmetric matrix is not a rotation of angle in ]0, 180[ */
    if (ut_num_equal (g[1][2], g[2][1], OL_EPS)
        && ut_num_equal (g[2][0], g[0][2], OL_EPS)
        && ut_num_equal (g[0][1], g[1][0], OL_EPS))
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
ol_rtheta_g (double *rtheta, double **g)
{
  double rtheta2[4];

  ol_r_memcpy (rtheta, rtheta2);
  ol_theta_deg2rad (rtheta[3], rtheta2 + 3);
  ol_rtheta_g_rad (rtheta2, g);

  return;
}

void
ol_rtheta_g_rad (double *rtheta, double **g)
{
  double C, S, *r = rtheta;
  C = cos (rtheta[3]);
  S = sin (rtheta[3]);

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
ol_rtheta_fprintf (FILE * file, double *rtheta, char *format)
{
  fprintf (file, format, rtheta[0]);
  fprintf (file, " ");
  fprintf (file, format, rtheta[1]);
  fprintf (file, " ");
  fprintf (file, format, rtheta[2]);
  fprintf (file, " ");
  fprintf (file, format, rtheta[3]);
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
ol_rtheta_fscanf (FILE * file, double *rtheta)
{
  if (fscanf (file, "%lf%lf%lf%lf", &rtheta[0], &rtheta[1], &rtheta[2], &rtheta[3]) == 4)
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
ol_rtheta_rtheta_rad (double *rtheta, double *rtheta2)
{
  if (rtheta2 != rtheta)
    ol_rtheta_memcpy (rtheta, rtheta2);

  /* ranging theta within [0, 2 OL_PI[ */
  while (rtheta2[3] < 0)
    rtheta2[3] += 2 * OL_PI;
  while (rtheta2[3] >= 2 * OL_PI)
    rtheta2[3] -= 2 * OL_PI;

  /* ranging theta within [0, OL_PI]: a rotation of theta about r is a
     rotation of 2 OL_PI - theta about -r */
  if (rtheta2[3] > OL_PI)
  {
    rtheta2[3] = 2 * OL_PI - rtheta2[3];
    ut_array_1d_scale (rtheta2, 3, -1);
  }

  return;
}

void
ol_rtheta_rtheta (double *rtheta, double *rtheta2)
{
  ol_rtheta_memcpy (rtheta, rtheta2);
  ol_theta_deg2rad (rtheta2[3], rtheta2 + 3);
  ol_rtheta_rtheta_rad (rtheta2, rtheta2);
  ol_theta_rad2deg (rtheta2[3], rtheta2 + 3);

  return;
}
