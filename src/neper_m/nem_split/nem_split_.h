/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_m.h"

#include"../nem_order/nem_order.h"

#include"nem_split.h"

extern void nem_split_edgenodes (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
                                 struct MESH *Mesh);
extern void nem_split_facenodes (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh,
                                 int **pfacenode, int ***peltfacenodes);
extern void nem_split_1d (struct MESH *pMesh);
extern void nem_split_2d (int *facenode, struct MESH *pMesh);
extern void nem_split_3d (int **eltfacenodes, struct NODES *pNodes, struct MESH *pMesh);

extern void nem_split_facenodes_3d (struct NODES *pNodes, struct MESH *Mesh, int **pfacenode,
                                    int ***peltfacenodes);
extern void nem_split_facenodes_curved (struct TESS Tess, int *facenode, struct NODES *pNodes,
                                        struct MESH *Mesh);
extern void nem_split_facenodes_periodic (struct TESS Tess, int *facenode,
                                          struct NODES *pNodes, struct MESH *Mesh);
extern void nem_split_mesh (int childqty, int **childnodes, struct MESH *pMesh);
