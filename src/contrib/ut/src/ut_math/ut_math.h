/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_MATH_H
#define UT_MATH_H

/// \file ut_math.h
/// \brief Mathematical functions
/// \note Use muparser
/// \author Romain Quey
/// \bug No known bugs

/// \brief Evaluate a mathematical expression
/// \param expr: expression
/// \param varqty: number of variables
/// \param vars: variables
/// \param vals: values of the variables
/// \param *pres: expression value
/// \return 0 on success, -1 on failure
  extern int ut_math_eval (char *expr, int varqty, char **vars, double *vals,
                           double *pres);

/// \brief Evaluate a mathematical expression, multiple
/// \param expr: expression
/// \param varqty: number of variables
/// \param vars: variables
/// \param evalqty: number of evaluations
/// \param *pres: expression values
/// \return 0 on success, -1 on failure
  extern int ut_math_evals (char *inexpr, int varqty, char **vars, double **vals,
                            int evalqty, double *res);

/// \brief Evaluate a mathematical expression as an \c int
/// \param expr: expression
/// \param varqty: number of variables
/// \param vars: variables
/// \param vals: values of the variables
/// \param *pres: expression value
/// \return 0 on success, -1 on failure
/// \note Value rounded to the nearest integer
  extern int ut_math_eval_int (char *expr, int varqty, char **vars,
                               double *vals, int *pres);

/// \brief Get the variables of a mathematical expression
/// \param expr: expression
/// \param *pvars: variables
/// \param *pvarqty: number of variables
/// \return 0
  extern int ut_math_vars (char *expr, char ***pvars, int *pvarqty);

  extern int ut_math_derivative_onesided (double *v, int order_acc,
                                          double *pres);

  extern int ut_math_eval_exprislogical (char *string);

#endif                          /* UT_MATH_H */

#ifdef __cplusplus
}
#endif
