/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<float.h>

#include"ut.h"
#include"neut_t.h"
#include"net_mtess_flatten_edge.h"

extern void net_mtess_flatten_edge_body (struct MTESS MTess, struct TESS *Tess,
                                   int *tessids, int tessqty,
				   struct TESS *pFTess,
				   struct TESSE *TessE,
				   struct FLATTEN *pFlatten);
extern void net_mtess_flatten_edge_domface (struct MTESS MTess, struct TESS *Tess,
				      int domtess, struct TESS *pFTess,
				      struct TESSE *TessE,
				      struct FLATTEN *pFlatten);
extern void net_mtess_flatten_edge_domedge (struct MTESS MTess, struct TESS *Tess,
				      int domtess, struct TESS *pFTess,
				      struct TESSE *TessE, struct
				      FLATTEN *pFlatten);
extern void net_mtess_flatten_edge_domface_edges (struct MTESS MTess,
					    struct TESS *Tess, int tessid,
					    int face, int *pVerQty,
					    double ***pVerCoo, int **pTessNb,
					    int *pTessQty, int ***pEdgeNb,
					    int **pEdgeQty,
					    int ****pEdgeVerNb,
					    int ***pEdgeVerQty);
extern void net_mtess_flatten_edge_domface_addedge (struct TESS *pFTess,
					      struct TESS Tess,
					      struct TESSE *pTessE, int edge,
					      int *dom,
					      struct FLATTEN *pFlatten);
extern void net_mtess_flatten_edge_domface_addedge_ivers (struct TESS *pFTess,
						    struct TESS Tess,
						    struct TESSE *pTessE,
						    int edge, int *EdgeVerNb,
						    int EdgeVerQty, int *dom,
						    struct FLATTEN *pFlatten);

extern void net_mtess_flatten_edge_domedge_edges (struct MTESS MTess,
					    struct TESS *Tess, int tessid,
					    int edge, int **pTessNb,
					    int *pTessQty, int ***pEdgeNb,
					    int ***pEdgeOri, int **pEdgeQty);
extern void net_mtess_flatten_edge_domedge_addedges (struct TESS *pFTess,
					       struct TESSE *TessE,
					       struct TESS *Tess, int domtess,
					       int domedge, int *TessNb,
					       int TessQty, int **EdgeNb,
					       int **EdgeOri, int
					       *EdgeQty,
					       struct FLATTEN *pFlatten);
extern void net_mtess_flatten_edge_domedge_linkedge (int fedge, struct TESSE *TessE,
					       int tess, int edge, int ori);

extern void net_mtess_flatten_edge_init_fedgetessedge (struct TESS *Tess,
						 int *tessids, int tessqty,
						 struct TESS *pFTess,
						 struct TESSE *TessE,
						 struct FLATTEN *pFlatten);
