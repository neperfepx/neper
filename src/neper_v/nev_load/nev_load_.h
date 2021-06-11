/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut_v.h"
#include"neper_m/nem_reconstruct/nem_reconstruct_mesh/nem_reconstruct_mesh.h"

#include "nev_load.h"

extern void nev_load_init_data_tess (struct TESS Tess, int dim,
				    struct DATA *pTessData);
extern void nev_load_init_data_tesr (struct TESR Tesr,
				    struct DATA *pTesrData);

extern void nev_load_init_data_node (struct NODES Nodes,
				    struct DATA *pData);
extern void nev_load_init_data_mesh (struct MESH Mesh,
				    struct DATA *pMeshData);

extern void nev_load_init_data_point (struct POINT Point,
				     struct DATA *pPointData);
