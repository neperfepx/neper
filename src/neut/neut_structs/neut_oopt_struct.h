/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTOOPT_H
#define STRUCTOOPT_H

struct OOPT
{
  // reference function
  struct FCT thetafct;

  // input data
  int n;                                // number of orientations
  int id;                               // identifier
  char *crysym;                         // crystal symmetry
  unsigned int nc;                      // number of crystallographic variants (24 for cubic)

  double neighd;                        // neighbour cutoff distance - geodesic
  double neighdE;                       // neighbour cutoff distance - Euclidean

  char *orioptistop;                    // termination criterion
  char *orioptineigh;                   // neighbour definition character string

  int fixoriqty;                        // number of fixed orientations
  int *fixori;                          // fixori[0...n-1] = 1 means fixed, 0 otherwise

  char *logvar;                         // logvar string
  FILE *logvar_fp;	                // variable log file

  // current state information
  double avforce_ini;                   // initial average force
  double avforce;                       // current average force
  double f;                             // value of the residual
  double fmin;                          // minimal value of the residual
  double avneighqty;                    // current average number of neighbours

  // profiling information
  struct timeval beg_time;              // start time
  struct timeval end_time;              // end time
  double force_dur;                     // force computation duration
  double potential_dur;                 // potential computation duration
  double rot_dur;                       // rotation duration
  double unrot_dur;                     // unrotation duration
  double total_dur;                     // total duration
};
typedef struct OOPT OOPT;

#endif /* STRUCTOOPT_H */
