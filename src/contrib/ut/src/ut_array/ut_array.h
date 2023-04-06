/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef HAVE_GSL
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>
#endif                          // HAVE_GSL

#ifndef UT_ARRAY_H
#define UT_ARRAY_H

/// \file ut_array.h
/// \brief Array-related functions
/// \author Romain Quey
/// \bug No known bugs
/// \todo move sort array to ut_sort?

/// \brief Set a 1D array of \c double to 0
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_zero (double *array, int size);

/// \brief Set a 1D array of \c int to 0
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_int_zero (int *array, int size);

/// \brief Set a 2D array of \c double to 0
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern void ut_array_2d_zero (double **array, int size1, int size2);

/// \brief Set a 2D array of \c int to 0
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern void ut_array_2d_int_zero (int **array, int size1, int size2);

/// \brief Set a 3D array of \c double to 0
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern void ut_array_3d_zero (double ***array, int size1, int size2,
                                int size3);

/// \brief Set a 1D array of \c int to 0
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern void ut_array_3d_int_zero (int ***array, int size1, int size2,
                                    int size3);

/// \brief Scale a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \param factor: scaling factor
  extern void ut_array_1d_scale (double *array, int size, double factor);

/// \brief Scale a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \param factor: scaling factor
/// \remark Elements are scaled by \c factor and cast back to \c int (no rounding).
  extern void ut_array_1d_int_scale (int *array, int size, double factor);

/// \brief Scale a 2D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param factor: scaling factor
  extern void ut_array_2d_scale (double **array, int size1, int size2,
                                 double factor);

/// \brief Normalize a 1D array of \c double
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_normalize (double *array, int size);

/// \brief Add 2 1D arrays of \c double
/// \param array1: 1st-index array
/// \param array2: 2nd-index array
/// \param size: size of the arrays
/// \param array3: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_1d_add (double *array1, double *array2, int size,
                               double *array3);

/// \brief Add 2 1D arrays of \c int
/// \param array1: 1st-index array
/// \param array2: 2nd-index array
/// \param size: size of the arrays
/// \param array3: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_1d_int_add (int *array1, int *array2, int size,
                                   int *array3);

/// \brief Add 2 2D arrays of \c double
/// \param array1: 1st-index array
/// \param array2: 2nd-index array
/// \param size1: 1st-index size of the arrays
/// \param size2: 2nd-index size of the arrays
/// \param array3: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_2d_add (double **array1, double **array2, int size1,
                               int size2, double **array3);

/// \brief Add 2 3D arrays of \c double
/// \param array1: 1st-index array
/// \param array2: 2nd-index array
/// \param array3: 3rd-index array
/// \param size1: 1st-index size of the arrays
/// \param size2: 2nd-index size of the arrays
/// \param size3: 3rd-index size of the arrays
/// \param array3: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_3d_add (double ***array1, double ***array2, int size1,
                               int size2, int size3, double ***array3);

/// \brief Add a value to a 1D array of \c double
/// \param array1: input array
/// \param size: size of the input array
/// \param val: value
/// \param array2: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_1d_addval (double *array1, int size, double val,
                                  double *array2);

/// \brief Subtract 2 1D arrays of \c double
/// \param array1: 1st-index array
/// \param array2: 2nd-index array
/// \param size: size of the arrays
/// \param array3: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_1d_sub (double *array1, double *array2, int size,
                               double *array3);

/// \brief Subtract 2 2D arrays of \c double
/// \param array1: 1st-index array
/// \param array2: 2nd-index array
/// \param size1: 1st-index size of the arrays
/// \param size2: 2nd-index size of the arrays
/// \param array3: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_2d_sub (double **array1, double **array2, int size1,
                               int size2, double **array3);

/// \brief Add a value to a 1D array of \c int
/// \param array1: input array
/// \param size: size of the input array
/// \param val: value
/// \param array2: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_1d_int_addval (int *array1, int size, int val,
                                      int *array2);

/// \brief Add a value to a 2D array of \c double
/// \param array1: input array
/// \param size1: 1st-index size of the input array
/// \param size2: 2nd-index size of the input array
/// \param val: value
/// \param array2: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_2d_addval (double **array1, int size1, int size2,
                                  double val, double **array2);

/// \brief Add a value to a 2D array of \c int
/// \param array1: input array
/// \param size1: 1st-index size of the input array
/// \param size2: 2nd-index size of the input array
/// \param val: value
/// \param array2: output array
/// \remark Input and output arrays can be the same.
  extern void ut_array_2d_int_addval (int **array1, int size1, int size2,
                                      int val, int **array2);

/// \brief Get the norm of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return norm
  extern double ut_array_1d_norm (double *array, int size);

/// \brief Get the norm of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return norm
  extern double ut_array_1d_int_norm (int *array, int size);

/// \brief Set a 1D array of \c int to the element ids
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_int_set_id (int *array, int size);

/// \brief Get the minimum of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return minimum
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_min (double *array, int size);

/// \brief Get the minimum of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return minimum
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_int_min (int *array, int size);

/// \brief Get the index of the minimum of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return index
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_int_min_index (int *array, int size);

/// \brief Get the minimum of a 2D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return minimum
/// \remark Failure if \c size* <= 0
  extern double ut_array_2d_min (double **array, int size1, int size2);

/// \brief Get the minimum of a 3D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return minimum
/// \remark Failure if \c size* <= 0
  extern double ut_array_3d_min (double ***array, int size1, int size2,
                                 int size3);

/// \brief Get the minimum of a 2D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return minimum
/// \remark Failure if \c size <= 0
  extern int ut_array_2d_int_min (int **array, int size1, int size2);

/// \brief Get the minimum of a column of a 2D array of \c double
/// \param array: array
/// \param col: column (2nd index)
/// \param size: size
/// \return minimum
/// \remark Failure if \c size <= 0
  extern double ut_array_2d_col_min (double **array, int col, int size);

/// \brief Get the minimum of a 3D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return minimum
/// \remark Failure if \c size* <= 0
  extern int ut_array_3d_int_min (int ***array, int size1, int size2,
                                  int size3);

/// \brief Get the minimum of a 3D array of \c unsigned \c char
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return minimum
/// \remark Failure if \c size* <= 0
  extern unsigned char ut_array_3d_uchar_min (unsigned char ***array,
                                              int size1, int size2,
                                              int size3);

/// \brief Get the minimum of a 3D array of \c unsigned \c short
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return minimum
/// \remark Failure if \c size* <= 0
  extern unsigned short ut_array_3d_ushort_min (unsigned short ***array,
                                                int size1, int size2,
                                                int size3);

/// \brief Get the index of the minimum element of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return index
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_min_index (double *array, int size);

/// \brief Get the maximum of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return maximum
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_max (double *array, int size);

/// \brief Get the maximum of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return maximum
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_int_max (int *array, int size);

