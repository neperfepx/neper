/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_cleaning_singnodedup (struct MESH *Mesh,
				      struct NODES *pNodes, int ***pFoDNodes);

extern void nem_cleaning_dupnodemerge (struct NODES *pNodes, struct
				       MESH *Mesh, double eps);
