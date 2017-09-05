/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesr_op_.h"

void
net_tesr_cell_segment (struct TESR Tesr, int cell, int dim, struct TESR *pSeg)
{
  int i, j, k, l, m, qty, rpt1qty, rpt2qty;
  int **tesr = NULL;
  int **rptnew = NULL;
  int **tmp = NULL;

  neut_tesr_free (pSeg);
  neut_tesr_set_zero (pSeg);
  neut_tesr_alloc (pSeg, Tesr.Dim, Tesr.size, Tesr.vsize);

  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
	if (Tesr.VoxCell[i][j][k] == cell && (*pSeg).VoxCell[i][j][k] == 0)
	{
	  (*pSeg).VoxCell[i][j][k] = ++(*pSeg).CellQty;

	  rpt1qty = 1;
	  tesr = ut_alloc_2d_int (1, 3);
	  ut_array_1d_int_set_3 (tesr[0], i, j, k);

	  while (rpt1qty > 0)
	  {
	    rpt2qty = 0;

	    for (l = 0; l < rpt1qty; l++)
	    {
	      neut_tesr_cell_pos_neighpos (Tesr, cell, tesr[l], dim, &tmp,
					   &qty);

	      for (m = 0; m < qty; m++)
		if (Tesr.VoxCell[tmp[m][0]][tmp[m][1]][tmp[m][2]] != 0
		    && (*pSeg).VoxCell[tmp[m][0]][tmp[m][1]][tmp[m][2]] == 0)
		  if (ut_array_2d_int_linepos (rptnew, rpt2qty, 3, tmp[m]) ==
		      -1)
		  {
		    rptnew = ut_realloc_2d_int_addline (rptnew, ++rpt2qty, 3);
		    ut_array_1d_int_memcpy (rptnew[rpt2qty - 1], 3, tmp[m]);
		  }

	      ut_free_2d_int (tmp, qty);
	      tmp = NULL;
	    }

	    for (m = 0; m < rpt2qty; m++)
	      (*pSeg).VoxCell[rptnew[m][0]][rptnew[m][1]][rptnew[m][2]] =
		(*pSeg).CellQty;

	    ut_free_2d_int (tesr, rpt1qty);
	    rpt1qty = rpt2qty;
	    tesr = ut_alloc_2d_int (rpt1qty, 3);
	    ut_array_2d_int_memcpy (tesr, rpt1qty, 3, rptnew);
	    ut_free_2d_int (rptnew, rpt2qty);
	    rptnew = NULL;
	  }

	  ut_free_2d_int (tesr, rpt1qty);
	}

  return;
}

void
net_tesr_segment (struct TESR Tesr, int dim, struct TESR *pSeg)
{
  int i, j, k, l, m, qty, rpt1qty, rpt2qty;
  int **tesr = NULL;
  int **rptnew = NULL;
  int **tmp = NULL;

  neut_tesr_set_zero (pSeg);
  neut_tesr_alloc (pSeg, Tesr.Dim, Tesr.size, Tesr.vsize);

  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
	if (Tesr.VoxCell[i][j][k] != 0 && (*pSeg).VoxCell[i][j][k] == 0)
	{
	  (*pSeg).VoxCell[i][j][k] = ++(*pSeg).CellQty;

	  rpt1qty = 1;
	  tesr = ut_alloc_2d_int (1, 3);
	  ut_array_1d_int_set_3 (tesr[0], i, j, k);

	  while (rpt1qty > 0)
	  {
	    rpt2qty = 0;

	    for (l = 0; l < rpt1qty; l++)
	    {
	      neut_tesr_cells_pos_neighpos (Tesr, NULL, 0, tesr[l], dim, &tmp,
					    &qty);

	      for (m = 0; m < qty; m++)
		if (Tesr.VoxCell[tmp[m][0]][tmp[m][1]][tmp[m][2]] != 0
		    && (*pSeg).VoxCell[tmp[m][0]][tmp[m][1]][tmp[m][2]] == 0)
		  if (ut_array_2d_int_linepos (rptnew, rpt2qty, 3, tmp[m]) ==
		      -1)
		  {
		    rptnew = ut_realloc_2d_int_addline (rptnew, ++rpt2qty, 3);
		    ut_array_1d_int_memcpy (rptnew[rpt2qty - 1], 3, tmp[m]);
		  }

	      ut_free_2d_int (tmp, qty);
	    }

	    for (m = 0; m < rpt2qty; m++)
	      (*pSeg).VoxCell[rptnew[m][0]][rptnew[m][1]][rptnew[m][2]] =
		(*pSeg).CellQty;

	    ut_free_2d_int (tesr, rpt1qty);
	    rpt1qty = rpt2qty;
	    tesr = ut_alloc_2d_int (rpt1qty, 3);
	    ut_array_2d_int_memcpy (tesr, rpt1qty, 3, rptnew);
	    ut_free_2d_int (rptnew, rpt2qty);
	    rptnew = NULL;
	  }

	  ut_free_2d_int (tesr, rpt1qty);
	}

  return;
}
