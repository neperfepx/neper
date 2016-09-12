/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
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

extern void nem_interface_dup_ver (struct TESS Tess, struct NODES
				    *pNodes, struct MESH *Mesh);
extern void nem_interface_dup_edge (struct TESS Tess, struct NODES
				    *pNodes, struct MESH *Mesh);
extern void nem_interface_dup_face (struct TESS Tess, struct NODES
				    *pNodes, struct MESH *Mesh);
extern void nem_interface_dup_ver_duplicate (struct TESS Tess, int i, struct NODES *pNodes,
			     struct MESH *Mesh);

extern void nem_interface_duplicate (int dim, int *polys, int polyqty,
    int *nodes, int nodeqty, struct NODES *pNodes, struct MESH *Mesh);

extern void nem_interface_dup_boundelts_3d (struct TESS Tess, struct NODES Nodes,
				        struct MESH *Mesh, struct BOUNDARY *pBound);
extern void nem_interface_dup_boundelts_2d (struct TESS Tess, struct NODES Nodes,
				        struct MESH *Mesh, struct BOUNDARY *pBound);

extern void nem_interface_dup_renumber_1d (struct TESS Tess, struct NODES Nodes,
				       struct MESH *Mesh);
extern void nem_interface_dup_renumber_2d (struct TESS Tess, struct NODES Nodes,
				       struct MESH *Mesh);
