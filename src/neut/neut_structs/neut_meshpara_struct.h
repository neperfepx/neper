/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTMESHPARA_H
#define STRUCTMESHPARA_H

  struct MESHPARA
  {
    char input;
    char *elttype;
    int dim;
    double cellsize;

    /// \brief Type of cl input data
    /// \details can be "abs" (absolute) or "rel" (relative to average
    /// grain size)
    char *cltype;

    /// \brief Argument of option -cl or -rcl
    double *clratio;

    double clmin;		// minimim value of cl
    double pl;			// value of -pl,

    double *poly_cl;
    double *face_cl;
    double *edge_cl;
    double *ver_cl;

    double *mesh3dclreps;

    /// "copy" or NULL
    char **edge_op;

    // "meshproj": projection onto plane of equation provided in face_eq,
    // back-projection onto existing mesh along normal provided in face_eq + 1
    // (for remeshing)
    // "planeproj": projection onto plane (for standard or regularization) of
    // equation provided in face_eq
    // "copy": copy mesh given in input (for a failed remeshing)
    char **face_op;

    // face equation (4 parameters) for the (orthogonal) projection of the
    // boundary
    double **face_eq;
  };
  typedef struct MESHPARA MESHPARA;

#endif				/* STRUCTMESHPARA_H */

#ifdef __cplusplus
}
#endif
