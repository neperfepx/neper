/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTPOINTDATA_H
#define STRUCTPOINTDATA_H

struct POINTDATA
{
  int PointQty;

  double **ColData;
  char *ColDataType;
  char *ColScheme;
  int **Col;
  double **trsdata;
  char *trsdatatype;
  char *trscheme;
  double *trs;
  double **RadData;
  char *RadDataType;
  double *Rad;
  char *Scale;
  char *ScaleTitle;

  double **CooData;
  char *CooDataType;
  double **Coo;
  double CooFact;

  double *Axes;

  char *Space;
};
typedef struct POINTDATA POINTDATA;

#endif /* STRUCTPOINTDATA_H */
