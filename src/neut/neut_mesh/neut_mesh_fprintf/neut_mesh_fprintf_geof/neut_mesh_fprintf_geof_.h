/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut.h"

#include "neut_mesh_fprintf_geof.h"

extern void neut_mesh_fprintf_geof_head (FILE * file);
extern void neut_mesh_fprintf_geof_nodes (FILE *, struct NODES);
extern void neut_mesh_fprintf_geof_elts (FILE *, struct MESH, struct MESH,
					 struct MESH, char *, int *);
extern void neut_mesh_fprintf_geof_nsets (FILE *, struct NSET, struct NSET,
					  struct NSET, char *);
extern void neut_mesh_fprintf_geof_sethead (FILE *);
extern void neut_mesh_fprintf_geof_bsets (FILE * file, struct MESH Mesh1D,
					  char *bsets);
extern void neut_mesh_fprintf_geof_elsets (FILE *, struct MESH, struct MESH,
					   struct MESH, char *, int *);
extern void neut_mesh_fprintf_geof_foot (FILE *);
extern void neut_mesh_fprintf_geof_nset (FILE *, char *, int, int *);
extern void neut_mesh_fprintf_geof_part (FILE *, struct PART);
extern void neut_mesh_fprintf_geof_part_nset (FILE *, struct PART);
extern void neut_mesh_fprintf_geof_part_elset (FILE *, struct PART);

extern void neut_mesh_fprintf_geof_lisets (FILE* file, struct MESH
    Mesh2D, struct BOUNDARY Bound, char* lisets);
extern void neut_mesh_fprintf_geof_lisets_all (FILE* file, struct MESH
    Mesh1D);
extern void neut_mesh_fprintf_geof_fasets (FILE* file, struct MESH
    Mesh2D, struct BOUNDARY Bound, char* lisets);
