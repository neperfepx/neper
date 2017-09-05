/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_slice (char **, int *, struct PRINT *);
extern void nev_slice_mesh (struct NODES, struct MESH,
			    struct NODEDATA, struct MESHDATA *,
			    char *, int *, struct NODES **, struct MESH **,
			    struct NODEDATA **,
			    struct MESHDATA ***, int ***pEltNewOld);
