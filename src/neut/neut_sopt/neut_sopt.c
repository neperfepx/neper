/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_sopt_.h"

#ifdef HAVE_NLOPT
void
neut_sopt_set_zero (struct SOPT *pSOpt)
{
  (*pSOpt).CellQty = 0;
  (*pSOpt).CellSCellQtyMin = NULL;
  (*pSOpt).CellSCellQtyMax = NULL;
  (*pSOpt).CellSCellQty = NULL;
  (*pSOpt).CellSSeedCoo = NULL;
  (*pSOpt).CellSSeedWei = NULL;

  (*pSOpt).CellSCellOptiThr = NULL;
  neut_point_set_zero (&(*pSOpt).SCellOptiPts);
  (*pSOpt).SCellOptiCellId = 0;

  return;
}

void
neut_sopt_free (struct SOPT *pSOpt)
{
  int i;

  ut_free_1d_int (&(*pSOpt).CellSCellQtyMin);
  ut_free_1d_int (&(*pSOpt).CellSCellQtyMax);
  ut_free_1d_int (&(*pSOpt).CellSCellQty);
  neut_sopt_set_zero (pSOpt);

  for (i = 1; i <= (*pSOpt).CellQty; i++)
  {
    ut_free_2d (&(*pSOpt).CellSSeedCoo[i], (*pSOpt).CellSCellQtyMax[i]);
    ut_free_1d (&(*pSOpt).CellSSeedWei[i]);
  }
  ut_free_1d_ppdouble (&(*pSOpt).CellSSeedCoo);
  ut_free_1d_pdouble (&(*pSOpt).CellSSeedWei);
  ut_free_1d (&(*pSOpt).CellSCellOptiThr);
  ut_free_1d_int (&(*pSOpt).CellSCellQtyMin);
  ut_free_1d_int (&(*pSOpt).CellSCellQtyMax);

  neut_point_free (&(*pSOpt).SCellOptiPts);

  return;
}
#endif
