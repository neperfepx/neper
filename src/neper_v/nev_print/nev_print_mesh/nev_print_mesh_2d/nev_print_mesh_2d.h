/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_mesh_2d (FILE * file, struct PRINT Print,
			       struct NODES Nodes, struct MESH *Mesh,
			       struct NODEDATA NodeData,
			       struct MESHDATA *MeshData);

extern void nev_print_mesh_2d_print (FILE * file, struct PRINT Print,
				     struct NODES N, struct MESH M1D,
				     struct MESH M2D,
				     struct NODEDATA NodeData,
				     struct MESHDATA *MeshData);

extern void nev_print_mesh_2d_compress (struct MESH Mesh2D, int *,
					struct MESH *pM1D);
