/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

#include"neut_mesh_fprintf_vtk.h"

extern void neut_meshheader_fprintf_vtk (FILE * file);
extern void neut_elts_fprintf_vtk (FILE * file, char *dim, struct NODES Nodes,
				   struct MESH Mesh1D,
				   struct MESH Mesh2D, struct MESH Mesh3D,
				   struct PART Part);
extern void neut_nodes_fprintf_vtk (FILE * file, struct NODES Nodes);
