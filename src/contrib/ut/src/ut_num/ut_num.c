/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<math.h>
#include<float.h>
#include"ut.h"

double
ut_num_min (double num1, double num2)
{
  return ((num1 < num2) ? num1 : num2);
}

int
ut_num_min_int (int num1, int num2)
{
  return ((num1 < num2) ? num1 : num2);
}

double
ut_num_max (double num1, double num2)
{
  return ((num1 > num2) ? num1 : num2);
}

int
ut_num_max_int (int num1, int num2)
{
  return ((num1 > num2) ? num1 : num2);
}

int
ut_num_d2ri (double d)
{
  return (d - floor (d) <
          0.49999999999999) ? (int) floor (d) : (int) ceil (d);
}

int
ut_num_fabs_int (int d)
{
  return d * ut_num_sgn_int (d);
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
ut_num_bound (double n, double min, double max)
{
  if (n < min)
    return min;
  else if (n > max)
    return max;
  else
    return n;
}

// dirty implementation, but let's keep that for backward compatibility
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

  if (res != res)               // NaN
  {
    if (nb > 1)                 /* (1 can be slightly >1) */
      res = 0;
    else if (nb < -1)           /* the same */
      res = M_PI;
  }

  return res;
}

double
ut_num_sqrt (double nb)
{
  return (nb < 0) ? 0 : sqrt (nb);
}

int
ut_num_sgn (double num)
{
  return num >= 0 ? 1 : -1;
}

int
ut_num_sgn_int (int num)
{
  return num >= 0 ? 1 : -1;
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

double
ut_num_readwcomma (char *string)
{
  double num;
  char *string2 = NULL;

  ut_string_string (string, &string2);

  ut_string_fnr (string2, ',', '.');
  sscanf (string2, "%lf", &num);

  ut_free_1d_char (&string2);

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
ut_num_switch (double *pa, double *pb)
{
  double tmp = (*pa);
  (*pa) = (*pb);
  (*pb) = tmp;

  return;
}

void
ut_num_switch_int (int *pa, int *pb)
{
  int tmp = (*pa);
  (*pa) = (*pb);
  (*pb) = tmp;

  return;
}

double
ut_num_log (double a)
{
  return (a > 0) ? log (a) : -DBL_MAX;
}

double
ut_num_reverseendian (double data)
{
  unsigned int i;
  double result;
  const char *datac = (const char *) &data;

  char *dest = (char *) &result;

  for (i = 0; i < sizeof (double); i++)
    dest[i] = datac[sizeof (double) - i - 1];

  return result;
}
