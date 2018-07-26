/* Copyright (C) 2003-2018, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_GSL
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>
#endif // HAVE_GSL

#ifndef UT_DATA_H
#define UT_DATA_H

extern void ut_data_fit_linear (double *x, double *y, int n, double *pc0,
				double *pc1, double **cov, double *psumsq);

#endif /* UT_DATA_H */

#ifdef __cplusplus
}
#endif
