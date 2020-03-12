/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file ut_nlopt.h
/// \brief NLopt-related functions
/// \author Romain Quey
/// \bug No known bugs
/// \todo Function not documentated because of \c #ifdef

#ifndef UT_NLOPT_H
#define UT_NLOPT_H

#ifdef HAVE_NLOPT
#include<nlopt.h>

/// \brief Set NLopt algorithm variable from name
/// \param optialgo: algorithm
/// \return pointer to the nlopt algorithm variable
  extern void ut_nlopt_algo (char *optialgo, nlopt_algorithm * palgo);

#endif                          // HAVE_NLOPT

#endif                          /* UT_NLOPT_H */

#ifdef __cplusplus
}
#endif
