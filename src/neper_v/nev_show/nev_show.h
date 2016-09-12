/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_show_init (struct TESS Tess, struct TESR Tesr,
			   struct NODES Nodes, struct MESH *Mesh,
			   int SQty, struct POINT Point,
			   struct PRINT *pPrint);

extern void nev_show (char **, int *, struct TESS, struct TESR,
		      struct NODES, struct MESH *, struct POINT,
		      struct PRINT *);
