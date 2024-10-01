/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_TENSOR_H
#define UT_TENSOR_H

/// \file ut_tensor.h
/// \brief Tensor functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Transform a vector into a Lequeu vector
/// \param vect: 5-component vector
/// \param lvect: 5-component lequeu vector
  extern void ut_tensor_vec2leq (double *vect, double *lvect);

/// \brief Transform a Lequeu vector into a vector
/// \param lvect: 5-component lequeu vector
/// \param vect: 5-component vector
  extern void ut_tensor_leq2vec (double *lvect, double *vect);

/// \brief Transform a vector into a tensor
/// \param lvect: 5-component lequeu vector
/// \param tens: 3x3 tensor
  extern void ut_tensor_leq2tens (double *lvect, double **tens);

/// \brief Transform a tensor into a Lequeu vector
/// \param tens: 3x3 tensor
/// \param lvect: 5-component lequeu vector
  extern void ut_tensor_ten2leq (double **tens, double *lvec);

#endif                          /* UT_TENSOR_H */

#ifdef __cplusplus
}
#endif
