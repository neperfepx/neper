/* Copyright (C) 2003-2021, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_MAT_H
#define UT_MAT_H

/// \file ut_mat.h
/// \brief Matrix-related functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Multiply 2 matrices
/// \param mat1: matrix 1
/// \param mat1size1: number of rows of \c mat1
/// \param mat1size2: number of columns of \c mat1
/// \param mat2: matrix 2
/// \param mat2size1: number of rows of \c mat2
/// \param mat2size2: number of columns of \c mat2
/// \param mat3: result matrix
/// \note \c mat3 can be one of \c mat1 or \c mat2
/// \note \c mat2size1 must be equal to \c mat1size2
/// \return 0 on success, -1 on failure
  extern int ut_mat_product (double **mat1, int mat1size1, int mat1size2,
                             double **mat2, int mat2size1, int mat2size2,
                             double **mat3);

/// \brief Multiply 2 3x3 matrices
/// \param mat1: matrix 1
/// \param mat2: matrix 2
/// \param mat3: result matrix
/// \note \c mat3 can be one of \c mat1 or \c mat2
  extern void ut_mat_product_33 (double **mat1, double **mat2, double **mat3);

/// \brief Compute the double-contracted product of 2 3x3 matrices
/// \param mat1: matrix 1
/// \param mat2: matrix 2
/// \param mat3: result matrix
/// \note \c mat3 can be one of \c mat1 or \c mat2
  extern double ut_mat_ddot_33 (double **, double **);

  extern int ut_mat_vect_product (double **, int, int, double *, int,
                                  double *);

/// \brief Compute the determinant of a 2x2 matrix
/// \param mat: matrix
/// \return determinant
  extern double ut_mat_det_22 (double **mat);

/// \brief Compute the determinant of a 3x3 matrix
/// \param mat: matrix
/// \return determinant
  extern double ut_mat_det_33 (double **mat);

/// \brief Inverse a matrix
/// \param mat: matrix
/// \param size: matrix size
/// \param matinv: inverse matrix
/// \return 0 on success, -1 on failure (not invertible)
/// \note Uses the GSL if size>3
  extern int ut_mat_inverse (double **mat, int size, double **matinv);

/// \brief Inverse a 2x2 matrix
/// \param mat: matrix
/// \param matinv: inverse matrix
/// \return 0 on success, -1 on failure (not invertible)
  extern int ut_mat_inverse_22 (double **mat, double **matinv);

/// \brief Inverse a 3x3 matrix
/// \param mat: matrix
/// \param matinv: inverse matrix
/// \return 0 on success, -1 on failure (not invertible)
  extern int ut_mat_inverse_33 (double **mat, double **matinv);

/// \brief Get the eigendecomposition of a matrix
/// \param mat: square matrix
/// \param size: size of \c mat
/// \param eval: eigen values
/// \param evect: eigen vectors
/// \note \c eval or \c evect may be \c NULL
/// \note The eigen values are sorted in descending order, and the eigen
/// vectors are listed accordingly.
/// The eigen vector corresponding to eval[i]   is evect[i][0..n-size]
  extern void ut_mat_eigen (double **mat, int size, double *eval,
                            double **evect);

/// \brief Get the transpose of a matrix
/// \param mat: square matrix
/// \param size1: number of rows of \c mat
/// \param size2: number of columns of \c mat
/// \param mattr: transpose matrix
  extern void ut_mat_transpose (double **mat, int size1, int size2,
                                double **mattr);

/// \brief Get the trace of a matrix
/// \param mat: square matrix
/// \param size: size of \c mat
/// \param *ptrace: trace
  extern void ut_mat_trace (double **mat, int size, double *ptrace);

/// \brief Get the deviatoric part of a matrix
/// \param mat: square matrix
/// \param size: size of \c mat
/// \param matdev: deviatoric part of \c mat
  extern void ut_mat_dev (double **mat, int size, double **matdev);

/// \brief Get the symmetrical part of a matrix
/// \param mat: square matrix
/// \param size: size of \c mat
/// \param matsym: symmetrical part of \c mat
  extern void ut_mat_sym (double **mat, int size, double **matsym);

/// \brief Get the skew symmetrical part of a matrix
/// \param mat: square matrix
/// \param size: size of \c mat
/// \param matskew: skew-symmetrical part of \c mat
  extern void ut_mat_skew (double **mat, int size, double **matskew);

/// \brief Get the square root of a matrix
/// \param mat: square matrix
/// \param size: size of \c mat
/// \param matskew: square root of \c mat
  extern void ut_mat_sqrt (double **mat, int size, double **matsqrt);

/// \brief Get the logarithm of a matrix
/// \param mat: square matrix
/// \param size: size of \c mat
/// \param matlog: logarithm of \c mat
  extern void ut_mat_log (double **mat, int size, double **matlog);

/// \brief Get the exponential of a matrix
/// \param mat: square matrix
/// \param size: size of \c mat
/// \param matexp: exponential of \c mat
  extern void ut_mat_exp (double **mat, int size, double **matexp);

  extern void ut_mat_svd (double **mat, int M, int N, double **U, double **s,
                          double **V);

  extern void ut_mat_polar (double **A, int size1, int size2, double **Q,
                            double **S);

#endif                          /* UT_MAT_H */

#ifdef __cplusplus
}
#endif
