/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_NUM_H
#define UT_NUM_H

/// \file ut_num.h
/// \brief Number-related functions
/// \author Romain Quey
/// \bug No known bugs
/// \todo Clean implementation of \c ut_num_tenlen
/// \todo Rename \c ut_num_tenlen*

/// \brief Get the minimum of 2 \c double
/// \param num1: number 1
/// \param num2: number 2
/// \return minimum
  extern double ut_num_min (double num1, double num2);

/// \brief Get the minimum of 2 \c int
/// \param num1: number 1
/// \param num2: number 2
/// \return minimum
  extern int ut_num_min_int (int num1, int num2);

/// \brief Get the maximum of 2 \c double
/// \param num1: number 1
/// \param num2: number 2
/// \return minimum
  extern double ut_num_max (double num1, double num2);

/// \brief Get the maximum of 2 \c int
/// \param num1: number 1
/// \param num2: number 2
/// \return minimum
  extern int ut_num_max_int (int num1, int num2);

/// \brief Get the sign of a \c double
/// \param num: number
/// \return 1 if positive, -1 if negative
  extern int ut_num_sgn (double num);

/// \brief Get the sign of an \c int
/// \param num: number
/// \return 1 if positive, -1 if negative
  extern int ut_num_sgn_int (int num);

/// \brief Get the square root of a \c double
/// \param num: number
/// \return square root if positive, 0 otherwise
/// \note Useful to handle the case where num is below 0 to machine precision
  extern double ut_num_sqrt (double num);

/// \brief Round a \c double to the nearest \c int
/// \param num: number
/// \return nearest integer
  extern int ut_num_d2ri (double num);

/// \brief Get the number of digits of a \c double (non-decimal part)
/// \param num: number
/// \return number of digits
  extern int ut_num_tenlen (double num);

/// \brief Get the number of digits of an \c int
/// \param num: number
/// \return number of digits
  extern int ut_num_tenlen_int (int);

/// \brief Get the arc cosine a \c double
/// \param cos: cosine
/// \return arc cosine
/// \note Useful to handle the case where cos is above 1 or below -1 to machine precision
  extern double ut_num_acos (double cos);

/// \brief Get the absolute value of an \c int
/// \param num: number
/// \return absolute value
  extern int ut_num_fabs_int (int num);

/// \brief Check if two \c double are equal to a given absolute tolerance
/// \param num1: number 1
/// \param num2: number 2
/// \param tol: absolute tolerance
/// \return 1 if equal, 0 otherwise
  extern int ut_num_equal (double num1, double num2, double tol);

/// \brief Check if two \c double are equal to a given relative tolerance
/// \param num1: number 1
/// \param num2: number 2
/// \param tol: relative tolerance
/// \return 1 if equal, 0 otherwise
  extern int ut_num_requal (double num1, double num2, double tol);

/// \brief Check if an \c int is even
/// \param num: number
/// \return 1 if even, 0 otherwise
  extern int ut_num_even (int num);

/// \brief Check if an \c int is odd
/// \param num: number
/// \return 1 if odd, 0 otherwise
  extern int ut_num_odd (int num);

/// \brief Get the logarithm of a \c double
/// \param num: number
/// \return logarithm if > 0, -DBL_MAX otherwise
/// \note Useful to handle the case where num is <= 0 to machine precision
  extern double ut_num_log (double num);

/// \brief Reverse the endianness of a \c double
/// \param num: number
/// \return number after reversing its endianness
  extern double ut_num_reverseendian (double num);

/// \brief Bound a \c double
/// \param num: number
/// \param min: lower bound
/// \param max: upper bound
/// \return min if num < min, max if num > max, num otherwise
  extern double ut_num_bound (double num, double min, double max);

/// \brief Bound an \c int
/// \param num: number
/// \param min: lower bound
/// \param max: upper bound
/// \return min if num < min, max if num > max, num otherwise
  extern int ut_num_bound_int (int num, int min, int max);

/// \brief Switch 2 \c double
/// \param *pnum1: number 1
/// \param *pnum2: number 2
  extern void ut_num_switch (double *pnum1, double *pnum2);

/// \brief Switch 2 \c int
/// \param *pnum1: number 1
/// \param *pnum2: number 2
  extern void ut_num_switch_int (int *pnum1, int *pnum2);

/// \brief Read a number from a character string and wrote with a comma as decimal point
/// \param num: number as a character string
/// \return number
  extern double ut_num_readwcomma (char *num);

#endif                          /* UT_NUM_H */

#ifdef __cplusplus
}
#endif
