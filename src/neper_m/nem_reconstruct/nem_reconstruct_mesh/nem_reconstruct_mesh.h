/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nem_reconstruct_mesh (char *, struct NODES *, struct MESH *,
				  struct TESS *);
extern void nem_reconstruct_mesh_fasets (struct MESH Mesh2D,
					 struct MESH MeshFasets,
					 struct TESS *pTess);
extern int nem_reconstruct_mesh_nsets (struct NODES Nodes, struct MESH Mesh2D,
				       struct NSET NSet2D, struct TESS *pTess);
