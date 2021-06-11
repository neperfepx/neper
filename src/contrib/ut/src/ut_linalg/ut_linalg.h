/* Copyright (C) 2003-2021, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_LINALG_H
#define UT_LINALG_H

/// \file ut_linalg.h
/// \brief Linear-algebra functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Solve a linear system, Ax = B, by LU decomposition
/// \param A: matrix A
/// \param B: vector B
/// \param size: dimension of A and B (A nxn, B n)
/// \param x: vector x, solution
/// \return 0 on success, -1 on failure
/// \note Use the GSL
  extern int ut_linalg_solve_LU (double **A, double *B, int size, double *X);

#endif                          /* UT_LINALG_H */

#ifdef __cplusplus
}
#endif
