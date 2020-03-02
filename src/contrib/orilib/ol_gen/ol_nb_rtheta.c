/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_nb_rtheta.h"

void
ol_nb_r (double n1, double n2, double *r)
{
  double alpha, beta;

  if (n1 < 0 || n1 > 1 || n2 < 0 || n2 > 1)
    abort ();

  if (n1 != 0)
  {
    alpha = acos (2 * n1 - 1);
    beta = 2 * OL_PI * n2;

    r[0] = sin (alpha) * cos (beta);
    r[1] = sin (alpha) * sin (beta);
    r[2] = cos (alpha);
  }

  else
    ut_array_1d_set_3 (r, 0, 0, 1);

  return;
}

void
ol_nb_max_theta_rad (double n4, double thetamax, double *ptheta)
{
  if (thetamax < 0)
    abort ();
  else if (thetamax > OL_PI)
    thetamax = OL_PI;

  (*ptheta) = thetamax * pow (n4, 1. / 3.);

  return;
}

void
ol_nb_max_theta (double n4, double thetamax, double *ptheta)
{
  ol_nb_max_theta_rad (n4, thetamax * OL_PI / 180, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_nb_max_rtheta_rad (double n1, double n2, double n3,
                      double thetamax, double *r, double *ptheta)
{
  ol_nb_r (n1, n2, r);
  ol_nb_max_theta_rad (n3, thetamax, ptheta);

  return;
}

void
ol_nb_max_rtheta (double n1, double n2, double n3,
                  double thetamax, double *r, double *ptheta)
{
  ol_nb_r (n1, n2, r);
  ol_nb_max_theta (n3, thetamax, ptheta);

  return;
}

void
ol_nb_max_theta_mis_rad (double n4, double thetamax, double *ptheta)
{
  if (thetamax < 0)
    abort ();
  else if (thetamax > OL_PI)
    thetamax = OL_PI;

  (*ptheta) = thetamax * n4;

  return;
}

void
ol_nb_max_theta_mis (double n4, double thetamax, double *ptheta)
{
  ol_nb_max_theta_mis_rad (n4, thetamax * OL_PI / 180, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_nb_max_rtheta_mis_rad (double n1, double n2, double n3,
                          double thetamax, double *r, double *ptheta)
{
  ol_nb_r (n1, n2, r);
  ol_nb_max_theta_mis_rad (n3, thetamax, ptheta);

  return;
}

void
ol_nb_max_rtheta_mis (double n1, double n2, double n3,
                      double thetamax, double *r, double *ptheta)
{
  ol_nb_r (n1, n2, r);
  ol_nb_max_theta_mis (n3, thetamax, ptheta);

  return;
}
