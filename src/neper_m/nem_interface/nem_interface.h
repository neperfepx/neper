/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

// interface types
#define NEM_INTERFACE_CONTINUOUS 0
#define NEM_INTERFACE_DISCONTINUOUS 1
#define NEM_INTERFACE_COHESIVE 2

extern void nem_interface (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
                          struct MESH *Mesh, struct BOUNDARY *pBound);
