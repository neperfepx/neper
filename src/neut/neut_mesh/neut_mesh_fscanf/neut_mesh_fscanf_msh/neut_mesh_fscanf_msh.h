/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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
				    struct MESH *pMeshCo, int *ptopology);

  extern void neut_mesh_fnscanf_msh (char *filename, struct NODES *pNodes,
					 struct MESH *pMesh0D,
					 struct MESH *pMesh1D,
					 struct MESH *pMesh2D,
					 struct MESH *pMesh3D,
					 struct MESH *pMeshCo, int *ptopology,
                                         char *mode);

#endif				// NEUT_MESH_FSCANF_GMSH

#ifdef __cplusplus
}
#endif
