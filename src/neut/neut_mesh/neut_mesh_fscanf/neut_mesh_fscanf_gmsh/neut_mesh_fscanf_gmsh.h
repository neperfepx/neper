/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FSCANF_GMSH

  extern void neut_mesh_fscanf_msh (FILE * file, struct NODES *pNodes,
				    struct MESH *pMesh0D,
				    struct MESH *pMesh1D,
				    struct MESH *pMesh2D,
				    struct MESH *pMesh3D);

  extern void neut_mesh_name_fscanf_msh (char *filename, struct NODES *pNodes,
					 struct MESH *pMesh0D,
					 struct MESH *pMesh1D,
					 struct MESH *pMesh2D,
					 struct MESH *pMesh3D);

#endif				// NEUT_MESH_FSCANF_GMSH

#ifdef __cplusplus
}
#endif
