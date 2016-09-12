/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_cell_.h"

void
net_mtess_flatten_cell (struct TESS *Tess,
			int *CTessIds, int CTessQty,
			struct TESS *pFTess)
{
  int i, j, id, id2;

  (*pFTess).CellQty = (*pFTess).PolyQty;

  id2 = 0;
  if (Tess[CTessIds[0]].CellLamId)
  {
    (*pFTess).CellLamId = ut_alloc_1d_int ((*pFTess).CellQty + 1);

    for (i = 0; i < CTessQty; i++)
    {
      id = CTessIds[i];

      for (j = 1; j <= Tess[id].CellQty; j++)
	(*pFTess).CellLamId[++id2] = Tess[id].CellLamId[j];
    }
  }

  return;
}
