/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"ut.h"

int
ut_stat_test_ks (double *dis1, double *dis2, int qty, double *pres)
{
  int i;

  (*pres) = 0;
  for (i = 0; i < qty; i++)
    (*pres) = ut_num_max (*pres, fabs (dis1[i] - dis2[i]));

  return 0;
}

int
ut_stat_test_kuiper (double *dis1, double *dis2, int qty, double *pres)
{
  int i;
  double Dp, Dm;

  Dp = 0;
  Dm = 0;
  for (i = 0; i < qty; i++)
  {
    if (dis1[i] > dis2[i])
      Dp = ut_num_max (Dp, dis1[i] - dis2[i]);
    else
      Dm = ut_num_max (Dm, -(dis1[i] - dis2[i]));
  }

  (*pres) = Dp + Dm;

  return 0;
}

void
ut_stat_distribparam (double *data, int n, double step, double *pfirst,
                      int *plength)
{
  double min, max, last;

  min = ut_array_1d_min (data, n);
  max = ut_array_1d_max (data, n);

  (*pfirst) = step * floor (min / step);
  last = step * floor (max / step);

  (*plength) = 1 + ut_num_d2ri ((last - (*pfirst)) / step);

  return;
}

void
ut_stat_distrib (double *data, int n, double step, double first, int length,
                 double *distrib)
{
  int i, nb;

  ut_array_1d_zero (distrib, length);
  for (i = 0; i < n; i++)
  {
    nb = (int) ((data[i] - first - .5 * step) / step);
    if (nb >= 0 && nb < length)
      distrib[nb]++;
  }

  for (i = 0; i < length; i++)
    distrib[i] /= (step * (double) n);

  return;
}

void
ut_stat_scaletopercent_int (int *array, int size, int *percent)
{
  int i, sum;

  sum = ut_array_1d_int_sum (array, size);

  for (i = 0; i < size; i++)
    percent[i] = ut_num_d2ri (100 * (double) array[i] / sum);

  sum = ut_array_1d_int_sum (percent, size);

  if (sum != 100)
    percent[ut_array_1d_int_max_index (percent, size)] += (100 - sum);

  return;
}
