/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_merge_.h"

void
neut_tess_merge (struct TESS *pTess)
{
  int i, j, id;
  int cellidqty = 0, *cellids = NULL, *cellqty = NULL, **cells = NULL;

  cellids = ut_alloc_1d_int ((*pTess).CellQty);
  ut_array_1d_int_memcpy (cellids, (*pTess).CellQty, (*pTess).CellId + 1);
  ut_array_1d_int_sort_uniq (cellids, (*pTess).CellQty, &cellidqty);

  cells = ut_alloc_1d_pint (cellidqty);
  cellqty = ut_alloc_1d_int (cellidqty);
  for (i = 0; i < cellidqty; i++)
  {
    id = cellids[i];
    for (j = 1; j <= (*pTess).CellQty; j++)
      if ((*pTess).CellId[j] == id)
	ut_array_1d_int_list_addelt (cells + i, cellqty + i, j);
  }

  if ((*pTess).Dim == 3)
    for (i = 0; i < cellidqty; i++)
      neut_tess_merge_polys (pTess, cells[i], cellqty[i]);

  else if ((*pTess).Dim == 2)
    for (i = 0; i < cellidqty; i++)
      neut_tess_merge_faces (pTess, cells[i], cellqty[i]);

  else
    abort ();

  neut_tess_merge_ondomain (pTess);

  neut_tess_merge_finalize (pTess);

  ut_free_1d_int (cellqty);
  ut_free_2d_int (cells, cellidqty);
  ut_free_1d_int (cellids);

  return;
}
