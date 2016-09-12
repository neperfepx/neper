/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_e.h"

double *
ol_e_alloc ()
{
  double *e = ut_alloc_1d (3);

  return e;
}

void
ol_e_free (double *e)
{
  if (!e)
    return;
  else
    free (e);

  return;
}

void
ol_e_set_zero (double *e)
{
  ut_array_1d_zero (e, 3);

  return;
}

void
ol_e_set_id (double *e)
{
  ut_array_1d_zero (e, 3);

  return;
}

void
ol_e_set_this (double *e, double phi1, double phi, double phi2)
{
  e[0] = phi1;
  e[1] = phi;
  e[2] = phi2;

  return;
}

void
ol_e_memcpy (double *esrc, double *edest)
{
  ut_array_1d_memcpy (edest, 3, esrc);

  return;
}

void
ol_g_e (double **g, double *e)
{
  ol_g_e_rad (g, e);

  ol_e_rad2deg (e, e);

  return;
}

void
ol_g_e_rad (double **g, double *e)
{
  e[1] = ut_num_acos (g[2][2]);

  if (fabs (e[1]) > OL_EPS_DEG && fabs (e[1] - OL_PI) > OL_EPS_DEG)
  {
    e[0] = atan2 (g[2][0], -g[2][1]);
    e[2] = atan2 (g[0][2], g[1][2]);
  }
  else
  {
    e[0] = atan2 (g[0][1], g[0][0]);
    e[2] = 0;
  }

  return;
}

void
ol_e_g (double *e, double **g)
{
  double *e2;

  e2 = ol_e_alloc ();
  ol_e_deg2rad (e, e2);

  ol_e_g_rad (e2, g);

  ol_e_free (e2);

  return;
}

void
ol_e_g_rad (double *e, double **g)
{
  g[0][0] = cos (e[0]) * cos (e[2]) - sin (e[0]) * sin (e[2]) * cos (e[1]);
  g[0][1] = sin (e[0]) * cos (e[2]) + cos (e[0]) * sin (e[2]) * cos (e[1]);
  g[0][2] = sin (e[2]) * sin (e[1]);

  g[1][0] = -cos (e[0]) * sin (e[2]) - sin (e[0]) * cos (e[2]) * cos (e[1]);
  g[1][1] = -sin (e[0]) * sin (e[2]) + cos (e[0]) * cos (e[2]) * cos (e[1]);
  g[1][2] = cos (e[2]) * sin (e[1]);

  g[2][0] = sin (e[0]) * sin (e[1]);
  g[2][1] = -cos (e[0]) * sin (e[1]);
  g[2][2] = cos (e[1]);

  return;
}

void
ol_e_e (double *e, double *e2)
{
  ol_e_deg2rad (e, e2);
  ol_e_e_rad (e2, e2);
  ol_e_rad2deg (e2, e2);

  return;
}

void
ol_e_e_rad (double *e, double *e2)
{
  int i;

  for (i = 0; i < 3; i++)
    e2[i] = e[i];

  /* ranging phi within [0,(2*OL_PI)] */
  e2[1] -= ((int) (e2[1] / (2 * OL_PI))) * (2 * OL_PI);
  if (e2[1] < 0)
    e2[1] += (2 * OL_PI);

  /* if phi>OL_PI, applying reflection */
  /* phi becomes within [0,OL_PI] */
  if (e2[1] > OL_PI)
  {
    e2[1] = (2 * OL_PI) - e2[1];
    e2[0] += OL_PI;
    e2[2] += OL_PI;
  }

  /* treating the std case where phi != 0 */
  if (fabs (e2[1]) > OL_EPS_DEG && fabs (e2[1] - OL_PI) > OL_EPS_DEG)
  {
    /* ranging phi2 within [0,(2*OL_PI)] */
    e2[2] -= ((int) (e2[2] / (2 * OL_PI))) * (2 * OL_PI);
    if (e2[2] < 0)
      e2[2] += (2 * OL_PI);
  }
  /* treating degeneracy: phi = 0: phi1 += phi2 and phi2 = 0. */
  else if (fabs (e2[1]) > OL_EPS_DEG)
  {
    e2[0] += e2[2];
    e2[2] = 0;
  }
  else				/* the same at phi = 180 */
  {
    e2[0] -= e2[2];
    e2[2] = 0;
  }

  /* ranging phi1 within [0,(2*OL_PI)] */
  e2[0] -= ((int) (e2[0] / (2 * OL_PI))) * (2 * OL_PI);
  if (e2[0] < 0)
    e2[0] += (2 * OL_PI);

  return;
}

