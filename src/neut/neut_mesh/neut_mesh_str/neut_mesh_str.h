/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_STR_H
#define NEUT_MESH_STR_H

/// \brief Create a 3D mesh into quad (hex) elts.
///
///
///
  extern void neut_mesh_str (int dim, int *size, struct NODES *pNodes,
			     struct MESH *pMesh, struct NSET *pNSet);

  extern void neut_mesh_str_3d (int *size, struct NODES *pNodes, struct
				MESH *pMesh, struct NSET *pNSet);
  extern void neut_mesh_str_2d (int *size, struct NODES *pNodes, struct
				MESH *pMesh, struct NSET *pNSet);
  extern void neut_mesh_str_1d (int *size, struct NODES *pNodes, struct
				MESH *pMesh, struct NSET *pNSet);

#endif				/* NEUT_MESH_STR_H */

#ifdef __cplusplus
}
#endif
