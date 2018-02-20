/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut_v.h"
#include"neper_m/nem_reconstruct/nem_reconstruct_mesh/nem_reconstruct_mesh.h"

#include "nev_load.h"

extern void nev_load_init_tessdata (struct TESS Tess,
				    struct TESSDATA *pTessData);
extern void nev_load_init_tesrdata (struct TESR Tesr,
				    struct TESRDATA *pTesrData);

extern void nev_load_init_nodedata (struct NODES Nodes,
				    struct NODEDATA *pNodeData);
extern void nev_load_init_meshdata (struct MESH Mesh,
				    struct MESHDATA *pMeshData);

extern void nev_load_init_pointdata (struct POINT Point,
				     struct POINTDATA *pPointData);
