/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_VECTOR_H
#define UT_VECTOR_H

/// \file ut_vector.h
/// \brief Vector functions
/// \author Romain Quey
/// \note Assume 3-component vector
/// \bug No known bugs

/// \brief Get the norm of a vector
/// \param vect: vector
/// \return norm
  extern double ut_vector_norm (double *vect);

/// \brief Get the norm of a vector, in 2D
/// \param vect: vector
/// \return norm
  extern double ut_vector_norm_2d (double *vect);

/// \brief Get the squared norm of a vector
/// \param vect: vector
/// \return squared norm
  extern double ut_vector_norm_squared (double *vect);

/// \brief Get the norm of a \c int vector
/// \param vect: vector
/// \return norm
  extern double ut_vector_int_norm (int *vect);

/// \brief Get the scalar product of two vectors
/// \param vect1: 1st vector
/// \param vect2: 2nd vector
/// \return scalar product
  extern double ut_vector_scalprod (double *vect1, double *vect2);

/// \brief Get the vectorial product of two vectors
/// \param vect1: 1st vector
/// \param vect2: 2nd vector
/// \param vect: result vector
  extern void ut_vector_vectprod (double *vect1, double *vect2, double *vect);

/// \brief Get a normalized vector from a vector
/// \param vect: vector
/// \param uvect: normalized vector
  extern void ut_vector_uvect (double *vect, double *uvect);

/// \brief Get the angle between 2 vectors
/// \param vect1: 1st vector
/// \param vect2: 2nd vector
/// \return angle (in degrees)
  extern double ut_vector_angle (double *vect1, double *vect2);

/// \brief Get the angle between 2 vectors, in radians
/// \param vect1: 1st vector
/// \param vect2: 2nd vector
/// \return angle (in radians)
  extern double ut_vector_angle_rad (double *vect1, double *vect2);

/// \brief Get the angle between 2 \c int vectors
/// \param vect1: 1st vector
/// \param vect2: 2nd vector
/// \return angle (in degrees)
  extern double ut_vector_int_angle (int *vect1, int *vect2);

/// \brief Get the angle between 2 \c int vectors, in radians
/// \param vect1: 1st vector
/// \param vect2: 2nd vector
/// \return angle (in radians)
  extern double ut_vector_int_angle_rad (int *vect1, int *vect2);

/// \brief Get the angle between 2 2D vectors
/// \param vect1: 1st vector
/// \param vect2: 2nd vector
/// \return angle (in degrees)
  extern double ut_vector_angle_2d (double *vect1, double *vect2);

/// \brief Get the angle between 2 2D vectors, in radians
/// \param vect1: 1st vector
/// \param vect2: 2nd vector
/// \return angle (in radians)
  extern double ut_vector_angle_rad_2d (double *vect1, double *vect2);

/// \brief Get the covariant matrix of a set of vectors
/// \param vects: vectors
/// \param vectqty: number of vectors
/// \param dim: dimension
/// \param covar: covariant matrix
  extern void ut_vector_set_covar (double **vects, double *weight, int vectqty, int dim,
                                   double **covar);

/// \brief Get the eigendecomposition of the covariant matrix of a set of vectors
/// \param vects: vectors
/// \param vectqty: number of vectors
/// \param dim: dimension
/// \param evect: eigen vectors
/// \param eval: eigen values
  extern void ut_vector_set_eigen (double **vects, double *weight, int vectqty, int dim,
                                   double **evect, double *eval);

#endif                          /* UT_VECTOR_H */

#ifdef __cplusplus
}
#endif
