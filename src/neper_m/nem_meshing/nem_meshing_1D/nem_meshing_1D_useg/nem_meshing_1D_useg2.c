/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1D_useg_.h"

int
cl1_cl_pl_n (double cl1, double clt, double pl)
{
  return 1 + ceil (log (clt / cl1) / log (pl));
}

double
cl1_pl_n_l (double cl1, double pl, double n)
{
  return cl1 * (pow (pl, n - 1) - 1) / (pl - 1);
}

double
cl1_cl2_pl_I (double cl1, double cl2, double pl)
{
  return 0.5 * (1 + (pl / (pl - 1)) * (cl2 - cl1));
}

double
cl1_pl_i_x (double cl1, double pl, int i)
{
  return cl1 * (pow (pl, i - 1) - 1) / (pl - 1);
}

double
cl1_pl_i_cl (double cl1, double pl, int i)
{
  return cl1 * pow (pl, i - 1);
}

int
cl1_pl_x_i (double cl1, double pl, double x)
{
  return 1 + floor (log (1 + (pl - 1) * x / cl1) / log (pl));
}

void
nem_meshing_1d_useg_reverse (int ptqty, double *ucoo, double *ucl)
{
  int i;

  ut_array_1d_reverseelts (ucoo, ptqty);
  for (i = 0; i <= ptqty - 1; i++)
    ucoo[i] = 1 - ucoo[i];

  ut_array_1d_reverseelts (ucl, ptqty);

  return;
}
