/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesr_op_.h"

void
net_tesr_cell_segment (struct TESR Tesr, int cell, int dim, struct TESR *pSeg)
{
  int i, j, k, l, m, qty, vox1qty, vox2qty;
  int **tesr = NULL;
  int **voxnew = NULL;
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

          vox1qty = 1;
          tesr = ut_alloc_2d_int (1, 3);
          ut_array_1d_int_set_3 (tesr[0], i, j, k);

          while (vox1qty > 0)
          {
            vox2qty = 0;

            for (l = 0; l < vox1qty; l++)
            {
              neut_tesr_cell_pos_neighpos (Tesr, cell, tesr[l], dim, &tmp,
                                           &qty);

              for (m = 0; m < qty; m++)
                if (Tesr.VoxCell[tmp[m][0]][tmp[m][1]][tmp[m][2]] != 0
                    && (*pSeg).VoxCell[tmp[m][0]][tmp[m][1]][tmp[m][2]] == 0)
                  if (ut_array_2d_int_linepos (voxnew, vox2qty, 3, tmp[m]) ==
                      -1)
                  {
                    voxnew = ut_realloc_2d_int_addline (voxnew, ++vox2qty, 3);
                    ut_array_1d_int_memcpy (tmp[m], 3, voxnew[vox2qty - 1]);
                  }

              ut_free_2d_int (&tmp, qty);
              tmp = NULL;
            }

            for (m = 0; m < vox2qty; m++)
              (*pSeg).VoxCell[voxnew[m][0]][voxnew[m][1]][voxnew[m][2]] =
                (*pSeg).CellQty;

            ut_free_2d_int (&tesr, vox1qty);
            vox1qty = vox2qty;
            tesr = ut_alloc_2d_int (vox1qty, 3);
            ut_array_2d_int_memcpy (voxnew, vox1qty, 3, tesr);
            ut_free_2d_int (&voxnew, vox2qty);
            voxnew = NULL;
          }

          ut_free_2d_int (&tesr, vox1qty);
        }

  return;
}

void
net_tesr_segment (struct TESR Tesr, int dim, struct TESR *pSeg)
{
  int i, j, k, l, m, qty, vox1qty, vox2qty;
  int **tesr = NULL;
  int **voxnew = NULL;
  int **tmp = NULL;

  neut_tesr_set_zero (pSeg);
  neut_tesr_alloc (pSeg, Tesr.Dim, Tesr.size, Tesr.vsize);

  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
        if (Tesr.VoxCell[i][j][k] != 0 && (*pSeg).VoxCell[i][j][k] == 0)
        {
          (*pSeg).VoxCell[i][j][k] = ++(*pSeg).CellQty;

          vox1qty = 1;
          tesr = ut_alloc_2d_int (1, 3);
          ut_array_1d_int_set_3 (tesr[0], i, j, k);

          while (vox1qty > 0)
          {
            vox2qty = 0;

            for (l = 0; l < vox1qty; l++)
            {
              neut_tesr_cells_pos_neighpos (Tesr, NULL, 0, tesr[l], dim, &tmp,
                                            &qty);

              for (m = 0; m < qty; m++)
                if (Tesr.VoxCell[tmp[m][0]][tmp[m][1]][tmp[m][2]] != 0
                    && (*pSeg).VoxCell[tmp[m][0]][tmp[m][1]][tmp[m][2]] == 0)
                  if (ut_array_2d_int_linepos (voxnew, vox2qty, 3, tmp[m]) ==
                      -1)
                  {
                    voxnew = ut_realloc_2d_int_addline (voxnew, ++vox2qty, 3);
                    ut_array_1d_int_memcpy (tmp[m], 3, voxnew[vox2qty - 1]);
                  }

              ut_free_2d_int (&tmp, qty);
            }

            for (m = 0; m < vox2qty; m++)
              (*pSeg).VoxCell[voxnew[m][0]][voxnew[m][1]][voxnew[m][2]] =
                (*pSeg).CellQty;

            ut_free_2d_int (&tesr, vox1qty);
            vox1qty = vox2qty;
            tesr = ut_alloc_2d_int (vox1qty, 3);
            ut_array_2d_int_memcpy (voxnew, vox1qty, 3, tesr);
            ut_free_2d_int (&voxnew, vox2qty);
            voxnew = NULL;
          }

          ut_free_2d_int (&tesr, vox1qty);
        }

  return;
}
