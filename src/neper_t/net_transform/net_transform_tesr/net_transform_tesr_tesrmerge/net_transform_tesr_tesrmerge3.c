/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tesr_tesrmerge_.h"

void
net_transform_tesr_tesrmerge_selectcells (struct TESR *Tesrs,
                                          char *string, int *CellIds,
                                          int **CellStates, int CellQty,
                                          int **pcells, int **ptesrs,
                                          int *pcellqty)
{
  int i, cell, t, tesrqty;

  tesrqty = 0;
  (*pcellqty) = 0;
  ut_free_1d_int (pcells);
  ut_free_1d_int (ptesrs);

  if (!strcmp (string, "full-absent"))
  {
    for (i = 0; i < CellQty; i++)
      if (ut_array_1d_int_sum (CellStates[i], 2) == 1)
      {
        t = ut_array_1d_int_eltpos (CellStates[i], 2, 1);
        ut_array_1d_int_list_addval_nocheck (ptesrs, &tesrqty, t);
        neut_tesr_cellid_cell (Tesrs[t], CellIds[i], &cell);
        ut_array_1d_int_list_addval_nocheck (pcells, pcellqty, cell);
      }
  }

  else if (!strcmp (string, "full-full"))
  {
    for (i = 0; i < CellQty; i++)
      if (CellStates[i][0] == 1 && CellStates[i][1] == 1)
      {
        t = 0;
        ut_array_1d_int_list_addval_nocheck (ptesrs, &tesrqty, t);
        neut_tesr_cellid_cell (Tesrs[t], CellIds[i], &cell);
        ut_array_1d_int_list_addval_nocheck (pcells, pcellqty, cell);
      }
  }

  else if (!strcmp (string, "full-partial"))
  {
    for (i = 0; i < CellQty; i++)
      if (CellStates[i][0] + CellStates[i][1] == 3)
      {
        t = ut_array_1d_int_eltpos (CellStates[i], 2, 1);
        ut_array_1d_int_list_addval_nocheck (ptesrs, &tesrqty, t);
        neut_tesr_cellid_cell (Tesrs[t], CellIds[i], &cell);
        ut_array_1d_int_list_addval_nocheck (pcells, pcellqty, cell);
      }
  }

  else if (!strcmp (string, "partial-partial"))
  {
    for (i = 0; i < CellQty; i++)
      if (CellStates[i][0] + CellStates[i][1] == 4)
      {
        t = 0;
        ut_array_1d_int_list_addval_nocheck (ptesrs, &tesrqty, t);
        neut_tesr_cellid_cell (Tesrs[t], CellIds[i], &cell);
        ut_array_1d_int_list_addval_nocheck (pcells, pcellqty, cell);
      }
  }

  else if (!strcmp (string, "partial-absent"))
  {
    for (i = 0; i < CellQty; i++)
      if (CellStates[i][0] + CellStates[i][1] == 2 && (CellStates[i][0] == 2 || CellStates[i][1] == 2))
      {
        t = ut_array_1d_int_eltpos (CellStates[i], 2, 2);
        ut_array_1d_int_list_addval_nocheck (ptesrs, &tesrqty, t);
        neut_tesr_cellid_cell (Tesrs[t], CellIds[i], &cell);
        ut_array_1d_int_list_addval_nocheck (pcells, pcellqty, cell);
      }
  }

  else
    abort ();

  ut_print_message (0, 4, "%d cell%s found\n", *pcellqty, *pcellqty > 1 ? "s" : "");

  return;
}

void
net_transform_tesr_tesrmerge_copycells (struct TESR *Tesrs, int *cells,
                                        int *tesrs, int cellqty, struct TESR *pTesr)
{
  int i;

  for (i = 0; i < cellqty; i++)
    net_transform_tesr_tesrmerge_copycell (Tesrs[tesrs[i]], cells[i], pTesr);

  return;
}

void
net_transform_tesr_tesrmerge_mergecells (struct TESR *Tesrs, int *cells,
                                         int *tesrs, int cellqty, struct TESR *pTesr)
{
  int i, t, cellid;
  int *cellpair = ut_alloc_1d_int (2);

  for (i = 0; i < cellqty; i++)
  {
    t = tesrs[i];
    cellpair[t] = cells[i];
    cellid = neut_tesr_cell_id (Tesrs[t], cells[i]);
    neut_tesr_cellid_cell (Tesrs[1 - t], cellid, cellpair + 1 - t);

    net_transform_tesr_tesrmerge_mergecell (Tesrs, cellpair, pTesr);
  }

  ut_free_1d_int (&cellpair);

  return;
}

