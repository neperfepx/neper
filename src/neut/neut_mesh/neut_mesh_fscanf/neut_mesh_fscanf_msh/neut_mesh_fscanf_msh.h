/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FSCANF_GMSH

  extern int neut_mesh_fscanf_msh (FILE * file, struct NODES *pNodes,
				    struct MESH *pMesh0D,
				    struct MESH *pMesh1D,
				    struct MESH *pMesh2D,
				    struct MESH *pMesh3D,
				    struct MESH *pMeshCo,
				    struct MESH *pMeshFasets, struct NSET *pNSet,
				    int *ptopology);

  extern void neut_mesh_fnscanf_msh (char *filename, struct NODES *pNodes,
					 struct MESH *pMesh0D,
					 struct MESH *pMesh1D,
					 struct MESH *pMesh2D,
					 struct MESH *pMesh3D,
					 struct MESH *pMeshCo,
					 struct MESH *pMeshFasets, struct NSET *pNSet,
					 int *ptopology, char *mode);

#endif				// NEUT_MESH_FSCANF_GMSH

#ifdef __cplusplus
}
#endif
