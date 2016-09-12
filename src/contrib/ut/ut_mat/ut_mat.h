/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_MAT_H
#define UT_MAT_H

extern int ut_mat_product (double **, int, int, double **, int, int,
			   double **);
extern int ut_mat_product_33 (double **, double **, double **);
extern double ut_mat_ddot_33 (double **, double **);

extern int ut_mat_vect_product (double **, int, int, double *, int, double *);

extern double ut_mat_det_22 (double **);
extern double ut_mat_det_33 (double **);
extern int ut_mat_inverse_22 (double **, double **);

extern void ut_mat_eigen (int, double **, double *, double **);
extern void ut_mat_transpose (double **, int, int, double **);
extern void ut_mat_trace (double**, int, double*);

extern int  ut_mat_inverse (double**, int, double**);
extern int ut_mat_svd (double** mat, int M, int N, double** U, double** s, double** V);
extern void  ut_mat_sym (double**, int, double**);
extern void  ut_mat_skew (double**, int, double**);

#endif /* UT_MAT_H */

#ifdef __cplusplus
}
#endif