/// \brief Get the maximum of a 2D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return maximum
/// \remark Failure if \c size* <= 0
  extern double ut_array_2d_max (double **array, int size1, int size2);

/// \brief Get the maximum of a 2D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return maximum
/// \remark Failure if \c size <= 0
  extern int ut_array_2d_int_max (int **array, int size1, int size2);

/// \brief Get the maximum of a 3D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return maximum
/// \remark Failure if \c size <= 0
  extern int ut_array_3d_int_max (int ***array, int size1, int size2,
                                  int size3);

/// \brief Get the maximum of a column of a 2D array of \c double
/// \param array: array
/// \param col: column (2nd index)
/// \param size: size
/// \return maximum
/// \remark Failure if \c size <= 0
  extern double ut_array_2d_col_max (double **array, int col, int size);

/// \brief Get the maximum of a 3D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return maximum
/// \remark Failure if \c size* <= 0
  extern double ut_array_3d_max (double ***array, int size1, int size2,
                                 int size3);

/// \brief Get the maximum of a 3D array of \c unsigned \c char
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return maximum
/// \remark Failure if \c size* <= 0
  extern unsigned char ut_array_3d_uchar_max (unsigned char ***array,
                                              int size1, int size2,
                                              int size3);

/// \brief Get the maximum of a 3D array of \c unsigned \c short
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return maximum
/// \remark Failure if \c size* <= 0
  extern unsigned short ut_array_3d_ushort_max (unsigned short ***array,
                                                int size1, int size2,
                                                int size3);

/// \brief Get the index of the maximum element of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return index
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_max_index (double *array, int size);

/// \brief Get the index of the maximum element of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return index
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_int_max_index (int *array, int size);

/// \brief Get the maximum absolute value of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return absolute maximum
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_absmax (double *array, int size);

/// \brief Get the absolute maximum of a 1D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return absolute maximum
/// \remark Failure if \c size <= 0
  extern double ut_array_2d_absmax (double **array, int size1, int size2);

/// \brief Get the index of the absolute maximum of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return index
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_absmax_index (double *array, int size);

/// \brief Get the absolute minimum of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return absolute minimum
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_absmin (double *, int);

/// \brief Get the index of the absolute minimum of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return index
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_absmin_index (double *, int);

/// \brief Get the mean of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_mean (double *array, int size);

/// \brief Get the sum of a 2D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return mean
  extern double ut_array_2d_sum (double **array, int size1, int size2);

/// \brief Get the mean of a 2D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return mean
  extern double ut_array_2d_mean (double **array, int size1, int size2);

/// \brief Get the mean of a column of a 2D array of \c double
/// \param array: array
/// \param size1: size (1st index)
/// \param col: column (2nd index)
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_2d_col_mean (double **array, int size1, int col);

/// \brief Get the weighted mean of a column of a 2D array of \c double
/// \param array: array
/// \param size1: size (1st index)
/// \param col: column (2nd index)
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_2d_col_wmean (double **array, double *weight, int size1, int col);

/// \brief Get the mean of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_int_mean (int *array, int size);

/// \brief Get the mean of a 3D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return mean
/// \remark Failure if \c size* <= 0
  extern double ut_array_3d_int_mean (int ***array, int size1, int size2,
                                      int size3);

/// \brief Get the mean of a 3D array of \c unsigned \c char
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return mean
/// \remark Failure if \c size* <= 0
  extern double ut_array_3d_uchar_mean (unsigned char ***array, int size1,
                                        int size2, int size3);

/// \brief Get the mean of a 3D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return mean
/// \remark Failure if \c size* <= 0
  extern double ut_array_3d_mean (double ***array, int size1, int size2,
                                  int size3);

/// \brief Get the mean of a 1D array of weighted \c double
/// \param array: array
/// \param weight: weight array
/// \param size: size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_wmean (double *array, double *weight, int size);

/// \brief Get the Lp mean of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \param p: power
/// \return Lmean
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_lmean (double *array, int size, double p);

/// \brief Get the Lp norm of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \param p: power
/// \return Lnorm
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_lnorm (double *array, int size, double p);

/// \brief Get the Lp mean (specified as an expression) of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \param expr: Lp expression: Linf, default (= L2), or L*
/// \return Lmean
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_lmean_expr (double *array, int size, char *expr);

/// \brief Get the geometric mean of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_gmean (double *array, int size);

/// \brief Get the geometric mean of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_int_gmean (int *array, int size);

/// \brief Get the geometric mean of a 2D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_2d_gmean (double **array, int size1, int size2);

/// \brief Get the geometric mean of a 3D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_3d_gmean (double ***array, int size1, int size2,
                                   int size3);

/// \brief Get the geometric mean of a 3D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_3d_int_gmean (int ***array, int size1, int size2,
                                       int size3);

/// \brief Get the geometric mean of a 3D array of \c unsigned char
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return mean
/// \remark Failure if \c size <= 0
  extern double ut_array_3d_uchar_gmean (unsigned char ***array, int size1,
                                         int size2, int size3);

/// \brief Get the standard deviation of a 1D array of \c double
/// \param array: array
/// \param mean: mean
/// \param size: size of the array
/// \return standard deviation
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_stddev (double *array, double mean, int size);

/// \brief Get the standard deviation of a 2D array of \c double
/// \param array: array
/// \param mean: mean
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return standard deviation
/// \remark Failure if \c size <= 0
  extern double ut_array_2d_stddev (double **array, double mean, int size1,
                                    int size2);

/// \brief Get the standard deviation of a 3D array of \c double
/// \param array: array
/// \param mean: mean
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return standard deviation
/// \remark Failure if \c size <= 0
  extern double ut_array_3d_stddev (double ***array, double mean, int size1,
                                    int size2, int size3);

/// \brief Get the standard deviation of a 3D array of \c int
/// \param array: array
/// \param mean: mean
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return standard deviation
/// \remark Failure if \c size <= 0
  extern double ut_array_3d_int_stddev (int ***array, double mean, int size1,
                                        int size2, int size3);

/// \brief Get the standard deviation of a 3D array of \c unsigned \c short
/// \param array: array
/// \param mean: mean
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return standard deviation
/// \remark Failure if \c size <= 0
  extern double ut_array_3d_ushort_stddev (unsigned short ***array,
                                           double mean, int size1, int size2,
                                           int size3);

/// \brief Get the standard deviation of a 3D array of \c unsigned \c char
/// \param array: array
/// \param mean: mean
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \return standard deviation
/// \remark Failure if \c size <= 0
  extern double ut_array_3d_uchar_stddev (unsigned char ***array, double mean,
                                          int size1, int size2, int size3);

/// \brief Get the standard deviation of a 1D weighted array of \c double
/// \param array: array
/// \param warray: weight array
/// \param mean: mean
/// \param size: size of the array
/// \return standard deviation
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_wstddev (double *array, double *warray,
                                     double mean, int size);

