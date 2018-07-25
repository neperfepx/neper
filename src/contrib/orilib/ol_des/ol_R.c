/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_R.h"

double *
ol_R_alloc ()
{
  double *R = ut_alloc_1d (3);

  return R;
}

void
ol_R_free (double *R)
{
  if (!R)
    return;
  else
    free (R);

  return;
}

void
ol_R_set_zero (double *R)
{
  ut_array_1d_zero (R, 3);

  return;
}

void
ol_R_set_id (double *R)
{
  ut_array_1d_zero (R, 3);

  return;
}

void
ol_R_set_this (double *R,
double R1, 
double R2, 
double R3)
{
  R[0] = R1;
  R[1] = R2;
  R[2] = R3;

  return;
}


void
ol_R_memcpy (double *Rsrc, double *Rdest)
{
  ut_array_1d_memcpy (Rdest, 3, Rsrc);

  return;
}

void
ol_rtheta_R (double *r, double theta, double *R)
{
  ol_rtheta_R_rad (r, theta * OL_PI / 180, R);

  return;
}

void
ol_rtheta_R_rad (double *r, double theta, double *R)
{
  int i;

  for (i = 0; i <= 2; i++)
    R[i] = r[i] * tan (theta / 2);

  return;
}

void
ol_R_rtheta (double *R, double *r, double *ptheta)
{
  ol_R_rtheta_rad (R, r, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_R_rtheta_rad (double *R, double *r, double *ptheta)
{
  int i;
  double norm;

  norm = ut_array_1d_norm (R, 3);

  (*ptheta) = 2 * atan (norm);

  if ((*ptheta) < OL_EPS_DEG)
  {
    r[0] = 1;
    r[1] = 0;
    r[2] = 0;
  }
  else
    for (i = 0; i <= 2; i++)
      r[i] = R[i] / norm;

  return;
}

void
ol_R_r (double *R, double *r)
{
  double theta;

  ol_R_rtheta_rad (R, r, &theta);

  return;
}

void
ol_R_theta (double *R, double *ptheta)
{
  ol_R_theta_rad (R, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_R_theta_rad (double *R, double *ptheta)
{
  (*ptheta) = 2 * atan (ut_array_1d_norm (R, 3));

  return;
}

void
ol_g_R (double **g, double *R)
{
  int i;
  double *r;
  double theta;

  r = ol_r_alloc ();

  ol_g_rtheta_rad (g, r, &theta);

  for (i = 0; i <= 2; i++)
    R[i] = r[i] * tan (theta / 2);

  ol_r_free (r);

  return;
}

void
ol_R_g (double *R, double **g)
{
  int i;
  double norm;
  double *r;
  double theta;

  r = ol_r_alloc ();

  /* Calculation of the angle/axis pair from R */
  norm = ut_num_sqrt (R[0] * R[0] + R[1] * R[1] + R[2] * R[2]);
  theta = 2 * atan (norm);

  if (theta > 0)
    for (i = 0; i <= 2; i++)
      r[i] = R[i] / norm;
  else
    ol_R_set_this (r, 1, 0, 0);

  ol_rtheta_g_rad (r, theta, g);

  ol_r_free (r);

  return;
}

int
ol_R_fscanf (FILE * file, double *R)
{
  if (fscanf (file, "%lf%lf%lf", &R[0], &R[1], &R[2]) == 3)
    return 1;
  else
    return 0;
}

int
ol_R_fprintf (FILE * file, double *R, char *format)
{
  fprintf (file, format, R[0]);
  fprintf (file, " ");
  fprintf (file, format, R[1]);
  fprintf (file, " ");
  fprintf (file, format, R[2]);
  return fprintf (file, "\n");
}

void
ol_R_e (double *R, double *e)
{
  double *q = ol_q_alloc ();

  ol_R_q (R, q);
  ol_q_e (q, e);

  ol_q_free (q);

  return;
}

void
ol_R_e_rad (double *R, double *e)
{
  double *q = ol_q_alloc ();

  ol_R_q (R, q);
  ol_q_e_rad (q, e);

  ol_q_free (q);

  return;
}
