/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTTESRDATA_H
#define STRUCTTESRDATA_H

  struct TESRDATA
  {
    char *type;
    int Qty;

    char *ColDataType;
    double **ColData;
    int *ColDataDef;		// 1 if data defined, 0 otherwise
    int **Col;
    char *ColScheme;

    double **trsdata;
    char *trsdatatype;
    char *trscheme;
    double *trs;

    char *Scale;
    char *ScaleTitle;

    int *BCol;
    double BRad;
  };
  typedef struct TESRDATA TESRDATA;

#endif				/* STRUCTTESRDATA_H */

#ifdef __cplusplus
}
#endif
