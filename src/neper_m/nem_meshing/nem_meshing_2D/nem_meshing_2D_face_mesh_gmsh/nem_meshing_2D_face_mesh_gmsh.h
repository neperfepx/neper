/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern int nem_meshing_2D_face_mesh_gmsh (struct TESS Tess, struct NODES
                                          RNodes, struct MESH *RMesh,
                                          int face, struct MESHPARA MeshPara,
                                          struct NODES Nodes,
                                          struct MESH *Mesh, double cl,
                                          char *gmsh, char *tmp, char *algo,
                                          double rnd, double allowed_t,
                                          struct NODES *pN, struct MESH *pM,
                                          int **pbnodes, int **plbnodes,
                                          int *pbnodeqty, double *pacl,
                                          struct timeval *pctrlc_t,
                                          double *pelapsed_t);

extern void nem_meshing_2D_face_mesh_gmsh_boundary (struct TESS Tess, struct NODES Nodes,
                                        struct MESH *Mesh, int face, int **pbnodes,
                                        double ***pbnodecoos, int *pbnodeqty);
