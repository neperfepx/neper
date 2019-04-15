/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTREG_H
#define STRUCTREG_H

  struct REG
  {
    int mloop;
    int maxedgedelqty;
    double fmax;

    char *seltype;

    double *PolySel;
    double *FaceSel;
    double *EdgeSel;
  };
  typedef struct REG REG;

#endif				/* STRUCTREG_H */

#ifdef __cplusplus
}
#endif
