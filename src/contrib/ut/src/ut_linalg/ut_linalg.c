/* Copyright (C) 2003-2026, Romain Quey, CNRS.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"ut.h"

#ifdef HAVE_GSL
#include<gsl/gsl_linalg.h>

// Solve a small linear system (size <= UT_LINALG_SMALL) by Gaussian
// elimination with partial pivoting, without allocating memory.
#define UT_LINALG_SMALL 4

static int
ut_linalg_solve_LU_small (double **A, double *B, int size, double *X)
{
  int i, j, k, piv;
  double a[UT_LINALG_SMALL][UT_LINALG_SMALL], b[UT_LINALG_SMALL];
  double tmp, fact, det = 1;

  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
      a[i][j] = A[i][j];
    b[i] = B[i];
  }

  for (k = 0; k < size; k++)
  {
    // pivoting: largest absolute value in column k
    piv = k;
    for (i = k + 1; i < size; i++)
      if (fabs (a[i][k]) > fabs (a[piv][k]))
        piv = i;

    if (piv != k)
    {
      for (j = k; j < size; j++)
      {
        tmp = a[k][j];
        a[k][j] = a[piv][j];
        a[piv][j] = tmp;
      }
      tmp = b[k];
      b[k] = b[piv];
      b[piv] = tmp;
      det = -det;
    }

    det *= a[k][k];

    // elimination (if the pivot is 0, the system is singular and this is
    // caught by the determinant test below)
    if (a[k][k] != 0)
      for (i = k + 1; i < size; i++)
      {
        fact = a[i][k] / a[k][k];
        for (j = k + 1; j < size; j++)
          a[i][j] -= fact * a[k][j];
        b[i] -= fact * b[k];
      }
  }

  if (fabs (det) < 1e-15)
    return -1;

  // back substitution
  for (i = size - 1; i >= 0; i--)
  {
    tmp = b[i];
    for (j = i + 1; j < size; j++)
      tmp -= a[i][j] * X[j];
    X[i] = tmp / a[i][i];
  }

  return 0;
}

int
ut_linalg_solve_LU (double **A, double *B, int size, double *X)
{
  int i, j, status;

  if (size <= UT_LINALG_SMALL)
    return ut_linalg_solve_LU_small (A, B, size, X);

  gsl_matrix *Agsl = NULL;
  gsl_vector *Bgsl = NULL;
  gsl_vector *Xgsl = NULL;
  gsl_permutation *pgsl = NULL;
  int s;

  Agsl = gsl_matrix_alloc (size, size);
  Bgsl = gsl_vector_alloc (size);
  Xgsl = gsl_vector_alloc (size);
  pgsl = gsl_permutation_alloc (size);

  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++)
      gsl_matrix_set (Agsl, i, j, A[i][j]);

  for (j = 0; j < size; j++)
    gsl_vector_set (Bgsl, j, B[j]);

  gsl_linalg_LU_decomp (Agsl, pgsl, &s);

  status = 0;
  if (fabs (gsl_linalg_LU_det (Agsl, s)) < 1e-15)
    status = -1;
  else
  {
    gsl_linalg_LU_solve (Agsl, pgsl, Bgsl, Xgsl);

    for (j = 0; j < size; j++)
      X[j] = gsl_vector_get (Xgsl, j);
  }

  gsl_matrix_free (Agsl);
  gsl_vector_free (Bgsl);
  gsl_vector_free (Xgsl);
  gsl_permutation_free (pgsl);

  return status;
}
#endif
