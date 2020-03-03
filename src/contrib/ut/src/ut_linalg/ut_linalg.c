/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"ut.h"

#ifdef HAVE_GSL
#include<gsl/gsl_linalg.h>

int
ut_linalg_solve_LU (double **A, double *B, int size, double *X)
{
  int i, j, status;

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
