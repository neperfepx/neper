/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPOINT_H
#define STRUCTPOINT_H

  struct POINT
  {
    int Dim;
    int *Periodic;
    double **BBox;

    int PointQty;
    double **PointCoo;
    double *PointRad;
  };
  typedef struct POINT POINT;

#endif				/* STRUCTPOINT_H */

#ifdef __cplusplus
}
#endif
