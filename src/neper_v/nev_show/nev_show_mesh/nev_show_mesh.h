/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_show_mesh (char **argv, int *pi, struct PRINT *pPrint);
extern void nev_show_mesh_slice (char **argv, int *pi, struct PRINT *pPrint);
extern void nev_show_mesh_elt (char **argv, int *pi, struct TESS Tess, struct
			       NODES Nodes, struct MESH *Mesh,
			       struct PRINT *pPrint);
extern void nev_show_mesh_elset (char **argv, int *pi, struct TESS Tess,
				 struct NODES Nodes, struct MESH *Mesh,
				 struct PRINT *pPrint);
extern void nev_show_mesh_nodes (char **argv, int *pi, struct TESS Tess,
				 struct NODES Nodes, struct MESH *Mesh,
				 struct PRINT *pPrint);
