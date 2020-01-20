/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

#include"neut_mesh_fprintf_gmsh.h"

extern void neut_mesh_fprintf_gmsh_pre (struct TESS Tess,
    char *fasetlist, char ***pfasets, int **pfasetids, int *pfasetqty);

extern void neut_meshheader_fprintf_gmsh (FILE * file, char *mode, struct MESH
					  Mesh0D, struct MESH Mesh1D,
					  struct MESH Mesh2D,
					  struct MESH Mesh3D);
extern void neut_elts_fprintf_gmsh (FILE * file, char *mode, struct TESS Tess,
                                    struct MESH Mesh0D, struct MESH Mesh1D,
                                    struct MESH Mesh2D, struct MESH Mesh3D,
				    struct PART Part, struct MESH CMesh,
                                    char **fasets, int *fasetids, int fasetqty,
                                    char *dim, char *numbering);
extern void neut_nodes_fprintf_gmsh (FILE * file, char *mode, struct NODES Nodes);

extern void neut_physical_fprintf_gmsh (FILE * file, struct MESH Mesh0D, struct
    MESH Mesh1D, struct MESH Mesh2D, struct MESH Mesh3D, struct MESH MeshCo,
    char **fasets, int *fasetids, int fasetqty, char *dim);
