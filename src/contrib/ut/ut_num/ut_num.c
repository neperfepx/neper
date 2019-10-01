/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<math.h>
#include<float.h>
#include"ut.h"

double
ut_num_atan2 (double s, double c)
{
  return atan2 (s, c);
}

int
ut_num_d2ri (double d)
{
  if (d - floor (d) < 0.49999999999999)
    return (int) floor (d);
  else
    return (int) ceil (d);
}

int
ut_num_fabs_int (int d)
{
  return d * ut_num_sgn_int (d);
}

int
ut_num_min_int (int n1, int n2)
{
  return ((n1 < n2) ? n1 : n2);
}

int
ut_num_max_int (int n1, int n2)
{
  return ((n1 > n2) ? n1 : n2);
}

int
ut_num_bound_int (int n, int min, int max)
{
  if (n < min)
    return min;
  else if (n > max)
    return max;
  else
    return n;
}

double
ut_num_min (double n1, double n2)
{
  return ((n1 < n2) ? n1 : n2);
}

double
ut_num_max (double n1, double n2)
{
  return ((n1 > n2) ? n1 : n2);
}

double
ut_num_bound (double n, double min, double max)
{
  if (n < min)
    return min;
  else if (n > max)
    return max;
  else
    return n;
}

int
ut_num_tenlen (double num)
{
  int i;

  for (i = 0; i <= 8; i++)
    if ((double) num >= pow (10, i) && (double) num < pow (10, i + 1))
      return i;

  return -1;
}

int
ut_num_tenlen_int (int num)
{
  int length;

  if (num == 0)
    length = 1;
  else
    length = 1 + log (fabs (num + 1e-6)) / log (10);

  return length;
}

double
ut_num_acos (double nb)
{
  double res;

  res = acos (nb);

  if (res != res) // NaN
  {
    if (nb > 1)			/* (1 can be slightly >1) */
      res = 0;
    else if (nb < -1)		/* the same */
      res = M_PI;
  }

  return res;
}

double
ut_num_sqrt (double nb)
{
  if (nb < 0)			/* (0 can be slightly <0) */
    return 0;
  else
    return sqrt (nb);
}

double
ut_num_sgn (double num)
{
  if (num >= 0)
    return 1;
  else
    return -1;
}

int
ut_num_sgn_int (int num)
{
  if (num >= 0)
    return 1;
  else
    return -1;
}

int
ut_num_rotpos (int beg, int end, int pos, int near)
{
  pos += near;

  if (end < beg)
    abort ();

  while (pos > end)
    pos -= (end - beg + 1);

  while (pos < beg)
    pos += (end - beg + 1);

  return pos;
}

int
ut_num_equal_int (int n1, int n2)
{
  if (n1 == n2)
    return 1;
  else
    return 0;
}

int
ut_num_equal_double (double n1, double n2, double eps)
{
  return ut_num_equal (n1, n2, eps);
}

int
ut_num_equal (double n1, double n2, double eps)
{
  if (fabs (n1 - n2) < eps)
    return 1;
  else
    return 0;
}

int
ut_num_requal (double n1, double n2, double eps)
{
  if (n1 == 0 && n2 == 0)
    return 1;
  else if (ut_num_equal (2 * fabs (n1 - n2) / (n1 + n2), 0, eps))
    return 1;
  else
    return 0;
}

int
ut_num_lcomden (int n1, int n2)
{
  int i, test, testop, res;
  int *n1den = ut_alloc_1d_int (ut_num_fabs_int (n1) + 1);
  int *n2den = ut_alloc_1d_int (ut_num_fabs_int (n2) + 1);

  n1den[0] = 0;
  if (n1 >= 0)
  {
    for (i = 2; i <= n1; i++)
      if (n1 % i == 0)
      {
	n1den[0]++;
	n1den[n1den[0]] = i;
      }
  }
  else
  {
    for (i = -2; i >= n1; i--)
      if (n1 % i == 0)
      {
	n1den[0]++;
	n1den[n1den[0]] = i;
      }
  }


  n2den[0] = 0;
  if (n2 >= 0)
  {
    for (i = 1; i <= n2; i++)
      if (n2 % i == 0)
      {
	n2den[0]++;
	n2den[n2den[0]] = i;
      }
  }
  else
  {
    for (i = -1; i >= n2; i--)
      if (n2 % i == 0)
      {
	n2den[0]++;
	n2den[n2den[0]] = i;
      }
  }

  res = 1;
  for (i = n2den[0]; i > 0; i--)
  {
    test = ut_array_1d_int_find (n1den + 1, n1den[0], n2den[i]);
    testop = ut_array_1d_int_find (n1den + 1, n1den[0], -n2den[i]);
    if (test != -1)
    {
      res = n1den[test + 1];
      break;
    }
    if (testop != -1)
    {
      res = n1den[testop + 1];
      break;
    }
  }

  ut_free_1d_int (n1den);
  ut_free_1d_int (n2den);

  return res;
}

/* this routine read the number wrote into string with a comma decimal
 * delimiter. It does change string. */
double
ut_num_readwcomma (char *string)
{
  double num;

  ut_string_fnr (string, ',', '.');
  sscanf (string, "%lf", &num);

  return num;
}

int
ut_num_even (int num)
{
  if (num / 2 == (num + 1) / 2)
    return 1;
  else
    return 0;
}

int
ut_num_odd (int num)
{
  if (num / 2 != (num + 1) / 2)
    return 1;
  else
    return 0;
}

void
ut_num_switch (double* pa, double* pb)
{
  double tmp = (*pa);
  (*pa) = (*pb);
  (*pb) = tmp;

  return;
}

void
ut_num_switch_int (int* pa, int* pb)
{
  int tmp = (*pa);
  (*pa) = (*pb);
  (*pb) = tmp;

  return;
}

double
ut_num_percent (double a, double b)
{
  return 100 * a / b;
}

double
ut_num_log (double a)
{
  if (a > 0)
    return log (a);
  else if (a > -1e-9)
    return -DBL_MAX;
  else
    abort ();
}

double
ut_num_reverseendian (double data)
{
  unsigned int i;
  double result;
  const char* datac = (const char *) &data;

  char *dest = (char *)&result;

  for(i = 0; i < sizeof (double); i++)
     dest[i] = datac[sizeof (double) - i - 1];

  return result;
}