/// \brief Get the sum of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return sum
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_sum (double *array, int size);

/// \brief Get the sum of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return sum
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_int_sum (int *array, int size);

/// \brief Get the sum of a 1D array of \c unsigned \c int
/// \param array: array
/// \param size: size of the array
/// \return sum
/// \remark Failure if \c size <= 0
  extern unsigned int ut_array_1d_uint_sum (unsigned int *array, int size);

/// \brief Get the absolute sum of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return sum
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_abssum (double *array, int size);

/// \brief Get the sum of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return sum
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_int_abssum (int *array, int size);

/// \brief Get the sum of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return product
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_prod (double *array, int size);

/// \brief Get the sum of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return product
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_int_prod (int *array, int size);

/// \brief Get the scalar product of two 1D arrays of \c double
/// \param array1: 1st array
/// \param array2: 2nd array
/// \param size: size of the arrays
/// \return scalar product
/// \remark Failure if \c size <= 0
  extern double ut_array_1d_scalprod (double *array1, double *array2,
                                      int size);

/// \brief Get the scalar product of two 1D arrays of \c int
/// \param array1: 1st array
/// \param array2: 2nd array
/// \param size: size of the arrays
/// \return scalar product
/// \remark Failure if \c size <= 0
  extern int ut_array_1d_int_scalprod (int *array1, int *array2, int size);

/// \brief Get a sample of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \param fact: sampling factor
/// \param *parray2: output array
/// \param *psize2: size of the output array
  extern void ut_array_1d_sample (double *array, int size, double fact,
                                  double **parray2, int *psize2);

/// \brief Get a sample of a 2D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param fact: sampling factor
/// \param *parray2: output array
/// \param *psize2: size of the output array
  extern void ut_array_2d_sample (double **array, int size1, int size2,
                                  double fact, double ***parray2,
                                  int *psize2);

/// \brief Get the signs of the elements of an array
/// \param array: array
/// \param size: size of the array
/// \param sarray: sign array
  extern void ut_array_1d_int_sgn (int *array, int size, int *sarray);

/// \brief Copy a \c double array
/// \param src: source array
/// \param size: size of the array
/// \param dest: destination array
  extern void ut_array_1d_memcpy (double *src, int size, double *dest);

/// \brief Copy a 1D \c int array into a \c double array
/// \param src: source array
/// \param size: size of the array
/// \param dest: destination array
  extern void ut_array_1d_memcpy_fromint (int *src, int size, double *dest);

/// \brief Copy a 1D \c double array into a \c int array
/// \param src: source array
/// \param size: size of the array
/// \param dest: destination array
  extern void ut_array_1d_memcpy_toint (double *src, int size, int *dest);

/// \brief Copy a 1D \c int array
/// \param src: source array
/// \param size: size of the array
/// \param dest: destination array
  extern void ut_array_1d_int_memcpy (int *src, int size, int *dest);

/// \brief Copy a 1D \c unsigned \c int array
/// \param src: source array
/// \param size: size of the array
/// \param dest: destination array
  extern void ut_array_1d_uint_memcpy (unsigned int *src, int size,
                                       unsigned int *dest);

/// \brief Copy a 2D \c double array
/// \param src: source array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param dest: destination array
  extern void ut_array_2d_memcpy (double **src, int size1, int size2,
                                  double **dest);

/// \brief Copy a 2D \c int array
/// \param src: source array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param dest: destination array
  extern void ut_array_2d_int_memcpy (int **src, int size1, int size2,
                                      int **dest);

/// \brief Copy a 1D \c char* array
/// \param src: source array
/// \param size: size of the array
/// \param dest: destination array
  extern void ut_array_1d_pchar_memcpy (char **src, int size, char **dest);

/// \brief Copy a 3D \c double array
/// \param src: source array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param dest: destination array
  extern void ut_array_3d_memcpy (double ***src, int size1, int size2,
                                  int size3, double ***dest);

/// \brief Copy a 4D \c int array
/// \param src: source array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param dest: destination array
  extern void ut_array_3d_int_memcpy (int ***src, int size1, int size2,
                                      int size3, int ***dest);

/// \brief Copy a 4D \c unsigned \c char array
/// \param src: source array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param dest: destination array
  extern void ut_array_3d_uchar_memcpy (unsigned char ***src, int size1,
                                        int size2, int size3,
                                        unsigned char ***dest);

/// \brief Copy a 4D \c double array
/// \param src: source array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param size4: 4th-index size of the array
/// \param dest: destination array
  extern void ut_array_4d_memcpy (double ****src, int size1, int size2,
                                  int size3, int size4, double ****dest);

/// \brief Switch 2 elements of a 1D \c double array
/// \param array: array
/// \param id1: id of the 1st element
/// \param id2: id of the 2nd element
  extern void ut_array_1d_switch (double *array, int id1, int id2);

/// \brief Switch 2 elements of a 1D \c int array
/// \param array: array
/// \param id1: id of the 1st element
/// \param id2: id of the 2nd element
  extern void ut_array_1d_int_switch (int *array, int id1, int id2);

/// \brief Switch 2 lines of a 2D \c double array
/// \param array: array
/// \param linelength: line length
/// \param id1: id of the 1st line (1st-index)
/// \param id2: id of the 2nd line (1st-index)
  extern void ut_array_2d_switchlines (double **array, int linelength,
                                       int id1, int id2);

/// \brief Switch 2 lines of a 2D \c int array
/// \param array: array
/// \param linelength: line length
/// \param id1: id of the 1st line (1st-index)
/// \param id2: id of the 2nd line (1st-index)
  extern void ut_array_2d_int_switchlines (int **array, int linelength,
                                           int id1, int id2);

/// \brief Switch 2 lines of a 2D \c int array, with lines of different lengths
/// \param array: array
/// \param id1: id of the 1st line (1st-index)
/// \param id1length: 1st line length
/// \param id2: id of the 2nd line (1st-index)
/// \param id2length: 2nd line length
  extern void ut_array_2d_int_switchlines_length (int **array, int id1,
                                                  int id1length, int id2,
                                                  int id2length);

/// \brief Switch 2 subarrays of a 3D \c double array
/// \param array: array
/// \param size1: 1st-index size of the subarrays
/// \param size2: 2nd-index size of the subarrays
/// \param id1: id of the 1st subarray (1st-index)
/// \param id2: id of the 2nd subarray (1st-index)
  extern void ut_array_3d_switcharrays (double ***array, int size1, int
                                        size2, int line1, int line2);

/// \brief Switch 2 subarrays of a 3D \c int array
/// \param array: array
/// \param size1: 1st-index size of the subarrays
/// \param size2: 2nd-index size of the subarrays
/// \param id1: id of the 1st subarray (1st-index)
/// \param id2: id of the 2nd subarray (1st-index)
  extern void ut_array_3d_int_switcharrays (int ***array, int size1, int
                                            size2, int line1, int line2);

