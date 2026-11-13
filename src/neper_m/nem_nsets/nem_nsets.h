/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nem_nsets (struct IN_M, struct TESS, struct TESR,
                       struct NODES, struct MESH*, struct NSET *);
extern void nem_nsets_custom (struct IN_M In, struct TESS Tess,
                              struct NODES Nodes, struct MESH *Mesh,
                              struct NSET *NSet);
