/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPOINT_H
#define STRUCTPOINT_H

  struct POINT
  {
    char *Name;

    char *Type;
    int Dim;

    int Qty;
    double **Coo;

    double *Rad;

    char *crysym; // crystal symmetry, if relevant

    // pointer to SIM structure
    struct SIM *pSim;
  };
  typedef struct POINT POINT;

#endif				/* STRUCTPOINT_H */

#ifdef __cplusplus
}
#endif
