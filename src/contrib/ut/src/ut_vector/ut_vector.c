/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"ut.h"

double
ut_vector_norm (double *a)
{
  return sqrt (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

double
ut_vector_norm_squared (double *a)
{
  return a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
}

double
ut_vector_int_norm (int *a)
{
  return sqrt ((double) a[0] * (double) a[0] + (double) a[1] * (double) a[1] +
               (double) a[2] * (double) a[2]);
}

double
ut_vector_scalprod (double *a, double *b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void
ut_vector_vectprod (double *a, double *b, double *c)
{
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];

  return;
}

double
ut_vector_angle_rad (double *a, double *b)
{
  double angle;

  angle = ut_vector_scalprod (a, b);
  angle /= ut_vector_norm (a);
  angle /= ut_vector_norm (b);
  angle = ut_num_acos (angle);

  return angle;
}

double
ut_vector_angle (double *a, double *b)
{
  return (180 / M_PI) * ut_vector_angle_rad (a, b);
}

double
ut_vector_angle_rad_2d (double *a, double *b)
{
  double angle;

  angle = ut_array_1d_scalprod (a, b, 2);
  angle /= ut_array_1d_norm (a, 2);
  angle /= ut_array_1d_norm (b, 2);
  angle = ut_num_acos (angle);

  return angle;
}

double
ut_vector_angle_2d (double *a, double *b)
{
  return (180 / M_PI) * ut_vector_angle_rad_2d (a, b);
}

double
ut_vector_int_angle_rad (int *a, int *b)
{
  int i;
  double angle;
  double *a2 = ut_alloc_1d (3);
  double *b2 = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    a2[i] = (double) a[i];

  for (i = 0; i < 3; i++)
    b2[i] = (double) b[i];

  angle = ut_vector_scalprod (a2, b2);
  angle /= ut_vector_norm (a2);
  angle /= ut_vector_norm (b2);
  angle = acos (angle);
  if (angle != angle)           // NaN
    angle = 0;

  ut_free_1d (&a2);
  ut_free_1d (&b2);

  return angle;
}

double
ut_vector_int_angle (int *a, int *b)
{
  return (180 / M_PI) * ut_vector_int_angle_rad (a, b);
}

void
ut_vector_set_covar (double **v, int qty, int dim, double **covar)
{
  int i, j, k;

  ut_array_2d_zero (covar, dim, dim);

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      for (k = 0; k < qty; k++)
        covar[i][j] += v[k][i] * v[k][j];

  ut_array_2d_scale (covar, dim, dim, 1. / qty);

  return;
}

void
ut_vector_set_eigen (double **v, int qty, int dim, double **evect,
                     double *eval)
{
  double **covar = ut_alloc_2d (dim, dim);

  ut_vector_set_covar (v, qty, dim, covar);
  ut_mat_eigen (covar, dim, eval, evect);

  ut_free_2d (&covar, dim);

  return;
}

void
ut_vector_uvect (double *v, double *uvect)
{
  ut_array_1d_memcpy (v, 3, uvect);
  ut_array_1d_scale (uvect, 3, 1. / ut_array_1d_norm (uvect, 3));

  return;
}
