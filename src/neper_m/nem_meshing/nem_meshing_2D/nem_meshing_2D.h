/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_2D (struct IN_M, struct MESHPARA, struct TESS,
			    struct NODES, struct MESH *, struct NODES *,
			    struct MESH *);

extern void nem_meshing_2D_old (struct IN_M In, struct MESHPARA MeshPara,
				struct TESS Tess, struct NODES RNodes,
				struct MESH *RMesh, struct NODES *pNodes,
				struct MESH *Mesh);
