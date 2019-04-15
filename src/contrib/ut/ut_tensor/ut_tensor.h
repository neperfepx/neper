/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_TENSOR_H
#define UT_TENSOR_H

extern void ut_tensor_vec2leq (double *, double *);
extern void ut_tensor_leq2vec (double *, double *);
extern void ut_tensor_std2leq (double *, double *);
extern void ut_tensor_leq2std (double *, double *);
extern void ut_tensor_leq2mat (double *, double **);
extern void ut_tensor_mat2leq (double **, double *);

#endif /* UT_TENSOR_H */

#ifdef __cplusplus
}
#endif
