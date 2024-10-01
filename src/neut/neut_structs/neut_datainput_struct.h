/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTDATAINPUT_H
#define STRUCTDATAINPUT_H

  struct DATAINPUT
  {
    char *input;

    struct SIM   *pSim;
    struct TESS  *pTess;
    struct TESR  *pTesr;
    struct NODES *pNodes;
    struct MESH **pMesh;
    struct POINT *pPoints;
  };
  typedef struct DATAINPUT DATAINPUT;

#endif				/* STRUCTDATAINPUT_H */

#ifdef __cplusplus
}
#endif
