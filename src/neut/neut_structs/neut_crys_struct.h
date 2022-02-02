/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTCRYS_H
#define STRUCTCRYS_H

/// \brief Tessellation structure
  struct CRYS
  {
    // crystal symmetry: cubic
    char *crysym;

    // elasticity constants
    // for cubic: C11, C12, C44
    double *C;
  };
  typedef struct CRYS CRYS;

#endif				/* STRUCTCRYS_H */

#ifdef __cplusplus
}
#endif
