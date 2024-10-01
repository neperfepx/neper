/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPF_H
#define STRUCTPF_H

  struct PF
  {
    char *space; // pf or ipf
    char *shape;
    char *projection;
    char *refsym;
    char *font;
    char *mode;
    int clustering;

    double **pfdir;
    double pfkernelsig;
    char *pfpolelabel;
    char *pfprojlabel;
    char *pfpolestring;
    int pfpoleqty;
    double **pfpoles; // [1...]

    // for tracking
    int inputqty;
    char **inputs;

    int *inputcellqty;
    double ***inputcellori;
    int **inputcellpole;

    // ipf border
    int ipfptqty;
    double **ipfpts;
    // same after 1.001 expansion and centering
    int domainptqty;
    double **domainpts;

    // crystal symmetry
    char *crysym;

    // gridsize
    int *gridsize;
    double ***grid;  // grid
    double ***Pgrid; // back-projected grid (on the sphere)
    double ***lgrid; // enlarged grid
  };
  typedef struct PF PF;

#endif				/* STRUCTPF_H */

#ifdef __cplusplus
}
#endif
