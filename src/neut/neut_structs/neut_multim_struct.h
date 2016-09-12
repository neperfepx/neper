/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTMULTIM_H
#define STRUCTMULTIM_H

  struct MULTIM
  {
    int algoqty;

    char ***algos;
    int *algohit;

    double **mOdis;
    double **mOsize;
    double **mO;

    int *Oalgo;
    double *Odis;
    double *Osize;
    double *O;
  };
  typedef struct MULTIM MULTIM;

#endif				/* STRUCTMULTIM_H */

#ifdef __cplusplus
}
#endif