void
net_transform_tesr_tesrmerge_init_alloc (struct TESR *Tesrs, struct TESR *pTesr)
{
  int i, j;
  double **bbox = ut_alloc_2d (3, 2);
  double **tmp = ut_alloc_2d (3, 2);
  int *size = ut_alloc_1d_int (3);

  neut_tesr_set_zero (pTesr);

  (*pTesr).CellId = ut_alloc_1d_int (1);
  (*pTesr).CellOri = ut_alloc_2d (1, 4);
  ut_string_string (Tesrs[0].CellCrySym, &(*pTesr).CellCrySym);

  for (i = 0; i < 3; i++)
    ut_array_1d_set_2 (bbox[i], DBL_MAX, -DBL_MAX);

  for (i = 0; i < 2; i++)
  {
    neut_tesr_bbox (Tesrs[i], tmp);

    for (j = 0; j < 3; j++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], tmp[j][0]);
      bbox[j][1] = ut_num_max (bbox[j][1], tmp[j][1]);
    }
  }


  for (i = 0; i < 3; i++)
    size[i] = (int) ceil ((bbox[i][1] - bbox[i][0]) / Tesrs[0].vsize[i]);

  neut_tesr_alloc (pTesr, 3, size, Tesrs[0].vsize, "CellBBox,VoxOri");

  ut_array_1d_set_3 ((*pTesr).Origin, bbox[0][0], bbox[1][0], bbox[2][0]);

  ut_free_2d (&bbox, 3);
  ut_free_2d (&tmp, 3);
  ut_free_1d_int (&size);

  return;
}

void
net_transform_tesr_tesrmerge_copycell (struct TESR Tesr, int i, struct TESR *pTesr)
{
  int *pos = ut_alloc_1d_int (3);
  int *pos2 = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);

  neut_tesr_addcell (pTesr);

  (*pTesr).CellId[(*pTesr).CellQty] = neut_tesr_cell_id (Tesr, i);

  if (Tesr.CellOri)
    ut_array_1d_memcpy (Tesr.CellOri[i], 4, (*pTesr).CellOri[(*pTesr).CellQty]);

  for (pos2[2] = Tesr.CellBBox[i][2][0]; pos2[2] <= Tesr.CellBBox[i][2][1]; pos2[2]++)
    for (pos2[1] = Tesr.CellBBox[i][1][0]; pos2[1] <= Tesr.CellBBox[i][1][1]; pos2[1]++)
      for (pos2[0] = Tesr.CellBBox[i][0][0]; pos2[0] <= Tesr.CellBBox[i][0][1]; pos2[0]++)
        if (Tesr.VoxCell[pos2[0]][pos2[1]][pos2[2]] == i)
        {
          neut_tesr_pos_coo (Tesr, pos2, coo);
          neut_tesr_coo_pos (*pTesr, coo, 1, pos);
          (*pTesr).VoxCell[pos[0]][pos[1]][pos[2]] = (*pTesr).CellQty;
          ut_array_1d_memcpy (Tesr.VoxOri[pos2[0]][pos2[1]][pos2[2]], 4,
                              (*pTesr).VoxOri[pos[0]][pos[1]][pos[2]]);
        }

  ut_free_1d_int (&pos);
  ut_free_1d_int (&pos2);
  ut_free_1d (&coo);

  return;
}

