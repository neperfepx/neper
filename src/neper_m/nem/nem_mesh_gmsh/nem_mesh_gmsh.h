/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_mesh_gmsh_options (FILE *, int, int, int, double);

extern int nem_mesh_2d_gmsh_writeboundary (struct TESS, struct NODES,
                                           struct MESH *, int, double *,
                                           int **pbnodes, double ***pbnodecoos,
                                           int *pbnodeqty, FILE *);
extern int nem_mesh_2d_gmsh (struct TESS, int, double *, struct NODES,
			     struct MESH *, double, char *, char *, char *, double,
			     double, struct NODES *, struct MESH *,
                             int **pbnodes, int **pbnoderbnodes, int *pbnodeqty,
			     double *, struct timeval *, double *);
extern int nem_mesh_2d_gmsh_b (struct TESS, int, struct NODES, struct MESH *,
			       char *, char *, double, double, struct NODES *,
			       struct MESH *, double *);
extern int nem_mesh_3d_gmsh (struct TESS, int, struct NODES, struct MESH *,
			     double, double, char *, char *, char *, char *, double,
			     double, struct NODES *, struct MESH *, double *,
			     struct timeval *, double *);
extern void nem_mesh_2d_gmsh_writeface_b (double, FILE *);
extern void nem_mesh_3d_gmsh_writepoly (double, char *, FILE *);
