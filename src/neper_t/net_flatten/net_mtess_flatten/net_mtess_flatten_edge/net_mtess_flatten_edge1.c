/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_edge_.h"

void
net_mtess_flatten_edge (struct MTESS MTess, struct TESS *Tess,
			int TessId, int *CTessIds, int CTessQty,
			struct TESS *pFTess, struct TESSE
			*TessE, struct FLATTEN *pFlatten)
{
  int i, id;

  (*pFTess).EdgeQty = 0;
  (*pFTess).EdgeVerNb = ut_alloc_1d_pint (1);

  TessE[TessId].EdgeQty = Tess[TessId].EdgeQty;
  TessE[TessId].EdgeFEdgeQty = ut_alloc_1d_int  (Tess[TessId].EdgeQty + 1);
  TessE[TessId].EdgeFEdgeNb  = ut_alloc_1d_pint (Tess[TessId].EdgeQty + 1);
  TessE[TessId].EdgeFEdgeOri = ut_alloc_1d_pint (Tess[TessId].EdgeQty + 1);

  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    TessE[id].EdgeFEdgeQty = ut_alloc_1d_int  (Tess[id].EdgeQty + 1);
    TessE[id].EdgeFEdgeNb  = ut_alloc_1d_pint (Tess[id].EdgeQty + 1);
    TessE[id].EdgeFEdgeOri = ut_alloc_1d_pint (Tess[id].EdgeQty + 1);
  }

  // Body of the tessellation
  net_mtess_flatten_edge_body (MTess, Tess, CTessIds, CTessQty, pFTess,
			       TessE, pFlatten);

  // Dom face edges: looking at the faces of the parent tessellation
  net_mtess_flatten_edge_domface (MTess, Tess, TessId, pFTess, TessE, pFlatten);

  // Dom edge edges: looking at the edges of the parent tessellation
  net_mtess_flatten_edge_domedge (MTess, Tess, TessId, pFTess, TessE, pFlatten);

  neut_tess_init_veredge (pFTess);
  neut_tess_init_edgelength (pFTess);

  net_mtess_flatten_edge_init_fedgetessedge (Tess, CTessIds,
					     CTessQty, pFTess, TessE, pFlatten);

  return;
}
