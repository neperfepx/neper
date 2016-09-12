/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_MATH_H
#define UT_MATH_H

extern int ut_math_eval_ww (char*, int, char**, double*, double*);
extern int ut_math_eval (char*, int, char**, double*, double*);
extern int ut_math_eval_int (char*, int, char**, double*, int*);
extern int ut_math_eval_substitute (char*, char**);
extern int ut_math_eval_substitute_max (char*, char**);
extern int ut_math_eval_substitute_min (char*, char**);
extern int ut_math_eval_subeval (char*, char*, char*, char*);

#endif /* UT_MATH_H */

#ifdef __cplusplus
}
#endif
