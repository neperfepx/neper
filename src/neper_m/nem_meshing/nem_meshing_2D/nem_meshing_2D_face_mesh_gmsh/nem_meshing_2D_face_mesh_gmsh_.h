/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include"neper_config.h"

#ifdef HAVE_OPENMP
#include<omp.h>
#endif // HAVE_OPENMP

#include"ut.h"
#include"neut_m.h"
#include"../../../nem/nem.h"

#include"nem_meshing_2D_face_mesh_gmsh.h"

extern void nem_meshing_2D_face_mesh_gmsh_proj (struct TESS Tess, struct NODES Nodes,
                                                int face, struct MESHPARA MeshPara,
                                                int *bnodes,
                                                double ***pbnodecoos, double **pbnodecls,
                                                int bnodeqty);

extern void nem_meshing_2D_face_mesh_gmsh_writeboundary (struct NODES Nodes, int *bnodes,
                                                         double **bnodecoos, double *bnodecls,
                                                         int bnodeqty, FILE * file);

extern void nem_meshing_2D_face_mesh_gmsh_backproj (struct TESS Tess,
                                          struct NODES RNodes,
                                          struct MESH *RMesh, int face,
                                          struct MESHPARA MeshPara,
                                          struct NODES Nodes,
                                          int *bnodes, int *lbnodes,
                                          int bnodeqty, struct NODES *pN,
                                          struct MESH M);

extern void nem_meshing_2D_face_mesh_gmsh_backproj_fixboundary (struct NODES Nodes,
                                                 int *bnodes, int *lbnodes,
                                                 int bnodeqty, struct NODES *pN);
