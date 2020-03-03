/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTNODEDATA_H
#define STRUCTNODEDATA_H

  struct NODEDATA
  {
    int NodeQty;

    double **ColData;
    char *ColDataType;
    char *ColScheme;
    int **Col;
    double **RadData;
    char *RadDataType;
    double *Rad;
    char *Scale;
    char *ScaleTitle;

    double **CooData;
    char *CooDataType;
    double **Coo;
    double CooFact;
  };
  typedef struct NODEDATA NODEDATA;

#endif				/* STRUCTNODEDATA_H */

#ifdef __cplusplus
}
#endif
