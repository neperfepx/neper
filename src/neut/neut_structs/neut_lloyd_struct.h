/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTLLOYD_H
#define STRUCTLLOYD_H

  struct LLOYD
  {
    // parameters
    int itermax;	// max number of iterations
    double val;		// max value
    double fact;	// convergence factor
    char* norm;		// norm to determine objval from the seed-centroid
                        // distances

    int dimqty;		// number of dimensions on which to operate
    int *dims;		// dimensions on which to operate (0:x, 1:y, 2:z, 3:w)

    // current values
    int iter;		// iteration number
    double objval;	// current value of the residual
  };
  typedef struct LLOYD LLOYD;

#endif				/* STRUCTLLOYD_H */

#ifdef __cplusplus
}
#endif
