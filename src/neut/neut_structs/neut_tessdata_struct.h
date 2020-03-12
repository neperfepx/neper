/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTTESSDATA_H
#define STRUCTTESSDATA_H

  struct TESSDATA
  {
    int SeedQty;
    int VerQty;
    int EdgeQty;
    int FaceQty;
    int PolyQty;

    double ***ColData;
    char **ColDataType;
    char **ColScheme;
    int ***Col;
    double ***trsdata;
    char **trsdatatype;
    char **trscheme;
    double **trs;
    double ***RadData;
    char **RadDataType;
    double **Rad;
    char **Scale;
    char **ScaleTitle;
  };
  typedef struct TESSDATA TESSDATA;

#endif				/* STRUCTTESSDATA_H */

#ifdef __cplusplus
}
#endif
