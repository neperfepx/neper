/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTDATA_H
#define STRUCTDATA_H

  struct DATA
  {
    int *qty;

    char *entity;
    char *type;

    double **val;

    // size stuff
    double *size;

    // colour stuff
    int **Col;
    char *colourscheme;
    char *min;
    char *max;
    double height;
    char *ticks;
  };
  typedef struct DATA DATA;

#endif				/* STRUCTDATA_H */

#ifdef __cplusplus
}
#endif
