/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>

#include"neut_m.h"
#include"ut.h"

#include"structIn_m.h"
#include"nem_meshing_pinching.h"

extern void nem_meshing_pinching_inter (struct MESHPARA MeshPara, struct
    TESS *pTess, struct NODES *pNodes, struct MESH *Mesh, int elset);

extern void nem_meshing_pinching_fix (struct MESHPARA MeshPara, struct TESS
    *pTess, struct NODES RNodes, struct MESH *RMesh, struct NODES *pNodes, struct
    MESH *Mesh, int* elts);

extern int nem_meshing_pinching_testpoly (struct IN_M In, struct TESS Tess, struct MESH *Mesh,
                                          int poly);

extern void nem_meshing_pinching_fix_eltfacets (struct MESH *Mesh, int elt,
                                                int **peltfacets, int *peltfacetqty);

extern void nem_meshing_pinching_fix_split (struct NODES *pNodes, struct MESH *Mesh,
					    int elt, int eltfacet, int *newnode);

extern void nem_meshing_pinching_fix_proj (struct MESHPARA MeshPara, struct
    TESS *pTess, struct NODES RNodes, struct MESH* RMesh, struct NODES *pNodes,
    int elset, int newnode);
