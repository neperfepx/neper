/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_v.h"
#include "nev_print_mesh_2d.h"
#include "../../nev_print.h"

extern void nev_print_mesh_2d_print_edges (FILE * file, int,
					   struct NODES N,
					   struct MESH M1D,
					   struct MESHDATA *MeshData);

extern void nev_print_mesh_2d_print_faces (FILE * file, struct PRINT
					   Print, struct NODES N,
					   struct MESH M2D,
					   struct NODEDATA NodeData,
					   struct MESHDATA *MeshData);
