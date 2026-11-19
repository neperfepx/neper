/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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
    int delcell;
    double fmax;

    char *seltype;

    double *PolySel;
    double *FaceSel;
    double *EdgeSel;

    // vertices of the straight corners of the faces of the initial
    // tessellation (e.g. of a multiscale tessellation), which can become
    // slightly reflex, [1...FaceQty][0...]: [i][0] is the number of
    // vertices of face i, and [i][1...] are the vertices
    int FaceQty;
    int **FaceStraightVers;
  };
  typedef struct REG REG;

#endif				/* STRUCTREG_H */

#ifdef __cplusplus
}
#endif
