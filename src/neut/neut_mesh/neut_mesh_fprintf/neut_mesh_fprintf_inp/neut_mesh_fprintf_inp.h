/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FPRINTF_INP

  extern void neut_mesh_fprintf_inp (FILE * file, char *dim, struct TESS Tess,
				     struct NODES Nodes, struct MESH Mesh0D,
                                     struct MESH Mesh1D, struct MESH Mesh2D,
                                     struct MESH Mesh3D, struct MESH MeshCo,
                                     struct NSET NSet0D, struct NSET NSet1D,
                                     struct NSET NSet2D, char *nset, char
                                     *faset, struct PART Part, struct BOUNDARY
                                     Bound);

#endif				// NEUT_MESH_FPRINTF_INP

#ifdef __cplusplus
}
#endif
