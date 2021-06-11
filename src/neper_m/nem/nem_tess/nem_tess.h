/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_tess_face_1delts (struct TESS, struct MESH *, int, int **,
				  int *);

extern void nem_tess_edgebodies_nodes (struct TESS Tess, struct MESH *,
				       int **pnodes, int *pnodeqty);

extern void nem_tess_facebodies_nodes (struct TESS Tess, struct MESH *,
				       int **pnodes, int *pnodeqty);

extern void nem_tess_updatefrommesh_geom (struct TESS *pTess, struct
					  NODES Nodes, struct MESH *Mesh);

extern void nem_tess_updatefrommesh_geom_polyfaceori (struct TESS *pTess,
						      struct NODES Nodes,
						      struct MESH *Mesh);

extern void nem_tess_updatefrommesh_geom_seed (struct TESS *pTess, struct
					       NODES Nodes, struct MESH
					       *Mesh);

extern void nem_tess_updatefrommesh_geom_ver (struct TESS *pTess, struct
					      NODES Nodes, struct MESH *Mesh);
extern void nem_tess_updatefrommesh_geom_edge (struct TESS *pTess, struct
					       NODES Nodes, struct MESH
					       *Mesh);
extern void nem_tess_updatefrommesh_geom_face (struct TESS *pTess, struct
					       NODES Nodes, struct MESH
					       *Mesh);
