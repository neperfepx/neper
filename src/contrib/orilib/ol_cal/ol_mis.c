/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_mis.h"

void
ol_g_lng (double** g, double** lng)
{
  double theta;

  ut_mat_skew (g, 3, lng);
  ol_g_theta_rad (g, &theta);
  if (theta != 0)
    ut_array_2d_scale (lng, 3, 3, theta / sin (theta));

  return;
}
