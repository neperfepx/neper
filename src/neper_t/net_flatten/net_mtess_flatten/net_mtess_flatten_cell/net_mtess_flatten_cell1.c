/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_cell_.h"

void
net_mtess_flatten_cell (struct TESS *Tess, int *CTessIds, int CTessQty,
                        struct TESS *pFTess)
{
  int i, j, id, id2;

  (*pFTess).CellQty = (*pFTess).PolyQty;

  // CellLamId
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

  // CellModeId
  id2 = 0;
  if (Tess[CTessIds[0]].CellModeId)
  {
    (*pFTess).CellModeId = ut_alloc_1d_int ((*pFTess).CellQty + 1);

    for (i = 0; i < CTessQty; i++)
    {
      id = CTessIds[i];

      for (j = 1; j <= Tess[id].CellQty; j++)
        (*pFTess).CellModeId[++id2] = (Tess[id].CellModeId) ? Tess[id].CellModeId[j] : -1;
    }
  }

  // CellGroup
  id2 = 0;
  if (Tess[CTessIds[0]].CellGroup)
  {
    (*pFTess).CellGroup = ut_alloc_1d_int ((*pFTess).CellQty + 1);

    for (i = 0; i < CTessQty; i++)
    {
      id = CTessIds[i];

      for (j = 1; j <= Tess[id].CellQty; j++)
        (*pFTess).CellGroup[++id2] = Tess[id].CellGroup[j];
    }
  }

  // CellOri
  id2 = 0;
  if (Tess[CTessIds[0]].CellOri)
  {
    (*pFTess).CellOri = ut_alloc_2d ((*pFTess).CellQty + 1, 4);

    for (i = 0; i < CTessQty; i++)
    {
      id = CTessIds[i];

      for (j = 1; j <= Tess[id].CellQty; j++)
        ut_array_1d_memcpy (Tess[id].CellOri[j], 4, (*pFTess).CellOri[++id2]);
    }
  }

  // CellOriDistrib
  id2 = 0;
  if (Tess[CTessIds[0]].CellOriDistrib)
  {
    (*pFTess).CellOriDistrib = ut_alloc_1d_pchar ((*pFTess).CellQty + 1);

    for (i = 0; i < CTessQty; i++)
    {
      id = CTessIds[i];

      for (j = 1; j <= Tess[id].CellQty; j++)
        ut_string_string (Tess[id].CellOriDistrib[j], &(*pFTess).CellOriDistrib[++id2]);
    }
  }

  return;
}
