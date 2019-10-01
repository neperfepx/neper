/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"../structIn_m.h"

extern void nem_smoothing (struct IN_M In, struct TESS *pTess,
			   struct NODES *pNodes, struct MESH *Mesh);
extern void nem_smoothing_laplacian (struct TESS Tess,
                                     struct NODES *pNodes,
				     struct MESH *Mesh, int dim,
				     double A, int itermax, char *nodetype);
