/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_poly_.h"

void
net_mtess_flatten_poly (struct MTESS MTess, struct TESS *Tess, int TessId,
                        int *CTessIds, int CTessQty, struct TESS *pFTess,
                        struct TESSE **pTessE, struct FLATTEN *pFlatten)
{
  int i, id;

  (*pFTess).PolyQty = 0;

  (*pTessE)[TessId].PolyQty = Tess[TessId].PolyQty;
  (*pTessE)[TessId].PolyFPolyQty = ut_alloc_1d_int (Tess[TessId].PolyQty + 1);
  (*pTessE)[TessId].PolyFPolyNb = ut_alloc_1d_pint (Tess[TessId].PolyQty + 1);
  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    (*pTessE)[id].PolyFPolyQty = ut_alloc_1d_int (Tess[id].PolyQty + 1);
    (*pTessE)[id].PolyFPolyNb = ut_alloc_1d_pint (Tess[id].PolyQty + 1);
  }

  net_mtess_flatten_cell_body (MTess, Tess, CTessIds, CTessQty, pFTess,
                               pTessE, pFlatten);

  (*pFTess).PolyState = ut_alloc_1d_int ((*pFTess).PolyQty + 1);

  return;
}