/// \brief Sort a 1D \c double array
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_sort (double *array, int size);

/// \brief Sort a 1D \c double array in terms of absolute values
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_abssort (double *array, int size);

/// \brief Sort a 1D \c int array
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_int_sort (int *array, int size);

/// \brief Sort a 1D \c char* array
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_pchar_sort (char **array, int size);

/// \brief Sort a 1D \c double array, descending order
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_sort_des (double *array, int size);

/// \brief Sort a 1D \c double array, descending order
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_int_sort_des (int *array, int size);

/// \brief Sort a 1D \c double array, get the indices
/// \param array: array
/// \param size: size of the array
/// \param idarray: index array
  extern void ut_array_1d_sort_index (double *array, int size, int *idarray);

/// \brief Sort a 1D \c double array in terms of absolute values, get the indices
/// \param array: array
/// \param size: size of the array
/// \param idarray: index array
  extern void ut_array_1d_abssort_index (double *array, int size,
                                         int *idarray);

/// \brief Sort a 1D \c int array, get the indices
/// \param array: array
/// \param size: size of the array
/// \param idarray: index array
  extern void ut_array_1d_int_sort_index (int *array, int size, int *idarray);

/// \brief Sort a 1D \c double array in descending order, get the indices
/// \param array: array
/// \param size: size of the array
/// \param idarray: index array
  extern void ut_array_1d_sort_des_index (double *array, int size,
                                          int *idarray);

/// \brief Sort a 1D \c int array in descending order, get the indices
/// \param array: array
/// \param size: size of the array
/// \param idarray: index array
  extern void ut_array_1d_int_sort_des_index (int *array, int size,
                                              int *idarray);

/// \brief Sort a 1D \c int array in terms of absolute values, get the indices
/// \param array: array
/// \param size: size of the array
/// \param idarray: index array
  extern void ut_array_1d_int_abssort_index (int *array, int size,
                                             int *idarray);

/// \brief Sort a 2D \c int array based on a column (i.e., rearrange lines)
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param col: column (2nd-index)
  extern void ut_array_2d_int_sortbycol (int **array, int size1, int size2,
                                         int col);

/// \brief Sort a 2D \c int array in descending order based on a column (i.e., rearrange lines)
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param col: column (2nd-index)
  extern void ut_array_2d_int_sortbycol_des (int **array, int size1,
                                             int size2, int col);

/// \brief Sort a 1D \c double array, but apply permutation to another array
/// \param array: array
/// \param size: size of the array
/// \param permarray: array permutations are applied to
  extern void ut_array_1d_sort_permarray_int (double *array, int size,
                                              int *permarray);

/// \brief Remove contiguous duplicate element of a 1D \c int array
/// \param array: array
/// \param size: input size of the array
/// \param *psize: output size of the array
  extern void ut_array_1d_int_uniq (int *array, int size, int *psize);

/// \brief Sort and remove contiguous duplicate element of a 1D \c int array
/// \param array: array
/// \param size: input size of the array
/// \param *psize: output size of the array
  extern void ut_array_1d_int_sort_uniq (int *array, int size, int *psize);

/// \brief Find an element in a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param elt: element
/// \return id of the element if element exist, -1 otherwise
  extern int ut_array_1d_int_find (int *array, int size, int elt);

/// \brief Replace an element by another in a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param elt1: replaced element
/// \param elt2: replacing element
  extern void ut_array_1d_int_findnreplace (int *array, int size, int elt1,
                                            int elt2);

/// \brief Rotate the elements of a 1D \c double array
/// \param array: array
/// \param size: size of the array
/// \param s: rotation vector
  extern void ut_array_1d_rotate (double *array, int size, int s);

/// \brief Rotate the elements of a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param s: rotation vector
  extern void ut_array_1d_int_rotate (int *array, int size, int s);

/// \brief Reverse the elements of a 1D \c double array
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_reverseelts (double *, int);

/// \brief Reverse the elements of a 1D \c int array
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_int_reverseelts (int *array, int size);

/// \brief Set a 1D \c double array to a value
/// \param array: array
/// \param size: size of the array
/// \param val: value
  extern void ut_array_1d_set (double *array, int size, double val);

/// \brief Set a 1D \c int array to a value
/// \param array: array
/// \param size: size of the array
/// \param val: value
  extern void ut_array_1d_int_set (int *array, int size, int val);

/// \brief Set a 1D \c unsigned \c int array to a value
/// \param array: array
/// \param size: size of the array
/// \param val: value
  extern void ut_array_1d_uint_set (unsigned int *array, int size,
                                    unsigned int val);

/// \brief Set a 2D \c double array to a value
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param val: value
  extern void ut_array_2d_set (double **array, int size1, int size2,
                               double val);

/// \brief Set a 2D \c int array to a value
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param val: value
  extern void ut_array_2d_int_set (int **array, int size, int size2, int val);

/// \brief Set the column of a 2D \c double array to a value
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param col: column
/// \param val: value
  extern void ut_array_2d_col_set (double **array, int size, int size2,
                                   double val);

/// \brief Set the column of a 2D \c int array to a value
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param col: column
/// \param val: value
  extern void ut_array_2d_int_col_set (int **array, int size, int col,
                                       int val);

/// \brief Set a 3D \c double array to a value
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param val: value
  extern void ut_array_3d_set (double ***array, int size, int size2,
                               int size3, double val);

/// \brief Set a 3D \c int array to a value
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param val: value
  extern void ut_array_3d_int_set (int ***array, int size, int size2,
                                   int size3, int val);

/// \brief Determine whether 2 \c double arrays are equal to a given tolerance
/// \param array1: 1st array
/// \param array2: 2nd array
/// \param size: size of the arrays
/// \param tol: tolerancy
/// \return 1 if equal and 0 otherwise
  extern int ut_array_1d_equal (double *array, double *array2, int size,
                                double tol);

/// \brief Determine whether 2 \c int arrays are equal
/// \param array1: 1st array
/// \param size1: size of the 1st array
/// \param array2: 2nd array
/// \param size2: size of the 2nd array
/// \return 1 if equal and 0 otherwise
  extern int ut_array_1d_int_equal (int *array1, int size1, int *array2,
                                    int size2);

/// \brief Determine whether 2 \c double arrays are equal to a given relative tolerance
/// \param array1: 1st array
/// \param array2: 2nd array
/// \param size: size of the arrays
/// \param tol: tolerance
/// \return 1 if equal and 0 otherwise
  extern int ut_array_1d_requal (double *array1, double *array2, int size,
                                 double tol);

