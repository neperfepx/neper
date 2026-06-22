/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTORI_H
#define STRUCTORI_H

  struct ORI
  {
    char *space;            // space (rodrigues or euler)
    char *crysym;           // crystal symmetry

    char *mode;             // density or symbol (or combination)
    char *layout;           // layout (default surface,slices)
    char *field;            // field to print(?)

    // For tracking

    int inputqty;           // number of inputs
    char **inputs;          // inputs

    struct OSPACE Sp;       // orientation space

    struct SIM *pSim;       // pointer to SIM structure
  };
  typedef struct ORI ORI;

#endif				/* STRUCTORI_H */

#ifdef __cplusplus
}
#endif
