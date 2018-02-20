/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTCSYSDATA_H
#define STRUCTCSYSDATA_H

  struct CSYSDATA
  {
    double *ColData;
    char *ColDataType;
    int *Col;
    double RadData;
    char *RadDataType;
    double Rad;
    double LengthData;
    char *LengthDataType;
    double Length;

    double *CooData;
    char *CooDataType;
    double *Coo;

    char **Label;
    double FontSize;
  };
  typedef struct CSYSDATA CSYSDATA;

#endif				/* STRUCTCSYSDATA_H */

#ifdef __cplusplus
}
#endif