/// \brief Determine whether 2 \c int arrays are opposite
/// \param array1: 1st array
/// \param array2: 2nd array
/// \param size: size of the arrays
/// \return 1 if opposite and 0 otherwise
  extern int ut_array_1d_int_opposite (int *array, int *array2, int size);

/// \brief Determine whether 2 \c double arrays are equal to a given tolerance
/// \param array1: 1st array
/// \param array2: 2nd array
/// \param size1: 1st-index size of the arrays
/// \param size1: 2nd-index size of the arrays
/// \param tol: tolerancy
/// \return 1 if equal and 0 otherwise
  extern int ut_array_2d_equal (double **array1, double **array2, int size1,
                                int size2, double tol);

/// \brief Permute the element of a 1D \c double array
/// \param array: array
/// \param size: size of the array
/// \param permute: index permutation array
  extern void ut_array_1d_permute (double *array, int size, int *permute);

/// \brief Permute the element of a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param permute: index permutation array
  extern void ut_array_1d_int_permute (int *array, int size, int *permute);

/// \brief Permute the lines of a 2D \c double array
/// \param array: array
/// \param size2: 1st-index size of the array
/// \param size1: 2nd-index size of the array
/// \param permute: index permutation array
  extern void ut_array_2d_int_permute (int **array, int size1, int size2,
                                       int *permute);

/// \brief Get a rotated position within an array
/// \param start: start position
/// \param end: end position
/// \param position: position
/// \param shift: position shift
/// \return rotated position
/// \note Periodicity applies.
  extern int ut_array_1d_rotpos (int start, int end, int pos, int shift);

/// \brief Get the position of an element within a 1D \c double array
/// \param array: array
/// \param size: size of the array
/// \param value: value
/// \return position
/// \note The position of the closest element is returned.
  extern int ut_array_1d_eltpos (double *array, int size, double value);

/// \brief Get the position of an element within a 1D \c double array, use interpolation
/// \param array: array
/// \param size: size of the array
/// \param value: value
/// \return position
/// \note The array must be sorted in ascending order.
  extern double ut_array_1d_eltpos_interpolate (double *array, int size,
                                                double value);

/// \brief Get the position of an element within a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param value: value
/// \return position if found and -1 otherwise
  extern int ut_array_1d_int_eltpos (int *array, int size, int value);

/// \brief Get the position of a string a 1D \c char* array
/// \param array: array
/// \param size: size of the array
/// \param value: string value
/// \return position if found and -1 otherwise
  extern int ut_array_1d_string_eltpos (char **array, int size, char *value);

/// \brief Get the position of an element within a 1D \c unsigned \c int array
/// \param array: array
/// \param size: size of the array
/// \param value: value
/// \return position if found and -1 otherwise
  extern int ut_array_1d_uint_eltpos (unsigned int *array, int size,
                                      unsigned int value);

/// \brief Get all positions of an element within a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param value: value
/// \param pos: positions
/// \return number of positions
  extern int ut_array_1d_int_eltpos_all (int *array, int size, int value,
                                         int *pos);

/// \brief Get the position of an element within a 2D \c int array
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param value: value
/// \param *ppos1: 1st-index position
/// \param *ppos2: 2nd-index position
/// \return 1 if found and -1 otherwise
  extern int ut_array_2d_int_eltpos (int **array, int size1, int size2,
                                     int value, int *ppos1, int *ppos2);

/// \brief Get the position of a line within a 2D \c int array
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param subarray: subarray
/// \return position if found and -1 otherwise
  extern int ut_array_2d_int_linepos (int **array, int size1, int size2,
                                      int *values);

/// \brief Get the position of a line within a non-square 2D \c int array
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: array of 2nd-index size of the array
/// \param subarray: subarray
/// \param subsize: size of the subarray
/// \return position if found and -1 otherwise
  extern int ut_array_2dns_int_linepos (int **array, int size1, int *size2,
                                        int *subarray, int subsize);

/// \brief Get the number of occurrence of a value within a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param value: value
/// \return number of values
  extern int ut_array_1d_int_valnb (int *array, int size, int value);

/// \brief Get the values and their numbers of occurrence within a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param *pvals: values
/// \param *pvalnbs: numbers of occurrence of the values
/// \param *pvalnb: number of values
/// \return number of values
  extern void ut_array_1d_int_valnbs (int *array, int size, int **pvals,
                                      int **pvalnbs, int *pvalnb);

/// \brief Get the number of occurrence of a value within a 1D \c unsigned \c int array
/// \param array: array
/// \param size: size of the array
/// \param value: value
/// \return number of values
  extern int ut_array_1d_uint_valnb (unsigned int *array, int size,
                                     unsigned int value);

/// \brief Get the intersection between two \c int arrays
/// \param array1: first array
/// \param size1: size of the first array
/// \param array2: second array
/// \param size2: size of the second array
/// \param array3: result array
/// \param size3: size of the result array
  extern void ut_array_1d_int_inter (int *array1, int size1, int *array2,
                                     int size2, int *array3, int *psize3);

/// \brief Concatenate two \c int arrays
/// \param array1: first array
/// \param size1: size of the first array
/// \param array2: second array
/// \param size2: size of the second array
/// \param *parray3: result array
/// \param *psize3: size of the result array
  extern void ut_array_1d_int_cat (int *array1, int size1, int *array2,
                                   int size2, int **parray3, int *size3);

/// \brief Remove an element of a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param value: value to remove
/// \param loop: number of loops
/// \return number of removed elements
/// \note The array is "compressed" upon element removal.
  extern int ut_array_1d_int_rmelt (int *array, int size, int value,
                                    int loop);

/// \brief Transpose a 2D \c double arrays
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param *parray2: transposed array
  extern void ut_array_2d_transpose (double **array, int size1, int size2,
                                     double ***parray2);

/// \brief Transpose a 2D \c int arrays
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param *parray2: transposed array
  extern void ut_array_2d_int_transpose (int **array, int size1, int size2,
                                         int ***parray2);

/// \brief Add a value to a 1D list \c int array
/// \param *parray: array
/// \param *psize: size of the array
/// \param value: value to remove
/// \note The element is added only if it does not exist
  extern int ut_array_1d_int_list_addval (int **parray, int *psize,
                                          int value);

/// \brief Add a value to a 1D list \c int array, do not check for existence
/// \param *parray: array
/// \param *psize: size of the array
/// \param value: value to remove
/// \note Should be used only if the element does not exist in the array.
/// \note The value is added at the end of the array.
  extern int ut_array_1d_int_list_addval_nocheck (int **parray, int *psize,
                                                  int value);

/// \brief Add values to a 1D list \c int array
/// \param *parray: array
/// \param *psize: size of the array
/// \param values: values to add
/// \param eltsize: size of \c values
  extern void ut_array_1d_int_list_addvals (int **parray, int *psize,
                                            int *values, int valuenb);

