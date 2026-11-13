/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_homochoric.h"

#ifdef HAVE_GSL

// conversions between rotation axis / angle pairs and homochoric vectors,
// used internally by the conversions from / to rotation matrices and
// quaternions
static void ol_rtheta_homochoric_rad (double *, double *);
static void ol_homochoric_rtheta (struct FCT *, double *, double *);
static void ol_homochoric_rtheta_rad (struct FCT *, double *, double *);
static void ol_homochoric_theta_rad (struct FCT *, double *, double *);

double *
ol_homochoric_alloc (void)
{
  double *homochoric = ut_alloc_1d (3);

  return homochoric;
}

void
ol_homochoric_free (double **phomochoric)
{
  ut_free_1d (phomochoric);

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
                        double homochoric2, double homochoric3)
{
  homochoric[0] = homochoric1;
  homochoric[1] = homochoric2;
  homochoric[2] = homochoric3;

  return;
}


void
ol_homochoric_memcpy (double *homochoricsrc, double *homochoricdest)
{
  ut_array_1d_memcpy (homochoricsrc, 3, homochoricdest);

  return;
}

static void
ol_rtheta_homochoric_rad (double *rtheta, double *homochoric)
{
  int i;
  double norm = pow (0.75 * (rtheta[3] - sin (rtheta[3])), 1. / 3);

  for (i = 0; i < 3; i++)
    homochoric[i] = rtheta[i] * norm;

  return;
}

static void
ol_homochoric_rtheta (struct FCT *phfct, double *homochoric, double *rtheta)
{
  ol_homochoric_rtheta_rad (phfct, homochoric, rtheta);
  ol_theta_rad2deg (rtheta[3], rtheta + 3);

  return;
}

void
ol_homochoric_q (struct FCT *phfct, double *homochoric, double *q)
{
  double *rtheta = ol_rtheta_alloc ();

  ol_homochoric_rtheta (phfct, homochoric, rtheta);
  ol_rtheta_q (rtheta, q);

  ol_rtheta_free (&rtheta);

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

  ut_fct_init_interp (phfct);

  return;
}

static void
ol_homochoric_rtheta_rad (struct FCT *phfct, double *homochoric,
                          double *rtheta)
{
  int i;
  double norm;

  norm = ut_array_1d_norm (homochoric, 3);

  ol_homochoric_theta_rad (phfct, homochoric, rtheta + 3);

  if (rtheta[3] < OL_EPS)
  {
    rtheta[0] = 1;
    rtheta[1] = 0;
    rtheta[2] = 0;
  }
  else
    for (i = 0; i < 3; i++)
      rtheta[i] = homochoric[i] / norm;

  return;
}

static void
ol_homochoric_theta_rad (struct FCT *phfct, double *homochoric,
                         double *ptheta)
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
  double *rtheta = ol_rtheta_alloc ();

  ol_g_rtheta_rad (g, rtheta);

  ol_rtheta_homochoric_rad (rtheta, homochoric);

  ol_rtheta_free (&rtheta);

  return;
}

void
ol_q_homochoric (double *q, double *homochoric)
{
  double *rtheta = ol_rtheta_alloc ();

  ol_q_rtheta_rad (q, rtheta);

  ol_rtheta_homochoric_rad (rtheta, homochoric);

  ol_rtheta_free (&rtheta);

  return;
}

void
ol_homochoric_g (struct FCT *phfct, double *homochoric, double **g)
{
  double *rtheta = ol_rtheta_alloc ();

  ol_homochoric_rtheta (phfct, homochoric, rtheta);

  ol_rtheta_g (rtheta, g);

  ol_rtheta_free (&rtheta);

  return;
}

int
ol_homochoric_fscanf (FILE * file, double *homochoric)
{
  if (fscanf
      (file, "%lf%lf%lf", &homochoric[0], &homochoric[1],
       &homochoric[2]) == 3)
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

#endif // HAVE_GSL
