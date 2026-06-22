/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTODF_H
#define STRUCTODF_H

#include "neut_struct_ospace.h"

  struct ODF
  {
    struct OSPACE Sp;         // orientation space

    double sigma;            // standard deviation of the kernel (in radian)

    // All arrays are 0-indexed
    int odfqty;              // number of elemental values (= Sp.Mesh[3].EltQty)
    double *odf;             // elemental values
    int odfnqty;             // number of nodal values (= Sp.Nodes.NodeQty)
    double *odfn;            // nodal values

    double odfmin;           // minimum elemental value
    double odfmax;           // maximum elemental value
    double odfmean;          // mean    elemental value
    double odfsig;           // standard deviation
  };
  typedef struct ODF ODF;

#endif /* STRUCTODF_H */

#ifdef __cplusplus
}
#endif
