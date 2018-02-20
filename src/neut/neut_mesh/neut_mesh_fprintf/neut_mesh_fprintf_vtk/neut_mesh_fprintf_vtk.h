/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FPRINTF_VTK

  extern void neut_mesh_fprintf_vtk (FILE * file, char *dim,
				     struct NODES Nodes, struct MESH Mesh1D,
				     struct MESH Mesh2D, struct MESH Mesh3D,
				     struct PART Part);

#endif				// NEUT_MESH_FPRINTF_VTK

#ifdef __cplusplus
}
#endif
