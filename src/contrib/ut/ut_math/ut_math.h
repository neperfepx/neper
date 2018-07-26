/* Copyright (C) 2003-2018, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_MATH_H
#define UT_MATH_H

extern int ut_math_eval (char*, int, char**, double*, double*);
extern int ut_math_eval_int (char*, int, char**, double*, int*);
extern int ut_math_derivative_onesided (double *v, int order_acc, double *pres);
extern int ut_math_vars (char* expr, char*** pvars, int *pvarqty);
extern void ut_math_functions (char ***pfcts, int *pfctqty);
extern int ut_math_string_isfunction (char *string);

#endif /* UT_MATH_H */

#ifdef __cplusplus
}
#endif
