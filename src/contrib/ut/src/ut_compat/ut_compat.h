/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

/// \file ut_compat.h
/// \brief Compatibility with non-POSIX systems (Windows, MinGW-w64): POSIX
/// functions not provided by the system, mapped to their equivalents or
/// implemented (in ut_compat.c)

#ifndef UT_COMPAT_H
#define UT_COMPAT_H

#include <sys/time.h>           // struct timeval, gettimeofday

// null device
#ifdef _WIN32
#define UT_NULL_DEVICE "NUL"
#else
#define UT_NULL_DEVICE "/dev/null"
#endif

// OpenMP functions, when not using OpenMP (a single thread)
#ifndef _OPENMP
static inline int omp_get_thread_num (void) { return 0; }
static inline int omp_get_num_threads (void) { return 1; }
static inline int omp_get_max_threads (void) { return 1; }
static inline int omp_in_parallel (void) { return 0; }
static inline void omp_set_num_threads (int qty) { (void) qty; }
#endif

#ifdef _WIN32

#include <stdio.h>
#include <direct.h>

#ifdef __cplusplus
extern "C"
{
#endif

  // stdio locking
#define flockfile _lock_file
#define funlockfile _unlock_file
#define getc_unlocked _getc_nolock

  // directories (no permissions)
#define mkdir(path, mode) _mkdir (path)

  // reading a line (allocated as needed)
#include <sys/types.h>
  extern ssize_t getline (char **plineptr, size_t *pn, FILE *stream);

  // random numbers, as POSIX's (same sequences)
  extern double drand48 (void);
  extern double erand48 (unsigned short xsubi[3]);
  extern void srand48 (long seed);

#ifdef __cplusplus
}
#endif

#endif                          /* _WIN32 */

#endif                          /* UT_COMPAT_H */