/// \brief Remove a value from a 1D list \c int array
/// \param *parray: array
/// \param *psize: size of the array
/// \param value: value to remove
  extern void ut_array_1d_int_list_rmval (int **parray, int *psize,
                                          int value);

/// \brief Remove values from a 1D list \c int array
/// \param *parray: array
/// \param *psize: size of the array
/// \param values: values to add
/// \param valuenb: number of \c values
  extern void ut_array_1d_int_list_rmvals (int **parray, int *psize,
                                           int *values, int valuenb);

/// \brief Merge lists
/// \param listvals: array of list values
/// \param listvalnbs: array of numbers of list values
/// \param listnb: number of lists
/// \param *pvals: values
/// \param *pvalnb: number of values
  extern void ut_array_1d_int_lists_merge (int **listvals, int *listvalnbs,
                                           int listnb, int **pvals,
                                           int *pvalnb);

/// \brief Merge two lists
/// \param vals1: values of the first list
/// \param valnb1: nb of values of the first list
/// \param vals2: values of the second list
/// \param valnb2: nb of values of the second list
/// \param *pvals: values
/// \param *pvalnb: number of values
  extern int ut_array_1d_int_listpair_merge (int *vals1, int valnb1,
                                             int *vals2, int valnb2,
                                             int **pvals, int *pvalnb);

/// \brief Test if two lists are equal
/// \param vals1: values of the first list
/// \param valnb1: nb of values of the first list
/// \param vals2: values of the second list
/// \param valnb2: nb of values of the second list
/// \param *pvals: values
/// \param *pvalnb: number of values
  extern int ut_array_1d_int_listpair_equal (int *vals1, int valnb1,
                                             int *vals2, int valnb2);

/// \brief Pair lists of same lengths
/// \param listvals1: array of list values, 1st array
/// \param listvalnbs1: number of list values (same for all lists), 1st array
/// \param listnb1: number of lists, 1st array
/// \param listvals2: array of list values, 2nd array
/// \param listvalnbs2: number of list values (same for all lists), 2nd array
/// \param listnb2: number of lists, 2nd array
/// \param pair: correspondance between the lists of the two arrays
/// \note list i of listvals1 corresponds to list pair[i] of listvals2
/// \return 0 on success and -1 on failure
/// \todo Use ut_array_1d_int_lists_pair inside
  extern int ut_array_1d_int_lists_pair_samelength (int **listvals1,
                                                    int listvalnbs1,
                                                    int listnb1,
                                                    int **listvals2,
                                                    int listvalnbs2,
                                                    int listnb2, int *pair);

/// \brief Pair lists
/// \param listvals1: array of list values, 1st array
/// \param listvalnbs1: array of numbers of list values, 1st array
/// \param listnb1: number of lists, 1st array
/// \param listvals2: array of list values, 2nd array
/// \param listvalnbs2: array of numbers of list values, 2nd array
/// \param listnb2: number of lists, 2nd array
/// \param pair: correspondance between the lists of the two arrays
/// \note list i of listvals1 corresponds to list pair[i] of listvals2
/// \return 0 on success and -1 on failure
  extern int ut_array_1d_int_lists_pair (int **listvals1, int *listvalnbs1,
                                         int listnb1, int **listvals2,
                                         int *listvalnbs2, int listnb2,
                                         int *pair);

/// \brief Pair the lines of 2D \c int array
/// \param a: 1st array
/// \param sizea1: 1st-index size of a
/// \param sizea2: 2nd-index size of a
/// \param b: 2nd array
/// \param sizea1: 1st-index size of b
/// \param sizea2: 2nd-index size of b
/// \param pair: correspondance between the lines of the two arrays
/// \note line i of a corresponds to list pair[i] of b
/// \return 0 on success and -1 on failure
  extern int ut_array_2d_int_pair (int **a, int sizea1, int sizea2, int **b,
                                   int sizeb1, int sizeb2, int *pair);

/// \brief Pair the lines of 2D \c double array
/// \param a: 1st array
/// \param sizea1: 1st-index size of a
/// \param sizea2: 2nd-index size of a
/// \param b: 2nd array
/// \param sizea1: 1st-index size of b
/// \param sizea2: 2nd-index size of b
/// \param pair: correspondance between the lines of the two arrays
/// \param *pdist: total L2 distance between the lines
/// \note line i of a corresponds to list pair[i] of b
/// \return 0 on success and -1 on failure
  extern int ut_array_2d_pair (double **a, int sizea1, int sizea2, double **b,
                               int sizeb1, int sizeb2, int *pair,
                               double *pdist);

/// \brief Get the inverse of a 1D \c int array
/// \param array: array
/// \param size: size of the array
/// \param *pinvarray: inverse array
/// \param *pinvsize: size of the inverse array
/// \note The inverse array is such that invarray[array[i]] = i.
  extern void ut_array_1d_int_inv (int *array, int size, int **invarray,
                                   int *invsize);

/// \brief Set a 1D \c int array of 2 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
  extern void ut_array_1d_int_set_2 (int *array, int val1, int val2);

/// \brief Set a 1D \c int array of 3 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
/// \param val3: 3rd value
  extern void ut_array_1d_int_set_3 (int *array, int val1, int val2,
                                     int val3);

/// \brief Set a 1D \c int array of 4 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
/// \param val3: 3rd value
/// \param val4: 4th value
  extern void ut_array_1d_int_set_4 (int *array, int val1, int val2, int val3,
                                     int val4);

/// \brief Set a 1D \c int array of 5 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
/// \param val3: 3rd value
/// \param val4: 4th value
/// \param val5: 5th value
  extern void ut_array_1d_int_set_5 (int *array, int val1, int val2, int val3,
                                     int val4, int val5);

/// \brief Set a 1D \c double array of 2 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
  extern void ut_array_1d_set_2 (double *array, double val1, double val2);

/// \brief Set a 1D \c double array of 2 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
/// \param val3: 3rd value
  extern void ut_array_1d_set_3 (double *array, double val1, double val2,
                                 double val3);

/// \brief Set a 1D \c double array of 2 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
/// \param val3: 3rd value
/// \param val4: 4th value
  extern void ut_array_1d_set_4 (double *array, double val1, double val2,
                                 double val3, double val4);

/// \brief Set a 1D \c double array of 2 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
/// \param val3: 3rd value
/// \param val4: 4th value
/// \param val5: 5th value
  extern void ut_array_1d_set_5 (double *array, double val1, double val2,
                                 double val3, double val4, double val5);

/// \brief Set a 1D \c double array of 2 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
/// \param val3: 3rd value
/// \param val4: 4th value
/// \param val5: 5th value
/// \param val6: 6th value
  extern void ut_array_1d_set_6 (double *array, double val1, double val2,
                                 double val3, double val4, double val5,
                                 double val6);

