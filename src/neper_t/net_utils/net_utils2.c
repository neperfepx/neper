/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_utils_.h"

void
net_tess_tesr_cell (struct TESS Tess, int cell, struct TESR *pTesr)
{
  int i, j, k;
  double **bbox = NULL;
  double *coo = NULL;
  int *pos = NULL;

  if (neut_tess_cell_isvoid (Tess, cell))
    return;

  bbox = ut_alloc_2d (3, 2);
  coo = ut_alloc_1d (3);
  pos = ut_alloc_1d_int (3);

  neut_tess_cell_bbox (Tess, cell, bbox);

  for (i = 0; i < Tess.Dim; i++)
    for (j = 0; j < 2; j++)
      bbox[i][j] -= (*pTesr).Origin[i];

  for (i = 0; i < Tess.Dim; i++)
    for (j = 0; j < 2; j++)
    {
      (*pTesr).CellBBox[cell][i][j]
        = ut_num_d2ri (ceil (bbox[i][j] / (*pTesr).vsize[i] + 1e-6));
      if (bbox[i][j] < 0)
        (*pTesr).CellBBox[cell][i][j]--;
    }
  if (Tess.Dim == 2)
    ut_array_1d_int_set_2 ((*pTesr).CellBBox[cell][2], 1, 1);

  for (k = (*pTesr).CellBBox[cell][2][0]; k <= (*pTesr).CellBBox[cell][2][1]; k++)
    for (j = (*pTesr).CellBBox[cell][1][0]; j <= (*pTesr).CellBBox[cell][1][1]; j++)
      for (i = (*pTesr).CellBBox[cell][0][0]; i <= (*pTesr).CellBBox[cell][0][1]; i++)
      {
	ut_array_1d_int_set_3 (pos, i, j, k);
	neut_tesr_pos_coo (*pTesr, pos, coo);

	if (neut_tess_point_incell (Tess, coo, cell) == 1)
        {
          neut_tesr_perpos_pos (*pTesr, Tess.Periodic, pos, pos);
          if (pos[0] >= 1 && pos[0] <= (*pTesr).size[0]
           && pos[1] >= 1 && pos[1] <= (*pTesr).size[1]
           && pos[2] >= 1 && pos[2] <= (*pTesr).size[2])
            (*pTesr).VoxCell[pos[0]][pos[1]][pos[2]] = cell;
        }
      }

  ut_free_2d (bbox, 3);
  ut_free_1d_int (pos);
  ut_free_1d (coo);

  return;
}