void
ol_e_deg2rad (double *e, double *e2)
{
  int i;

  for (i = 0; i <= 2; i++)
    ol_theta_deg2rad (e[i], &(e2[i]));

  return;
}

void
ol_e_rad2deg (double *e, double *e2)
{
  int i;

  for (i = 0; i < 3; i++)
    ol_theta_rad2deg (e[i], &(e2[i]));

  return;
}

int
ol_e_fprintf (FILE * file, double *e, const char *format)
{
  fprintf (file, format, e[0]);
  fprintf (file, " ");
  fprintf (file, format, e[1]);
  fprintf (file, " ");
  fprintf (file, format, e[2]);
  return fprintf (file, "\n");
}

int
ol_e_fscanf (FILE * file, double *e)
{
  if (fscanf (file, "%lf%lf%lf", &e[0], &e[1], &e[2]) == 3)
    return 1;
  else
    return 0;
}

void
ol_e_R (double *e, double *R)
{
  double *q = ol_q_alloc ();

  ol_e_q (e, q);
  ol_q_R (q, R);

  ol_q_free (q);

  return;
}

void
ol_e_R_rad (double *e, double *R)
{
  double *q = ol_q_alloc ();

  ol_e_q_rad (e, q);
  ol_q_R (q, R);

  ol_q_free (q);

  return;
}

void
ol_e_rtheta (double *e, double *r, double *ptheta)
{
  double *q = ol_q_alloc ();

  ol_e_q (e, q);
  ol_q_rtheta (q, r, ptheta);

  ol_q_free (q);

  return;
}

void
ol_e_rtheta_rad (double *e, double *r, double *ptheta)
{
  double *q = ol_q_alloc ();

  ol_e_q_rad (e, q);
  ol_q_rtheta_rad (q, r, ptheta);

  ol_q_free (q);

  return;
}

void
ol_e_r (double *e, double *r)
{
  double *q = ol_q_alloc ();

  ol_e_q (e, q);
  ol_q_r (q, r);

  ol_q_free (q);

  return;
}

void
ol_e_theta_rad (double *e, double *ptheta)
{
  double *q = ol_q_alloc ();

  ol_e_q_rad (e, q);
  ol_q_theta_rad (q, ptheta);

  ol_q_free (q);

  return;
}

void
ol_e_theta (double *e, double *ptheta)
{
  ol_e_theta_rad (e, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_e_er_rad (double* e, double* er)
{
  er[0] = e[0] - OL_PI / 2.;
  er[1] = e[1];
  er[2] = e[2] + OL_PI / 2.;
  ol_e_e_rad (er, er);

  return;
}

void
ol_e_ek_rad (double* e, double* ek)
{
  ek[0] = e[0] - OL_PI / 2.;
  ek[1] = e[1];
  ek[2] = OL_PI / 2. - e[2];
  ol_e_e_rad (ek, ek);

  return;
}

void
ol_er_e_rad (double* er, double* e)
{
  e[0] = er[0] + OL_PI / 2.;
  e[1] = er[1];
  e[2] = er[2] - OL_PI / 2.;
  ol_e_e_rad (e, e);

  return;
}

void
ol_ek_e_rad (double* ek, double* e)
{
  e[0] = ek[0] + OL_PI / 2.;
  e[1] = ek[1];
  e[2] = OL_PI / 2. - ek[2];
  ol_e_e_rad (e, e);

  return;
}

void
ol_e_er (double* e, double* er)
{
  ol_e_deg2rad (e , er);
  ol_e_er_rad  (er, er);
  ol_e_rad2deg (er, er);

  return;
}

void
ol_e_ek (double* e, double* ek)
{
  ol_e_deg2rad (e , ek);
  ol_e_ek_rad  (ek, ek);
  ol_e_rad2deg (ek, ek);

  return;
}

void
ol_er_e (double* er, double* e)
{
  ol_e_deg2rad (er, e);
  ol_er_e_rad  (e , e);
  ol_e_rad2deg (e , e);

  return;
}

void
ol_ek_e (double* ek, double* e)
{
  ol_e_deg2rad (ek, e);
  ol_ek_e_rad  (e , e);
  ol_e_rad2deg (e , e);

  return;
}
