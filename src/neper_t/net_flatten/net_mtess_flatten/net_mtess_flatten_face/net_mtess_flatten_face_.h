/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_t.h"
#include"net_mtess_flatten_face.h"

extern void net_mtess_flatten_face_body (struct MTESS MTess, struct TESS *Tess,
				   int *CTessIds, int CTessQty,
				   struct TESS *pFTess, struct TESSE
				   **pTessE, struct FLATTEN *pFlatten);
extern void net_mtess_flatten_face_domface (struct MTESS MTess, struct TESS *Tess,
                                      int TessId,
				      struct TESS *pFTess,
				      struct TESSE **pTessE,
				      struct FLATTEN *pFlatten);

extern void net_mtess_flatten_face_domface_dom (struct MTESS MTess,
					  struct TESS *Tess, int TessId,
					  int dface,
					  struct TESS *pFTess,
					  struct TESSE **pTessE, int *dom,
					  struct FLATTEN *pFlatten);

extern void net_mtess_flatten_face_domface_body (struct MTESS MTess,
					   struct TESS *Tess,
					   int TessId, int dface,
					   struct TESS *pFTess,
					   struct TESSE **pTessE, int *dom,
					   struct FLATTEN *pFlatten);

extern int net_mtess_flatten_face_domface_edge_tess_faces (struct FLATTEN
						     Flatten, int edge,
						     double *coo,
						     struct TESS Tess,
						     int domface,
						     int **pfaces,
						     int *pfaceqty);
extern int net_mtess_flatten_face_domface_ver_tess_edges (struct FLATTEN Flatten,
						    int fver,
						    struct TESS Tess,
						    int domface, int **pedges,
						    int *pedgeqty);
extern void net_mtess_flatten_face_domface_body_tessfaces (struct MTESS MTess,
						     struct TESS *Tess,
						     int TessId, int dface,
						     int **tessface);
extern void net_mtess_flatten_face_domface_body_nextedge (struct FLATTEN Flatten,
						    struct TESS Tess,
						    int dface,
						    struct TESS FTess,
						    int *tessfaces, int fedge,
						    int fver,
						    int *edgesinv,
						    int ***edgetessfacenb,
						    int
						    **edgetessfaceqty,
						    int *pnedge,
						    int *pnedgeori);

extern int net_mtess_flatten_face_domface_body_nextface (int *edges, int edgeqty,
						   int *edgeinv,
						   int ***edgetessfacenb,
						   int **edgetessfaceqty,
						   int ***pfacepairlist,
						   int *pfacepairqty,
						   int *pedgepos,
						   int *facepair);
