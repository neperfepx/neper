/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_tocta_.h"

int
net_tess_tocta_cellid (struct SEEDSET SSet, struct TESS *pTess)
{
  int i;
  double *array = ut_alloc_1d ((*pTess).CellQty);
  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);

  gsl_rng_set (r, SSet.Random);

  (*pTess).CellId = ut_alloc_1d_int ((*pTess).CellQty + 1);

  for (i = 0; i < (*pTess).CellQty; i++)
    array[i] = gsl_rng_uniform (r);

  ut_array_1d_sort_index (array, (*pTess).CellQty, (*pTess).CellId + 1);
  ut_array_1d_int_addval ((*pTess).CellId + 1, (*pTess).CellQty, 1,
                          (*pTess).CellId + 1);

  ut_free_1d (&array);
  gsl_rng_free (r);

  return 0;
}
