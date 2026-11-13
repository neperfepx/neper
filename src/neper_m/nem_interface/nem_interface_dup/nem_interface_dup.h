/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nem_interface_dup (struct TESS Tess, int *intertypes, struct NODES *pNodes,
                               struct MESH *Mesh, struct BOUNDARY *pBound, int ***pnodeseeds);

extern void nem_interface_dup_node_seed_copy (struct NODES Nodes, int **nodeseeds, int node,
                                              int seed, int *pcopy);
