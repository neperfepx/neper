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

  if (Tess.Periodic)
  {
    (*pCrystal).Periodic = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy (Tess.Periodic, Tess.Dim, (*pCrystal).Periodic);
  }

  (*pCrystal).PointQty = Tess.CellQty;
  (*pCrystal).PointCoo = ut_alloc_2d ((*pCrystal).PointQty + 1, 3);
  (*pCrystal).PointRad = ut_alloc_1d ((*pCrystal).PointQty + 1);

  for (i = 1; i <= Tess.CellQty; i++)
  {
    neut_tess_cell_centroid (Tess, i, (*pCrystal).PointCoo[i]);
    neut_tess_cell_radeq (Tess, i, (*pCrystal).PointRad + i);
  }

  return;
}