/// \brief Set a 1D \c double array of 2 entries to values
/// \param array: array
/// \param val1: 1st value
/// \param val2: 2nd value
/// \param val3: 3rd value
/// \param val4: 4th value
/// \param val5: 5th value
/// \param val6: 6th value
/// \param val7: 7th value
  extern void ut_array_1d_set_7 (double *array, double val1, double val2,
                                 double val3, double val4, double val5,
                                 double val6, double val7);

/// \brief Get the number of duplicate entries of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return number of duplicate entries
/// \note Example: array = 1 2 3 1 2 returns 2 (because of the 2 last elements)
  extern int ut_array_1d_int_duplicates (int *array, int size);

#ifdef HAVE_GSL
/// \brief Set a 1D array of \c double to random values
/// \param array: array
/// \param size: size of the array
/// \param min: minimal value
/// \param max: maximal value
/// \param r: GSL random number algo
  extern void ut_array_1d_set_rand (double *array, int size, double min,
                                    double max, gsl_rng * r);

/// \brief Set a 1D array of \c int to random values
/// \param array: array
/// \param size: size of the array
/// \param min: minimal value
/// \param max: maximal value
/// \param r: GSL random number algo
  extern void ut_array_1d_int_set_rand (int *array, int size, int min,
                                        int max, gsl_rng * r);

/// \brief Choose randomly elements from a 1D array of \c int
/// \param srcarray: source array
/// \param srcsize: size of the source array
/// \param destarray: destination array
/// \param destsize: size of the destination array
/// \param r: GSL random number algo
  extern void ut_array_1d_int_choose (int *srcarray, int srcsize,
                                      int *destarray, int destsize,
                                      gsl_rng * r);
#endif

/// \brief Round a 1D array of \c double
/// \param array: array
/// \param size: size of the array
/// \param array2: result array
  extern void ut_array_1d_round (double *array, int size, int *array2);

/// \brief Round a 1D array of \c double by keeping the sum constant
/// \param array: array
/// \param size: size of the array
/// \param array2: result array
  extern void ut_array_1d_round_keepsum (double *array, int size,
                                         double *array2);

/// \brief Get the absolute value of a 1D array of \c double
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_abs (double *array, int size);

/// \brief Get the absolute value of a 1D array of \c int
/// \param array: array
/// \param size: size of the array
  extern void ut_array_1d_int_abs (int *array, int size);

/// \brief Move an element of a 1D array of \c int to the beginning
/// \param array: array
/// \param id: index of the element
/// \note All elements that were before are shifted by 1 index
  extern void ut_array_1d_int_bubbleelt (int *array, int pos);

/// \brief Read a number from a character string and wrote with a comma as decimal point
/// \param num: number as a character string
/// \return number
  extern int ut_array_rotpos (int beg, int end, int pos, int near);

/// \brief Read a 1D array of \c double
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_fscanf (FILE * file, double *array, int size);

/// \brief Read a 1D array of \c int
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_int_fscanf (FILE * file, int *array, int size);

/// \brief Read a 1D array of \c char*
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_string_fscanf (FILE * file, char **array, int size);

/// \brief Read a 2D array of \c double
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return 1 on success, undefined otherwise
  extern int ut_array_2d_fscanf (FILE * file, double **array, int size1,
                                 int size2);

/// \brief Read a 2D array of \c int
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return 1 on success, undefined otherwise
  extern int ut_array_2d_int_fscanf (FILE * file, int **array, int size1,
                                     int size2);

/// \brief Read a 1D array of \c double, from filename
/// \param filename: file name
/// \param array: array
/// \param size: size of the array
/// \param mode: file opening mode, "r" (verbose) or "R" (not verbose)
/// \note filename can be written as filename,col=... to fscanf only a specific column of a file.
/// \note col=0 assigns 0 and col=-x assigns the opposite values of column x
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_fnscanf (char *filename, double *array, int size, char *mode);

/// \brief Read a 1D array of \c int, from filename
/// \param filename: file name
/// \param array: array
/// \param size: size of the array
/// \param mode: file opening mode, "r" (verbose) or "R" (not verbose)
/// \note filename can be written as filename,col=... to fscanf only a specific column of a file.
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_int_fnscanf (char *filename, int *array, int size, char *mode);

/// \brief Read a 2D array of \c double, from filename
/// \param filename: file name
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param mode: file opening mode, "r" (verbose) or "R" (not verbose)
/// \note filename can be written as filename,col=... to fscanf only specific columns of a file, e.g. filename,col=1:2:6
/// \note col=0 assigns 0 and col=-x assigns the opposite values of column x
/// \return 1 on success, undefined otherwise
  extern int ut_array_2d_fnscanf (char *filename, double **array, int size1,
                                  int size2, char *mode);

/// \brief Read a 2D array of \c int, from filename
/// \param filename: file name
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param mode: file opening mode, "r" (verbose) or "R" (not verbose)
/// \note filename can be written as filename,col=... to fscanf only specific columns of a file, e.g. filename,col=1:2:6
/// \return 1 on success, undefined otherwise
  extern int ut_array_2d_int_fnscanf (char *filename, int **array, int size1,
                                      int size2, char *mode);

/// \brief Read a sample of a 1D array of \c double
/// \param file: file pointer
/// \param realsize: real size of the array (in file)
/// \param fact: sampling factor
/// \param *parray: array (non preallocated)
/// \param *psize: size of array
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_fscanf_sample (FILE * file, int realsize,
                                        double fact, double **parray,
                                        int *psize);

/// \brief Read a sample of a 2D array of \c double
/// \param file: file pointer
/// \param realsize1: real 1st-index size of the array (in file)
/// \param realsize2: real 2nd-index size of the array (in file)
/// \param *parray: array (non preallocated)
/// \param *psize1: 1st-index size of array
/// \return 1 on success, undefined otherwise
  extern int ut_array_2d_fscanf_sample (FILE * file, int realsize1,
                                        int realsize2, double fact,
                                        double ***parray, int *psize);

/// \brief Read a string using a line prefix
/// \param file: file pointer
/// \param string: character string
/// \param prefix: line prefix ("none" for none)
/// \return 1 on success, undefined otherwise
  extern int ut_array_0d_string_fscanf_prefix (FILE * file, char *string,
                                               char *prefix);

/// \brief Read a \c double using a line prefix
/// \param file: file pointer
/// \param *pval: value
/// \param prefix: line prefix ("none" for none)
/// \return 1 on success, undefined otherwise
  extern int ut_array_0d_fscanf_prefix (FILE * file, double *pval,
                                        char *prefix);

/// \brief Read a 1D array of \c double using a line prefix
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param prefix: line prefix ("none" for none)
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_fscanf_prefix (FILE * file, double *array,
                                        int size, char *prefix);

/// \brief Read a 1D array of \c int using a line prefix
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param prefix: line prefix ("none" for none)
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_int_fscanf_prefix (FILE * file, int *array,
                                            int size, char *prefix);

