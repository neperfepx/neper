/* Copyright (C) 2003-2018, Romain Quey */
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
extern int ut_mat_inverse_33 (double **, double **);

extern void ut_mat_eigen (int, double **, double *, double **);
extern void ut_mat_transpose (double **, int, int, double **);
extern void ut_mat_trace (double**, int, double*);
extern void ut_mat_dev (double**, int, double**);

extern int  ut_mat_inverse (double**, int, double**);
extern int ut_mat_svd (double** mat, int M, int N, double** U, double** s, double** V);
extern void  ut_mat_sym (double**, int, double**);
extern void  ut_mat_skew (double**, int, double**);

extern int ut_mat_polar (double **A, int size1, int size2, double **Q,
			 double **S);
extern int ut_mat_sqrt (double **A, int size, double **B);
extern int ut_mat_log (double **A, int size, double **B);
extern int ut_mat_exp (double **A, int size, double **B);

#endif /* UT_MAT_H */

#ifdef __cplusplus
}
#endif
