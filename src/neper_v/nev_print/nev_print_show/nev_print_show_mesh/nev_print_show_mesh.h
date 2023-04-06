/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_show_mesh_elt (char *entity, char *expr, struct TESS *pTess, struct
			       NODES Nodes, struct MESH *Mesh,
			       struct PRINT *pPrint);
extern void nev_print_show_mesh_elset (char *entity, char *expr, struct TESS *pTess,
				 struct NODES Nodes, struct MESH *Mesh,
				 struct PRINT *pPrint);
extern void nev_print_show_mesh_nodes (char *expr, struct TESS *pTess,
				 struct NODES Nodes, struct MESH *Mesh,
				 struct PRINT *pPrint);
