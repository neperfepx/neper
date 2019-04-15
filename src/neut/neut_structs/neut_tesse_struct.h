/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTTESSE_H
#define STRUCTTESSE_H

/// \brief Tessellation discretization structure
  struct TESSE
  {
    int VerQty;
    int *VerFVer;

    int EdgeQty;
    int *EdgeFEdgeQty;
    int **EdgeFEdgeNb;
    int **EdgeFEdgeOri;

    int FaceQty;
    int *FaceFFaceQty;
    int **FaceFFaceNb;
    int **FaceFFaceOri;

    int PolyQty;
    int *PolyFPolyQty;
    int **PolyFPolyNb;
  };
  typedef struct TESSE TESSE;

#endif				/* STRUCTTESSE_H */

#ifdef __cplusplus
}
#endif
