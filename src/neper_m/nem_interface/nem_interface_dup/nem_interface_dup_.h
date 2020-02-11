/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<float.h>

#include"structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include"nem_interface_dup.h"

extern void nem_interface_dup_pre (struct IN_M In, struct TESS Tess,
                                   int **pvers, int *pverqty,
                                   int **pedges, int *pedgeqty,
                                   int **pfaces, int *pfaceqty);
extern void nem_interface_dup_ver (struct TESS Tess, int *vers, int verqty,
                                       struct NODES *pNodes, struct MESH *Mesh);
extern void nem_interface_dup_edge (struct TESS Tess, int *edges, int edgeqty,
                                        struct NODES *pNodes, struct MESH *Mesh);
extern void nem_interface_dup_face (struct TESS Tess, int *faces, int faceqty,
                                        struct NODES *pNodes, struct MESH *Mesh);

extern void nem_interface_duplicate (int *polys, int polyqty, int *nodes,
                                     int nodeqty, struct NODES *pNodes,
                                     struct MESH *Mesh);

extern void nem_interface_dup_boundelts_3d (struct TESS Tess, int *faces, int faceqty,
                                            struct NODES Nodes,
				        struct MESH *Mesh, struct BOUNDARY *pBound);
extern void nem_interface_dup_boundelts_2d (struct TESS Tess, struct NODES Nodes,
				        struct MESH *Mesh, struct BOUNDARY *pBound);

extern void nem_interface_dup_renumber_1d (struct TESS Tess, struct NODES Nodes,
				       struct MESH *Mesh);
extern void nem_interface_dup_renumber_2d (struct TESS Tess, struct NODES Nodes,
				       struct MESH *Mesh);

extern void nem_interface_dup_per (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh);