void
net_transform_tesr_tesrmerge_mergecell (struct TESR *Tesrs, int *cells,
                                        struct TESR *pTesr)
{
  int i, j, t;
  int *pos = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);
  double **bbox_coo = ut_alloc_2d (3, 2);
  double **CellBBoxCoo = ut_alloc_2d (3, 2);

  double interzmin, interzmax;

  interzmin = ut_num_max (Tesrs[0].Origin[2], Tesrs[1].Origin[2]);
  interzmax = ut_num_min (Tesrs[0].Origin[2] + Tesrs[0].size[2] * Tesrs[0].vsize[2],
                          Tesrs[1].Origin[2] + Tesrs[1].size[2] * Tesrs[1].vsize[2]);

  neut_tesr_addcell (pTesr);
  int cell = (*pTesr).CellQty;

  (*pTesr).CellId[cell] = neut_tesr_cell_id (Tesrs[0], cells[0]);

  for (i = 0; i < 3; i++)
    ut_array_1d_set_2 (CellBBoxCoo[i], DBL_MAX, -DBL_MAX);

  for (t = 0; t < 2; t++)
  {
    neut_tesr_cell_bbox_coo (Tesrs[t], cells[t], bbox_coo);

    for (i = 0; i < 3; i++)
    {
      CellBBoxCoo[i][0] = ut_num_min (CellBBoxCoo[i][0], bbox_coo[i][0]);
      CellBBoxCoo[i][1] = ut_num_max (CellBBoxCoo[i][1], bbox_coo[i][1]);
    }
  }

  for (j = 0; j < 2; j++)
  {
    ut_array_1d_set_3 (coo, CellBBoxCoo[0][j], CellBBoxCoo[1][j], CellBBoxCoo[2][j]);
    neut_tesr_coo_pos (*pTesr, coo, -1, pos);

    for (i = 0; i < 3; i++)
      (*pTesr).CellBBox[cell][i][j] = pos[i];
  }

  for (pos[2] = (*pTesr).CellBBox[cell][2][0]; pos[2] <= (*pTesr).CellBBox[cell][2][1]; pos[2]++)
    for (pos[1] = (*pTesr).CellBBox[cell][1][0]; pos[1] <= (*pTesr).CellBBox[cell][1][1]; pos[1]++)
      for (pos[0] = (*pTesr).CellBBox[cell][0][0]; pos[0] <= (*pTesr).CellBBox[cell][0][1]; pos[0]++)
      {
        neut_tesr_pos_coo (*pTesr, pos, coo);

        int *found = ut_alloc_1d_int (2);
        double **VoxOris = ut_alloc_2d (2, 4);

        int **poss = ut_alloc_2d_int (2, 3);

        for (t = 0; t < 2; t++)
        {
          neut_tesr_coo_pos (Tesrs[t], coo, 0, poss[t]);

          if (neut_tesr_pos_valid (Tesrs[t], poss[t])
              && Tesrs[t].VoxCell[poss[t][0]][poss[t][1]][poss[t][2]] == cells[t])
          {
            found[t] = 1;
            ut_array_1d_memcpy (Tesrs[t].VoxOri[poss[t][0]][poss[t][1]][poss[t][2]], 4,
                                VoxOris[t]);
          }
        }

        double A = (coo[2] - interzmin) / (interzmax - interzmin);

        if (ut_array_1d_int_sum (found, 2) == 1)
        {
          t = ut_array_1d_int_eltpos (found, 2, 1);

          if (coo[2] < interzmin && t != 1)
            abort ();

          if (coo[2] > interzmax && t != 0)
            abort ();

          if ((coo[2] < interzmin) || (coo[2] > interzmax)
           || (A < 0.5 && t == 1) || (A >= 0.5 && t == 0))
          {
            (*pTesr).VoxCell[pos[0]][pos[1]][pos[2]] = cell;
            ol_q_memcpy (Tesrs[t].VoxOri[poss[t][0]][poss[t][1]][poss[t][2]],
                         (*pTesr).VoxOri[pos[0]][pos[1]][pos[2]]);
          }
        }

        else
        if (ut_array_1d_int_sum (found, 2) == 2)
        {
          (*pTesr).VoxCell[pos[0]][pos[1]][pos[2]] = cell;

          struct OL_SET Set = ol_set_alloc (2, (*pTesr).CellCrySym);

          for (t = 0; t < 2; t++)
            ol_q_memcpy (Tesrs[t].VoxOri[poss[t][0]][poss[t][1]][poss[t][2]], Set.q[t]);

          ut_array_1d_set_2 (Set.weight, A, 1 - A);

          ol_set_mean (Set, (*pTesr).VoxOri[pos[0]][pos[1]][pos[2]]);

          ol_set_free (&Set);
        }

        ut_free_1d_int (&found);
        ut_free_2d (&VoxOris, 2);
        ut_free_2d_int (&poss, 2);
      }

  neut_tesr_cell_oriaverage (pTesr, cell);

  ut_free_1d_int (&pos);
  ut_free_1d (&coo);
  ut_free_2d (&bbox_coo, 3);
  ut_free_2d (&CellBBoxCoo, 3);

  return;
}
