/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTOSPACE_H
#define STRUCTOSPACE_H

#include "neut_struct_tess.h"
#include "neut_struct_nodes.h"
#include "neut_struct_mesh.h"

  struct OSPACE
  {
    // Space

    char *space;           // rodrigues or homochoric or euler-bunge
    char *spaceunit;       // radian or degree, for space=euler

    // Crystal symmetry

    char* crysym;          // crystal symmetry (cubic, hexagonal, etc.)

    // Fundamental region (FR)

    char *fr;              // built-in FR name, default "fr20"

    struct TESS Tess;      // FR's tessellation
    struct NODES Nodes;    // FR's nodes
    struct MESH *Mesh;     // FR's meshes
    void *pqcloud;         // pointer to the nanoflann cloud (elemental orientations)
    void *pqtree;          // pointer to the nanoflann tree (elemental orientations)
    void *pqcloudn;        // pointer to the nanoflann cloud (nodal orientations)
    void *pqtreen;         // pointer to the nanoflann tree (nodal orientations)

    struct FCT hfct;
    struct FCT thetafct;
  };
  typedef struct OSPACE OSPACE;

#endif /* STRUCTOSPACE_H */

#ifdef __cplusplus
}
#endif
