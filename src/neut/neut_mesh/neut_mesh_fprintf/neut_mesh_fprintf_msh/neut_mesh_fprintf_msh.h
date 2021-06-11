/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FPRINTF_MSH

  extern void neut_mesh_fprintf_msh (FILE * file, char *dim,
                                     struct TESS Tess, struct NODES Nodes,
                                     struct MESH Mesh0D, struct MESH Mesh1D,
                                     struct MESH Mesh2D, struct MESH Mesh3D,
                                     struct MESH MeshCo,
                                     struct NSET NSet0D, struct NSET NSet1D, struct NSET NSet2D,
                                     char *nsetlist, char *fasetlist, char *numbering,
                                     char *version, char *mode);

  extern void neut_mesh_fprintf_msh_orientations (FILE *file, struct MESH Mesh);

#endif                          // NEUT_MESH_FPRINTF_MSH

#ifdef __cplusplus
}
#endif
