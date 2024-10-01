/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPRIM_H
#define STRUCTPRIM_H

  struct PRIM
  {
    // plane, sphere, cylinder, torus
    char *Type;

    // parameters
    int ParmQty;
    double *Parms;

    // equation (for plane)
    double *Eq;

    // base point
    // = centre for spheres
    // = base point for cylinder
    double *Base;

    // normal vector
    // for plane and cylinder
    double *Dir;

    // radius
    // for sphere, cylinder and torus
    double *Rad;
  };
  typedef struct PRIM PRIM;

#endif				/* STRUCTPRIM_H */

#ifdef __cplusplus
}
#endif
