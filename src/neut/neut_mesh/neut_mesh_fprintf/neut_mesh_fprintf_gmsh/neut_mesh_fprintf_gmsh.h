/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FPRINTF_GMSH

  extern void neut_mesh_fprintf_gmsh (FILE * file, char *dim, struct NODES
				      Nodes, struct MESH Mesh0D, struct
				      MESH Mesh1D, struct MESH Mesh2D,
				      struct MESH Mesh3D, struct PART
				      Part, struct MESH CMesh, char
				      *numbering, char* mode);

#endif				// NEUT_MESH_FPRINTF_GMSH

#ifdef __cplusplus
}
#endif
