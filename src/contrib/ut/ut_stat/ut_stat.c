/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"ut.h"

int
ut_stat_test_ks (double* cd1, double* cd2, int qty, double* pres)
{
  int i;

  (*pres) = 0;
  for (i = 0; i < qty; i++)
    (*pres) = ut_num_max (*pres, fabs (cd1[i] - cd2[i]));

  return 0;
}

int
ut_stat_test_kuiper (double* cd1, double* cd2, int qty, double* pres)
{
  int i;
  double Dp, Dm;

  Dp = 0;
  Dm = 0;
  for (i = 0; i < qty; i++)
  {
    if (cd1[i] > cd2[i])
      Dp = ut_num_max (Dp, cd1[i] - cd2[i]);
    else
      Dm = ut_num_max (Dm, - (cd1[i] - cd2[i]));
  }

  (*pres) = Dp + Dm;

  return 0;
}
