/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
#include "nev_print_mesh_3d.h"
#include "../nev_print_mesh_2d/nev_print_mesh_2d.h"
#include "../../nev_print.h"

extern void nev_print_mesh_3d_compress (struct PRINT Print, struct NODES
					Nodes, struct MESH Mesh3D,
					struct NODES *, struct MESH *,
					int ***pprintelt3dface, int **,
					int **);

extern void
nev_print_mesh_3d_print (FILE * file, struct PRINT Print, struct NODES N,
			 struct MESH M1D, struct MESH M2D,
			 int *elt2delt3d, int *nodes_new_old,
			 struct NODEDATA NodeData, struct MESHDATA *MeshData);

extern void
nev_print_mesh_3d_print_faces (FILE * file, struct PRINT Print,
			       struct NODES N, struct MESH M2D,
			       int *elt2delt3d, int *nodes_new_old,
			       struct NODEDATA NodeData,
			       struct MESHDATA *MeshData);
extern void
nev_print_mesh_3d_print_edges (FILE * file, int showshadow,
			       struct NODES N, struct MESH M1D,
			       struct MESHDATA *MeshData);
