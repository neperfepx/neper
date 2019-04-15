/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_merge_.h"

int
neut_tess_cells_merge_nocompress (struct TESS *pTess, int *cells, int cellqty,
                                  double coplanar)
{
  int newcell = -1;

  if (cellqty >= 1)
  {
    if ((*pTess).Dim == 3)
      newcell = neut_tess_polys_merge (pTess, cells, cellqty);

    else if ((*pTess).Dim == 2)
      neut_tess_faces_merge (pTess, cells, cellqty);

    else
      abort ();
  }

  else
    return -1;

  if ((*pTess).Dim == 3)
  {
    neut_tess_merge_polyfaces (pTess, newcell, coplanar);

    neut_tess_merge_polyedges (pTess, newcell, coplanar);
  }

  return newcell;
}


int
neut_tess_cells_merge (struct TESS *pTess, int *cells, int cellqty,
                       double coplanar)
{
  int status;

  status = neut_tess_cells_merge_nocompress (pTess, cells, cellqty, coplanar);

  neut_tess_compress (pTess);

  return status;
}

int
neut_tess_cellexpr_merge (struct TESS *pTess, char *expr, double coplanar)
{
  int newcell, cellqty, *cells = NULL;

  neut_tess_expr_celllist (*pTess, expr, &cells, &cellqty);

  newcell = neut_tess_cells_merge (pTess, cells, cellqty, coplanar);

  ut_free_1d_int (cells);

  return newcell;
}

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

    neut_tess_cells_merge (pTess, cells[i], cellqty[i], 1);
  }

  ut_free_1d_int (cellqty);
  ut_free_2d_int (cells, cellidqty);
  ut_free_1d_int (cellids);

  return;
}
