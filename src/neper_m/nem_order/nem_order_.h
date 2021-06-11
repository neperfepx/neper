/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>

#include"structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include"nem_order.h"

extern void nem_order_pre (struct IN_M In, struct MESH *Mesh, int *domesh);
extern void nem_order_dim (struct IN_M In, struct NODES *pNodes, struct MESH *pMeshU, struct MESH *pMesh);
extern void nem_order_periodic (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh);
extern void nem_order_periodic_edges (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh);
extern void nem_order_periodic_faces (struct TESS Tess, struct NODES *pNodes, struct MESH *Mesh);
extern void nem_order_post (struct IN_M In, int NodeQty_before, struct NODES *pNodes);
extern void nem_order_newnode (struct NODES *pNodes, struct MESH *pMesh, int *nodes);
extern void nem_order_dim_record (struct MESH *pMesh, int *nodes, int node_o2);
