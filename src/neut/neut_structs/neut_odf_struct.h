/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTODF_H
#define STRUCTODF_H

#include"neut.h"

struct ODF
{
  // grid
  char *gridtype;
  struct NODES Nodes;
  struct MESH *Mesh;

  // smoothing
  double sigma;

  // values
  int odfqty;
  double *odf;
  int odfnqty;
  double *odfn;

  double odfmin;
  double odfmax;
  double odfmean; // 1
  double odfsig;

  struct FCT hfct;
};
typedef struct ODF ODF;

#endif /* STRUCTODF_H */
