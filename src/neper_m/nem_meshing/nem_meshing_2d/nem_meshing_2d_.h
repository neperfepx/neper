/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<float.h>
#include<ctype.h>
#include<unistd.h>
#include<limits.h>

#ifdef HAVE_muparser
#include<muparser.h>
#endif

#include"ut.h"
#include"neut_m.h"
#include"../../structIn_m.h"
#include"../../nem/nem.h"
#include"nem_meshing_2d_face_mesh_gmsh/nem_meshing_2d_face_mesh_gmsh.h"

#include"nem_meshing_2d.h"

extern void nem_meshing_2d_progress (struct MULTIM Multim, int face, int
                                     faceqty, char *message);

extern int nem_meshing_2d_face (struct IN_M, struct MESHPARA, struct MULTIM *,
                                struct timeval *, double *,
                                double *, struct TESS, struct NODES,
                                struct MESH *, struct NODES, struct MESH *,
                                struct NODES *, struct MESH *,
                                int **pbnodes, int **plbnodes, int *pbnodeqty,
                                int);

extern int nem_meshing_2d_face_per (struct TESS Tess, struct NODES Nodes,
                                    struct NODES *pNodes, struct MESH *,
                                    struct NODES *pN, struct MESH *pM,
                                    int **pmaster_id, int ***pbnodes,
                                    int ***plbnodes, int **pbnodeqty,
                                    int face);

extern void nem_meshing_2d_face_mesh (struct IN_M In, struct MESHPARA,
                                      struct MULTIM *pMultim, int algo,
                                      struct timeval *, double *pallowed_t,
                                      double *pmax_elapsed_t,
                                      struct TESS Tess, struct NODES RNodes,
                                      struct MESH *RMesh, struct NODES Nodes,
                                      struct MESH *Mesh, int face,
                                      struct NODES *pN, struct MESH *pM,
                                      int **pbnodes, int **plbnodes,
                                      int *pbnodeqty, double *pmOsize,
                                      double *pelapsed_t);

extern void nem_meshing_2d_face_record (struct TESS Tess, int face,
                                        struct NODES N, struct MESH M,
                                        int *bnodes, int *lbnodes,
                                        int bnodeqty, int *master_id,
                                        struct NODES *pNodes,
                                        int **N_global_id,
                                        struct MESH *Mesh);

extern void nem_meshing_2d_face_record_nodes (struct TESS Tess, int face,
                                              struct NODES N, int *bnodes,
                                              int *lbnodes, int bnodeqty,
                                              int *master_id, int **N_global_id,
                                              struct NODES *pNodes);

extern void nem_meshing_2d_face_record_elts (int face, struct MESH M,
                                             int *N_global_id,
                                             struct MESH *Mesh);

extern void nem_meshing_2d_smoothfaces (struct TESS Tess,
                                        struct NODES *pNodes,
                                        struct MESH *Mesh);

extern void nem_meshing_2d_face_laplaciansmooth (struct TESS Tess, int face,
                                                 int *lbnodes, int bnodeqty,
                                                 struct NODES *pN,
                                                 struct MESH M);

extern void nem_meshing_2d_face_per_rodrigues (struct TESS Tess, struct NODES *N, int face, struct NODES *pN);
