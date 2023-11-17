/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTODF_H
#define STRUCTODF_H

struct ODF
{
  // grid
  char *gridtype;
  char *gridunit;
  struct NODES Nodes;
  struct MESH *Mesh;

  // smoothing
  double sigma; // radian

  // values
  int odfqty;
  double *odf; // 0-indexed
  int odfnqty;
  double *odfn; // 0-indexed

  // Weight of each element, in terms of corrected volume, normalized
  double *EltWeight; // indexed 0

  double odfmin;
  double odfmax;
  double odfmean; // 1
  double odfsig;

  struct FCT hfct;
};
typedef struct ODF ODF;

#endif /* STRUCTODF_H */

#ifdef __cplusplus
}
#endif
