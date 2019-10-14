/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FPRINTF_FEPX

  extern void neut_mesh_fprintf_fepx_name (char *body,
					   struct TESS Tess,
					   struct NODES Nodes,
					   struct MESH Mesh2D,
					   struct MESH Mesh3D,
					   struct NSET NSet0D,
					   struct NSET NSet1D,
					   struct NSET NSet2D, char *nset,
					   char *faset, char *version);

extern void neut_mesh_fprintf_fepx_parms (FILE *, struct NODES, struct MESH);
extern void neut_mesh_fprintf_fepx_nodes (FILE *, struct NODES);
extern void neut_mesh_fprintf_fepx_elts (FILE *, struct TESS, struct NSET,
					 struct MESH);
extern void neut_mesh_fprintf_fepx_skinelts (FILE *, struct TESS, struct MESH,
					     struct MESH, struct NODES,
					     char *);
extern void neut_mesh_fprintf_fepx_elsets (FILE *, struct MESH, char*);
extern void neut_mesh_fprintf_fepx_nsets (FILE *, struct NSET, struct NSET,
					  struct NSET, char *, char *);
extern void neut_mesh_fprintf_fepx_kocks (FILE *file, struct TESS Tess, char *version);
extern void neut_mesh_fprintf_fepx_nset (FILE *, char *, int, int *);

#endif				// NEUT_MESH_FPRINTF_FEPX

#ifdef __cplusplus
}
#endif
