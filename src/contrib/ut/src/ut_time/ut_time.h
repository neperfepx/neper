/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_TIME_H
#define UT_TIME_H

/// \file ut_time.h
/// \brief Time-related functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Subtract two times
/// \param timeval1: start time
/// \param timeval2: end time
/// \return time difference
extern double ut_time_subtract (struct timeval* timeval1, struct timeval* timeval2);

#endif /* UT_TIME_H */

#ifdef __cplusplus
}
#endif
