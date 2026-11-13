/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_R.h"

double *
ol_R_alloc (void)
{
  double *R = ut_alloc_1d (3);

  return R;
}

void
ol_R_free (double **pR)
{
  ut_free_1d (pR);

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
ol_R_set_this (double *R, double R1, double R2, double R3)
{
  R[0] = R1;
  R[1] = R2;
  R[2] = R3;

  return;
}


void
ol_R_memcpy (double *Rsrc, double *Rdest)
{
  ut_array_1d_memcpy (Rsrc, 3, Rdest);

  return;
}

void
ol_rtheta_R (double *rtheta, double *R)
{
  int i;

  for (i = 0; i <= 2; i++)
    R[i] = rtheta[i] * tan (rtheta[3] * OL_PI / 360);

  return;
}

void
ol_rtheta_R_rad (double *rtheta, double *R)
{
  int i;

  for (i = 0; i <= 2; i++)
    R[i] = rtheta[i] * tan (rtheta[3] / 2);

  return;
}

void
ol_R_rtheta (double *R, double *rtheta)
{
  ol_R_rtheta_rad (R, rtheta);
  ol_theta_rad2deg (rtheta[3], rtheta + 3);

  return;
}

void
ol_R_rtheta_rad (double *R, double *rtheta)
{
  int i;
  double norm;

  norm = ut_array_1d_norm (R, 3);

  rtheta[3] = 2 * atan (norm);

  if (rtheta[3] < OL_EPS_RAD)
  {
    rtheta[0] = 1;
    rtheta[1] = 0;
    rtheta[2] = 0;
  }
  else
    for (i = 0; i <= 2; i++)
      rtheta[i] = R[i] / norm;

  return;
}

void
ol_g_R (double **g, double *R)
{
  double *rtheta = ol_rtheta_alloc ();

  ol_g_rtheta_rad (g, rtheta);
  ol_rtheta_R_rad (rtheta, R);

  ol_rtheta_free (&rtheta);

  return;
}

void
ol_R_g (double *R, double **g)
{
  double *rtheta = ol_rtheta_alloc ();

  ol_R_rtheta_rad (R, rtheta);
  ol_rtheta_g_rad (rtheta, g);

  ol_rtheta_free (&rtheta);

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
