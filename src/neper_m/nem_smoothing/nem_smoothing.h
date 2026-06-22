/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

extern void nem_smoothing (struct IN_M In, struct TESS *pTess,
			   struct NODES *pNodes, struct MESH *Mesh);
extern void nem_smoothing_laplacian (struct TESS Tess,
                                     struct NODES *pNodes,
				     struct MESH *Mesh, int dim,
				     double A, int itermax, char *nodetype);
