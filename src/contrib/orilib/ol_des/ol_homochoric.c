/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_homochoric.h"

double *
ol_homochoric_alloc ()
{
  double *homochoric = ut_alloc_1d (3);

  return homochoric;
}

void
ol_homochoric_free (double *homochoric)
{
  if (!homochoric)
    return;
  else
    free (homochoric);

  return;
}

void
ol_homochoric_set_zero (double *homochoric)
{
  ut_array_1d_zero (homochoric, 3);

  return;
}

void
ol_homochoric_set_id (double *homochoric)
{
  ut_array_1d_zero (homochoric, 3);

  return;
}

void
ol_homochoric_set_this (double *homochoric,
			double homochoric1,
			double homochoric2,
			double homochoric3)
{
  homochoric[0] = homochoric1;
  homochoric[1] = homochoric2;
  homochoric[2] = homochoric3;

  return;
}


void
ol_homochoric_memcpy (double *homochoricsrc, double *homochoricdest)
{
  ut_array_1d_memcpy (homochoricdest, 3, homochoricsrc);

  return;
}

void
ol_rtheta_homochoric (double *r, double theta, double *homochoric)
{
  ol_rtheta_homochoric_rad (r, theta * OL_PI / 180, homochoric);

  return;
}

void
ol_rtheta_homochoric_rad (double *r, double theta, double *homochoric)
{
  int i;
  double norm = pow (0.75 * (theta - sin (theta)), 1. / 3);

  for (i = 0; i < 3; i++)
    homochoric[i] = r[i] * norm;

  return;
}

void
ol_homochoric_rtheta (struct FCT *phfct, double *homochoric, double *r, double *ptheta)
{
  ol_homochoric_rtheta_rad (phfct, homochoric, r, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_homochoric_q (struct FCT *phfct, double *homochoric, double *q)
{
  double theta, *r = ol_r_alloc ();

  ol_homochoric_rtheta (phfct, homochoric, r, &theta);
  ol_rtheta_q (r, theta, q);

  ol_r_free (r);

  return;
}

void
ol_homochoric_thetafct (struct FCT *phfct)
{
  int i;
  double *ptmp = NULL;

  ut_fct_set_numerical (phfct, 0, M_PI, 100, "bounds");

  for (i = 0; i < 100; i++)
    (*phfct).y[i] =
      pow (0.75 * ((*phfct).x[i] - sin ((*phfct).x[i])), 1. / 3);
  ptmp = (*phfct).x;
  (*phfct).x = (*phfct).y;
  (*phfct).y = ptmp;

  ut_fct_set_init_interp (phfct);

  return;
}

void
ol_homochoric_rtheta_rad (struct FCT* phfct, double *homochoric, double *r, double *ptheta)
{
  int i;
  double norm;

  norm = ut_array_1d_norm (homochoric, 3);

  ol_homochoric_theta_rad (phfct, homochoric, ptheta);

  if ((*ptheta) < OL_EPS)
  {
    r[0] = 1;
    r[1] = 0;
    r[2] = 0;
  }
  else
    for (i = 0; i < 3; i++)
      r[i] = homochoric[i] / norm;

  return;
}

void
ol_homochoric_r (double *homochoric, double *r)
{
  int i;
  double norm = ut_array_1d_norm (homochoric, 3);

  for (i = 0; i < 3; i++)
    r[i] = homochoric[i] / norm;

  return;
}

void
ol_homochoric_theta (struct FCT *hfct, double *homochoric, double *ptheta)
{
  ol_homochoric_theta_rad (hfct, homochoric, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_homochoric_theta_rad (struct FCT *phfct, double *homochoric, double *ptheta)
{
  if (phfct)
    (*ptheta) = ut_fct_eval (*phfct, ut_array_1d_norm (homochoric, 3));

  else
  {
    struct FCT hfct2;
    ut_fct_set_zero (&hfct2);
    ol_homochoric_thetafct (&hfct2);
    (*ptheta) = ut_fct_eval (hfct2, ut_array_1d_norm (homochoric, 3));
    ut_fct_free (&hfct2);
  }

  return;
}

void
ol_g_homochoric (double **g, double *homochoric)
{
  double theta, *r = ol_r_alloc ();

  ol_g_rtheta_rad (g, r, &theta);

  ol_rtheta_homochoric (r, theta, homochoric);

  ol_r_free (r);

  return;
}

void
ol_homochoric_g (struct FCT *phfct, double *homochoric, double **g)
{
  double theta, *r = ol_r_alloc ();

  ol_homochoric_rtheta (phfct, homochoric, r, &theta);

  ol_rtheta_g (r, theta, g);

  ol_r_free (r);

  return;
}

int
ol_homochoric_fscanf (FILE * file, double *homochoric)
{
  if (fscanf (file, "%lf%lf%lf", &homochoric[0], &homochoric[1], &homochoric[2]) == 3)
    return 1;
  else
    return 0;
}

int
ol_homochoric_fprintf (FILE * file, double *homochoric, char *format)
{
  fprintf (file, format, homochoric[0]);
  fprintf (file, " ");
  fprintf (file, format, homochoric[1]);
  fprintf (file, " ");
  fprintf (file, format, homochoric[2]);
  return fprintf (file, "\n");
}

void
ol_homochoric_e (struct FCT *phfct, double *homochoric, double *e)
{
  ol_homochoric_e_rad (phfct, homochoric, e);
  ol_e_rad2deg (e, e);

  return;
}

void
ol_homochoric_e_rad (struct FCT *phfct, double *homochoric, double *e)
{
  double *q = ol_q_alloc ();

  ol_homochoric_q (phfct, homochoric, q);

  ol_q_e_rad (q, e);

  ol_q_free (q);

  return;
}
