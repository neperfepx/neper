/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_crystal_.h"

void
neut_tess_crystal (struct TESS Tess, struct POINT *pCrystal)
{
  int i;

  neut_point_set_zero (pCrystal);

  (*pCrystal).Dim = Tess.Dim;

  (*pCrystal).Qty = Tess.CellQty;
  (*pCrystal).Coo = ut_alloc_2d ((*pCrystal).Qty + 1, 3);
  (*pCrystal).Rad = ut_alloc_1d ((*pCrystal).Qty + 1);

  for (i = 1; i <= Tess.CellQty; i++)
  {
    neut_tess_cell_centroid (Tess, i, (*pCrystal).Coo[i]);
    neut_tess_cell_radeq (Tess, i, (*pCrystal).Rad + i);
  }

  return;
}
