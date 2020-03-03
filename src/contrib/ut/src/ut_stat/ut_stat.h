/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_STAT_H
#define UT_STAT_H

/// \file ut_stat.h
/// \brief Statistics-related functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Compute Kolmogorov–Smirnov test from 2 distributions
/// \param dis1: distribution 1
/// \param dis2: distribution 2
/// \param *pres: test
/// \return 0
extern int ut_stat_test_ks (double* dis1, double* dis2, int qty, double* pres);

/// \brief Compute Kuiper's test from 2 distributions
/// \param dis1: distribution 1
/// \param dis2: distribution 2
/// \param *pres: test
/// \return 0
extern int ut_stat_test_kuiper (double* dis1, double* dis2, int qty, double* pres);

/// \brief Compute the parameters of the distribution of values of a 1D \c double array
/// \param array: array
/// \param size: size of the array
/// \param step: step of the distribution
/// \param *pfirst: first element of the distribution
/// \param *plength: number of element (x-axis) of the distribution
extern void ut_stat_distribparam (double *array, int size, double step,
                                  double *pfirst, int *plength);

/// \brief Compute the distribution of values of a 1D \c double array
/// \param array: array
/// \param size: size of the array
/// \param step: step of the distribution
/// \param first: first element of the distribution
/// \param length: number of element (x-axis) of the distribution
/// \param distrib: distribution (must be preallocated)
extern void ut_stat_distrib (double *array, int size, double step, double first,
                             int length, double *distrib);

/// \brief Transform an array of \c int into percents
/// \param array: array
/// \param size: size of the array
/// \param percent: array of percents
/// \note The sum equals 100 (no rounding error)
extern void ut_stat_scaletopercent_int (int *array, int size, int *percent);

#endif /* UT_STAT_H */

#ifdef __cplusplus
}
#endif
