/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesr_op_.h"

// The filter is applied on a per cell basis.
void
neut_tesr_rmsat_cell_find (struct TESR Tesr, int dim, int *pvoxqty,
                           int ***pvoxpos)
{
  int i, j, k, p, part;
  struct TESR Seg;
  int *voxqty = NULL;

  (*pvoxqty) = 0;

  for (p = 1; p <= Tesr.CellQty; p++)
  {
    neut_tesr_set_zero (&Seg);

    // for cell, looking for the different parts
    net_tesr_cell_segment (Tesr, p, dim, &Seg);

    // if more than one part, marking points for filtering
    if (Seg.CellQty > 1)
    {
      // looking for the bigger part (will keep this one)
      neut_tesr_cells_voxqty (Seg, &voxqty);
      part = 1 + ut_array_1d_int_max_index (voxqty + 1, Seg.CellQty);

      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
            if (Seg.VoxCell[i][j][k] != 0 && Seg.VoxCell[i][j][k] != part)
            {
              (*pvoxpos) =
                ut_realloc_2d_int_addline ((*pvoxpos), ++(*pvoxqty), 3);
              ut_array_1d_int_set_3 ((*pvoxpos)[(*pvoxqty) - 1], i, j, k);
            }

      ut_free_1d_int (&voxqty);
    }

    neut_tesr_free (&Seg);
  }

  return;
}

void
neut_tesr_rmsat_cell_remove (struct TESR *pTesr, int dim, int voxqty,
                             int **voxpos)
{
  int i;

  (void) dim;

  for (i = 0; i < voxqty; i++)
    (*pTesr).VoxCell[voxpos[i][0]][voxpos[i][1]][voxpos[i][2]] = 0;

  /* legacy, caused possible infinite loop because neighcell could be the old cell.
   * This could be fixed, but it seems more natural to just set to 0 and follow by 'grow'

  int i, j, neighqty, id;
  int **neighpos = NULL;
  int *pos = NULL;
  int *cells = NULL, *cellqtys = NULL, cellqty;

  for (i = 0; i < voxqty; i++)
  {
    neut_tesr_cells_pos_neighpos (*pTesr, NULL, -1, voxpos[i], dim, &neighpos,
                                  &neighqty);

    int *neighcell = ut_alloc_1d_int (neighqty);
    for (j = 0; j < neighqty; j++)
    {
      pos = neighpos[j];
      neighcell[j] = (*pTesr).VoxCell[pos[0]][pos[1]][pos[2]];
    }

    ut_array_1d_int_valnbs (neighcell, neighqty, &cells, &cellqtys, &cellqty);
    id = ut_array_1d_int_max_index (cellqtys, cellqty);
    int newcell = cells[id];

    (*pTesr).VoxCell[voxpos[i][0]][voxpos[i][1]][voxpos[i][2]] = newcell;

    ut_free_1d_int (&neighcell);
  }

  ut_free_2d_int (&neighpos, neighqty);
  */

  return;
}

int
neut_tesr_grow_neigh (struct TESR *pTesr, int poly, int qty, int itermax,
                      int *piterqty)
{
  int i, j, k, ii, jj, kk, dataqty, filqty, totfilqty;
  int *data = NULL;
  int *vals = NULL, *valqtys = NULL;
  int valqty;
  struct TESR TesrCpy;
  int voxqty;

  neut_tesr_set_zero (&TesrCpy);

  data = ut_alloc_1d_int (pow (3, (*pTesr).Dim) - 1);

  voxqty = 0;
  totfilqty = 0;
  (*piterqty) = 1;
  do
  {
    neut_tesr_memcpy (*pTesr, &TesrCpy);

    filqty = 0;
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
          if ((*pTesr).VoxCell[i][j][k] == poly)
          {
            voxqty++;
            dataqty = 0;
            for (kk = k - 1; kk <= k + 1; kk++)
              for (jj = j - 1; jj <= j + 1; jj++)
                for (ii = i - 1; ii <= i + 1; ii++)
                  if (!(ii == i && jj == j && kk == k) && ii >= 1
                      && ii <= (*pTesr).size[0] && jj >= 1
                      && jj <= (*pTesr).size[1] && kk >= 1
                      && kk <= (*pTesr).size[2]
                      && TesrCpy.VoxCell[ii][jj][kk] != poly)
                    data[dataqty++] = TesrCpy.VoxCell[ii][jj][kk];

            int maxid, maxval;
            if (dataqty >= qty)
            {
              ut_array_1d_int_valnbs (data, dataqty, &vals, &valqtys,
                                      &valqty);
              maxid = ut_array_1d_int_max_index (valqtys, valqty);
              maxval = vals[maxid];
              filqty++;
              (*pTesr).VoxCell[i][j][k] = maxval;
              ut_free_1d_int (&vals);
              ut_free_1d_int (&valqtys);
            }
          }
    totfilqty += filqty;
  }
  while (filqty > 0 && ++(*piterqty) <= itermax);

  ut_free_1d_int (&data);
  neut_tesr_free (&TesrCpy);

  return (voxqty > 0) ? totfilqty : -1;
}