/// \brief Read a 1D array of \c char strings using a line prefix
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param prefix: line prefix ("none" for none)
/// \return 1 on success, undefined otherwise
  extern int ut_array_1d_string_fscanf_prefix (FILE * file, char **array,
                                               int size, char *prefix);

/// \brief Read a 2D array of \c double using a line prefix
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param prefix: line prefix ("none" for none)
/// \return 1 on success, undefined otherwise
  extern int ut_array_2d_fscanf_prefix (FILE * file, double **array,
                                        int size1, int size2, char *prefix);

/// \brief Read a 2D array of \c int using a line prefix
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param prefix: line prefix ("none" for none)
/// \return 1 on success, undefined otherwise
  extern int ut_array_2d_int_fscanf_prefix (FILE * file, int **array,
                                            int size1, int size2,
                                            char *prefix);

/// \brief Read a 1D array of \c double, from filename, enable wildcard
/// \param filename: file name
/// \param array: array
/// \param size: 1st-index size of the array
/// \param wcard: wildcard
/// \param mode: file opening mode, "r" (verbose) or "R" (not verbose)
/// \return 1 on success, undefined otherwise
/// \note If file \c filename exists, the data are read from it.  Otherwise, \c wcard is used.  \c wcard can be: "numeral" for initializing \c array from a number, "color" for initializing \c array from the RGB values of \c filename (if a valid color name; if equal to "id", the color palette is used)
  extern int ut_array_1d_fnscanf_wcard (char *filename, double *array,
                                        int size, char *wcard, char *mode);

/// \brief Read a 1D array of \c int, from filename, enable wildcard
/// \param filename: file name
/// \param array: array
/// \param size: 1st-index size of the array
/// \param wcard: wildcard
/// \param mode: file opening mode, "r" (verbose) or "R" (not verbose)
/// \return 1 on success, undefined otherwise
/// \note If file \c filename exists, the data are read from it.  Otherwise, \c wcard is used.  \c wcard can be: "numeral" for initializing \c array from a number, "color" for initializing \c array from the RGB values of \c filename (if a valid color name; if equal to "id", the color palette is used)
  extern int ut_array_1d_int_fnscanf_wcard (char *filename, int *array,
                                            int size, char *wcard, char *mode);

/// \brief Read a 2D array of \c double, from filename, enable wildcard
/// \param filename: file name
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param wcard: wildcard
/// \param mode: file opening mode, "r" (verbose) or "R" (not verbose)
/// \return 1 on success, undefined otherwise
/// \note If file \c filename exists, the data are read from it.  Otherwise, \c wcard is used.  \c wcard can be: "numeral" for initializing \c array from a number, "color" for initializing \c array from the RGB values of \c filename (if a valid color name; if equal to "id", the color palette is used)
  extern int ut_array_2d_fnscanf_wcard (char *filename, double **array,
                                        int size1, int size2, char *wcard, char *mode);

/// \brief Write a 1D array of \c double
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param format: format (usually "%f")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end.
/// \note \c format can be %d, in which case values rounded to the nearest integers are written.
  extern int ut_array_1d_fprintf (FILE * file, double *array, int size,
                                  const char *format);

/// \brief Write a 1D array of \c double on one column
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param format: format (usually "%f")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end.
/// \note \c format can be %d, in which case values rounded to the nearest integers are written.
  extern int ut_array_1d_fprintf_column (FILE * file, double *array, int size,
                                  const char *format);

/// \brief Write a 1D array of \c double on one column
/// \param filename: file name
/// \param array: array
/// \param size: size of the array
/// \param format: format (usually "%f")
/// \param mode: file opening mode, "w" (verbose) or "W" (not verbose)
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end.
/// \note \c format can be %d, in which case values rounded to the nearest integers are written.
  extern int ut_array_1d_fnprintf_column (char *filename, double *array, int size,
                                  const char *format, char *mode);

/// \brief Write a 1D array of \c int
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param format: format (usually "%d")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end.
  extern int ut_array_1d_int_fprintf (FILE * file, int *array, int size,
                                      const char *format);

/// \brief Write a 1D array of \c int in reverse order
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param format: format (usually "%d")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end.
  extern int ut_array_1d_int_fprintf_reverse (FILE * file, int *array, int size,
                                              const char *format);

/// \brief Write a 1D array of \c double
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param format: format (usually "%f")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end.
/// \note \c format can be %d, in which case values rounded to the nearest integers are written.
  extern int ut_array_1d_fprintf_nonl (FILE * file, double *array, int size,
                                       const char *format);

/// \brief Write a 1D array of \c int
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param format: format (usually "%d")
/// \return 1 on success, undefined otherwise
/// \note No newline is written at the end.
  extern int ut_array_1d_int_fprintf_nonl (FILE * file, int *array, int size,
                                           const char *format);

/// \brief Write a 1D array of \c unsigned \c int
/// \param file: file pointer
/// \param array: array
/// \param size: size of the array
/// \param format: format (usually "%ld")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end.
  extern int ut_array_1d_uint_fprintf (FILE * file, unsigned int *array,
                                       int size, const char *format);

/// \brief Write a 2D array of \c double
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param format: format (usually "%f")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end of each line.
  extern int ut_array_2d_fprintf (FILE * file, double **array, int size1,
                                  int size2, const char *format);

/// \brief Write a 2D array of \c int
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param format: format (usually "%d")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end of each line.
  extern int ut_array_2d_int_fprintf (FILE * file, int **array, int size1,
                                      int size2, const char *format);

/// \brief Write a 2D array of \c double
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param format: format (usually "%f")
/// \return 1 on success, undefined otherwise
/// \note All data are written on a single line, and a newline is written at the end.
  extern int ut_array_2d_fprintf_oneline (FILE * fileid, double **array,
                                          int size1, int size2,
                                          const char *format);

/// \brief Write specific columns of 2D array of \c double
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param cols: column indices
/// \param colnb: number of columns
/// \param format: format (usually "%f")
/// \return 1 on success, undefined otherwise
  extern int ut_array_2d_fprintf_col (FILE * file, double **array, int size1,
                                      int size2, int *cols, int colnb,
                                      const char *format);

/// \brief Write a non-square 2D array of \c int
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: array containing the 2nd-index sizes of the array
/// \param format: format (usually "%d")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end of each line.
  extern int ut_array_2dns_int_fprintf (FILE * file, int **array, int size1,
                                        int *size2, const char *format);

/// \brief Write a 3D array of \c int
/// \param file: file pointer
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param format: format (usually "%d")
/// \return 1 on success, undefined otherwise
/// \note A newline is written at the end of each line.
  extern int ut_array_3d_int_fprintf (FILE * file, int ***array, int size1,
                                      int size2, int size3,
                                      const char *format);

#endif                          /* UT_ARRAY_H */

#ifdef __cplusplus
}
#endif
