/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut.h"

#include "neut_mesh_fprintf_inp.h"

extern void neut_mesh_fprintf_inp_mesh2d (FILE *file,
					  struct MESH Mesh2D, int, char* type);
extern void neut_mesh_fprintf_inp_mesh3d (FILE *file,
					  struct MESH Mesh3D, int, char *type);
extern void neut_mesh_fprintf_inp_elsets (FILE *file, struct MESH
					  Mesh, char *name, int shift);

extern void neut_mesh_fprintf_inp_nsets (FILE *, struct NSET, struct NSET,
					 struct NSET, char*, char *);
extern void neut_mesh_fprintf_inp_fasets (FILE *, struct TESS Tess, struct MESH
					  Mesh2D, struct MESH Mesh3D,
					  struct NODES Nodes, char
					  *surf);

extern void neut_mesh_fprintf_inp_nset (FILE *, char *, int, int *, char*);

extern void neut_mesh_fprintf_inp_part_nset (FILE * file, struct PART Part);
extern void neut_mesh_fprintf_inp_part_elset (FILE * file, struct PART Part);

extern void neut_mesh_fprintf_inp_boundary (FILE* file, struct BOUNDARY Bound);
