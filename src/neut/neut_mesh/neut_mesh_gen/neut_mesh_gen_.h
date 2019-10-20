/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"neut.h"
#include"float.h"
#include"orilib.h"

extern int neut_mesh3d_elt_slice_tri (struct NODES Nodes, struct MESH
				      Mesh, int elt, double *eq,
				      struct NODES *pSNodes,
				      struct MESH *pSMesh, int **pelt_newold,
				      int ***pnode_newold,
				      double **pnode_fact);

extern int neut_mesh3d_elt_slice_quad (struct NODES Nodes, struct MESH
				       Mesh, int elt, double *eq,
				       struct NODES *pSNodes,
				       struct MESH *pSMesh, int **pelt_newold,
				       int ***pnode_newold,
				       double **pnode_fact);
