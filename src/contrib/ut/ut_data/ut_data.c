/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include"ut.h"
#ifdef HAVE_GSL
#include<gsl/gsl_fit.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>
#include<gsl/gsl_vector.h>
#include<gsl/gsl_blas.h>
#include<gsl/gsl_multifit_nlin.h>

void
ut_data_fit_linear (double *x, double *y, int n, double *pc0, double *pc1,
		    double **cov, double *psumsq)
{
  double c0, c1, cov00, cov01, cov11, sumsq;

  gsl_fit_linear (x, 1, y, 1, n, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);

  if (pc0)
    (*pc0) = c0;

  if (pc1)
    (*pc1) = c1;

  if (cov)
  {
    cov[0][0] = cov00;
    cov[0][1] = cov01;
    cov[1][1] = cov11;
  }

  if (psumsq)
    (*psumsq) = sumsq;

  return;
}

struct data
{
  size_t n;
  double *x;
  double *y;
};

int
expb_f (const gsl_vector * x, void *data, gsl_vector * f)
{
  size_t n = ((struct data *) data)->n;
  double *data_y = ((struct data *) data)->y;
  double *data_x = ((struct data *) data)->x;

  double c0 = gsl_vector_get (x, 0);
  double c1 = gsl_vector_get (x, 1);
  double c2 = gsl_vector_get (x, 2);
  double Yi;

  size_t i;

  for (i = 0; i < n; i++)
  {
    /* Model Yi = c0 + c1 * t + c2 * t * t */
    Yi = c0 + c1 * data_x[i] + c2 * pow (data_x[i], 2);
    gsl_vector_set (f, i, (Yi - data_y[i]));
  }

  return GSL_SUCCESS;
}

int
expb_df (const gsl_vector * x, void *data, gsl_matrix * J)
{
  size_t n = ((struct data *) data)->n;
  size_t i;
  double *data_x = ((struct data *) data)->x;
  (void) x;
  // double c1 = gsl_vector_get (x, 1);
  // double c2 = gsl_vector_get (x, 2);

  for (i = 0; i < n; i++)
  {
    /* Jacobian matrix J(i,j) = dfi / dxj, */
    /* where fi = (Yi - yi)      */
    /*       Yi = c0 + ...       */
    /* and the xj are the parameters (c0,...) */
    gsl_matrix_set (J, i, 0, 1);
    gsl_matrix_set (J, i, 1, data_x[i]);
    gsl_matrix_set (J, i, 2, data_x[i] * data_x[i]);
  }
  return GSL_SUCCESS;
}

int
expb_fdf (const gsl_vector * x, void *data, gsl_vector * f, gsl_matrix * J)
{
  expb_f (x, data, f);
  expb_df (x, data, J);

  return GSL_SUCCESS;
}

#endif // HAVE_GSL
