/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_nb_rtheta.h"

void
ol_nb_r (double n1, double n2, double *r)
{
  double alpha = ut_num_acos (2 * n1 - 1);
  double beta = 2 * OL_PI * n2;

  r[0] = sin (alpha) * cos (beta);
  r[1] = sin (alpha) * sin (beta);
  r[2] = cos (alpha);

  return;
}

void
ol_nb_max_theta_rad (double n3, double thetamax, double *ptheta)
{
  if (thetamax < 0)
    ut_print_message (2, 0, "Negative maximum rotation angle.\n");
  else if (thetamax > OL_PI)
    thetamax = OL_PI;

  (*ptheta) = thetamax * pow (n3, 1. / 3.);

  return;
}

void
ol_nb_max_theta (double n3, double thetamax, double *ptheta)
{
  ol_nb_max_theta_rad (n3, thetamax * OL_PI / 180, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_nb_max_rtheta_rad (double n1, double n2, double n3,
                      double thetamax, double *rtheta)
{
  ol_nb_r (n1, n2, rtheta);
  ol_nb_max_theta_rad (n3, thetamax, rtheta + 3);

  return;
}

void
ol_nb_max_rtheta (double n1, double n2, double n3,
                  double thetamax, double *rtheta)
{
  ol_nb_r (n1, n2, rtheta);
  ol_nb_max_theta (n3, thetamax, rtheta + 3);

  return;
}

void
ol_nb_max_theta_mis_rad (double n3, double thetamax, double *ptheta)
{
  if (thetamax < 0)
    ut_print_message (2, 0, "Negative maximum rotation angle.\n");
  else if (thetamax > OL_PI)
    thetamax = OL_PI;

  (*ptheta) = thetamax * n3;

  return;
}

void
ol_nb_max_theta_mis (double n3, double thetamax, double *ptheta)
{
  ol_nb_max_theta_mis_rad (n3, thetamax * OL_PI / 180, ptheta);
  ol_theta_rad2deg (*ptheta, ptheta);

  return;
}

void
ol_nb_max_rtheta_mis_rad (double n1, double n2, double n3,
                          double thetamax, double *rtheta)
{
  ol_nb_r (n1, n2, rtheta);
  ol_nb_max_theta_mis_rad (n3, thetamax, rtheta + 3);

  return;
}

void
ol_nb_max_rtheta_mis (double n1, double n2, double n3,
                      double thetamax, double *rtheta)
{
  ol_nb_r (n1, n2, rtheta);
  ol_nb_max_theta_mis (n3, thetamax, rtheta + 3);

  return;
}
