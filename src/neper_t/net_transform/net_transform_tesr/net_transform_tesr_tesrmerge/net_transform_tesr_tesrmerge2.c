/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tesr_tesrmerge_.h"

void
net_transform_tesr_tesrmerge_init (char *string, struct TESR *pTesr,
                                   struct TESR **pTesrs, int **pCellIds,
                                   int ***pCellStates,  int *pCellQty,
                                   char **pstrategy)
{
  int valqty, cell, i, t, *bbox = ut_alloc_1d_int (2);
  char *fct = NULL, **vars = NULL, **vals = NULL;


  neut_tesr_memcpy (*pTesr, pTesrs[0]);

  neut_tesr_set_zero (&(*pTesrs)[1]);

  ut_string_function (string, &fct, &vars, &vals, &valqty);

  if (strcmp (fct, "tesrmerge") || valqty < 1)
    abort ();

  neut_tesr_fnscanf (vals[0], &(*pTesrs)[1]);

  if (valqty == 2)
  {
    if (!vars[1] || !strcmp (vars[1], "strategy"))
      ut_string_string (vals[1], pstrategy);
    else
      ut_print_exprbug (vars[1]);
  }

  neut_tesr_free (pTesr);

  neut_tesr_init_cellbbox ((*pTesrs));
  neut_tesr_init_cellbbox ((*pTesrs) + 1);

  for (t = 0; t < 2; t++)
  {
    ut_array_1d_int_set_2 (bbox, INT_MAX, -INT_MAX);

    for (i = 1; i <= (*pTesrs)[t].CellQty; i++)
    {
      bbox[0] = ut_num_min (bbox[0], (*pTesrs)[t].CellBBox[i][2][0]);
      bbox[1] = ut_num_max (bbox[1], (*pTesrs)[t].CellBBox[i][2][1]);
    }

    if (bbox[0] != 1 || bbox[1] != (*pTesrs)[t].size[2])
      ut_print_message (2, 4, "`autocrop' was not run on tessellation %d\n", t + 1);
  }

  // the -loadtesr tesr must be on top (can be generalized)
  if ((*pTesrs)[1].Origin[2] > (*pTesrs)[0].Origin[2])
    abort ();

  net_transform_tesr_tesrmerge_init_alloc (*pTesrs, pTesr);

  ut_array_1d_int_listpair_merge ((*pTesrs)[0].CellId + 1, (*pTesrs)[0].CellQty,
                                  (*pTesrs)[1].CellId + 1, (*pTesrs)[1].CellQty,
                                  pCellIds, pCellQty);
  // int *CellIdInv = NULL, CellIdMax;
  // ut_array_1d_int_inv (CellId, CellQty, &CellIdInv, &CellIdMax);

  (*pCellStates) = ut_alloc_2d_int (*pCellQty, 2);

  for (i = 0; i < *pCellQty; i++)
    for (t = 0; t <= 1; t++)
      if (!neut_tesr_cellid_cell ((*pTesrs)[t], (*pCellIds)[i], &cell))
      {
        (*pCellStates)[i][t] = 1;

        if ((t == 0 && (*pTesrs)[t].CellBBox[cell][2][0] == 1)
         || (t == 1 && (*pTesrs)[t].CellBBox[cell][2][1] == (*pTesrs)[t].size[2]))
          (*pCellStates)[i][t] = 2;
      }

  ut_print_message (0, 3, "New volume has %dx%dx%d voxels.\n",
                    (*pTesr).size[0], (*pTesr).size[1], (*pTesr).size[2]);
  ut_print_message (0, 3, "%d cells detected.\n", *pCellQty);

  for (t = 0; t < 2; t++)
    if (!(*pTesrs)[t].CellCrySym)
      ut_print_message (2, 3, "*crysym not defined for tess %d\n", t);

  ut_free_1d_int (&bbox);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);

  return;
}

void
net_transform_tesr_tesrmerge_direct (struct TESR *Tesrs, int *CellIds,
                                     int **CellStates, int CellQty,
                                     struct TESR *pTesr)
{
  int i, cell, t, *pos = ut_alloc_1d_int (3), *pos2 = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);
  double interzmin, interzmax, interzmean;

  (void) CellStates;

  interzmin = ut_num_max (Tesrs[0].Origin[2], Tesrs[1].Origin[2]);
  interzmax = ut_num_min (Tesrs[0].Origin[2] + Tesrs[0].size[2] * Tesrs[0].vsize[2],
                          Tesrs[1].Origin[2] + Tesrs[1].size[2] * Tesrs[1].vsize[2]);
  interzmean = .5 * (interzmin + interzmax);

  for (i = 0; i < CellQty; i++)
  {
    neut_tesr_addcell (pTesr);
    (*pTesr).CellId[(*pTesr).CellQty] = CellIds[i];

    for (t = 0; t < 2; t++)
      if (!neut_tesr_cellid_cell (Tesrs[t], CellIds[i], &cell))
        for (pos2[2] = Tesrs[t].CellBBox[cell][2][0]; pos2[2] <= Tesrs[t].CellBBox[cell][2][1]; pos2[2]++)
            for (pos2[1] = Tesrs[t].CellBBox[cell][1][0]; pos2[1] <= Tesrs[t].CellBBox[cell][1][1]; pos2[1]++)
              for (pos2[0] = Tesrs[t].CellBBox[cell][0][0]; pos2[0] <= Tesrs[t].CellBBox[cell][0][1]; pos2[0]++)
                if (Tesrs[t].VoxCell[pos2[0]][pos2[1]][pos2[2]] == cell)
                {
                  neut_tesr_pos_coo (Tesrs[t], pos2, coo);
                  if ((t == 0 && coo[2] >= interzmean - (*pTesr).vsize[2]) || (t == 1 && coo[2] < interzmean))
                  {
                    neut_tesr_coo_pos (*pTesr, coo, 1, pos);
                    (*pTesr).VoxCell[pos[0]][pos[1]][pos[2]] = (*pTesr).CellQty;
                    ut_array_1d_memcpy (Tesrs[t].VoxOri[pos2[0]][pos2[1]][pos2[2]], 4,
                                        (*pTesr).VoxOri[pos[0]][pos[1]][pos[2]]);
                  }
                }
  }

  neut_tesr_init_cellbbox (pTesr);
  neut_tesr_oriaverage (pTesr);

  ut_free_1d_int (&pos);
  ut_free_1d_int (&pos2);
  ut_free_1d (&coo);

  return;
}

void
net_transform_tesr_tesrmerge_smart (struct TESR *Tesrs, int *CellIds,
                                    int **CellStates, int CellQty,
                                    struct TESR *pTesr)
{
  int *cells = NULL, *tesrs = NULL, cellqty;

  ut_print_message (0, 3, "Copying cells that are fully in one tesr...\n");
  net_transform_tesr_tesrmerge_selectcells (Tesrs, "full-absent", CellIds,
                                            CellStates, CellQty, &cells,
                                            &tesrs, &cellqty);
  net_transform_tesr_tesrmerge_copycells (Tesrs, cells, tesrs, cellqty, pTesr);

  ut_print_message (0, 3, "Copying cells that are fully in both tesrs...\n");
  net_transform_tesr_tesrmerge_selectcells (Tesrs, "full-full", CellIds,
                                            CellStates, CellQty, &cells,
                                            &tesrs, &cellqty);
  net_transform_tesr_tesrmerge_copycells (Tesrs, cells, tesrs, cellqty, pTesr);

  ut_print_message (0, 3, "Copying cells that are fully in one tesr, partially in the other...\n");
  net_transform_tesr_tesrmerge_selectcells (Tesrs, "full-partial", CellIds,
                                            CellStates, CellQty, &cells,
                                            &tesrs, &cellqty);
  net_transform_tesr_tesrmerge_copycells (Tesrs, cells, tesrs, cellqty, pTesr);

  ut_print_message (0, 3, "Merging cells that are partially in both tesrs...\n");
  net_transform_tesr_tesrmerge_selectcells (Tesrs, "partial-partial", CellIds,
                                            CellStates, CellQty, &cells,
                                            &tesrs, &cellqty);
  net_transform_tesr_tesrmerge_mergecells (Tesrs, cells, tesrs, cellqty, pTesr);

  ut_print_message (0, 3, "Copying cells that are partially in one tesr and not in the other...\n");
  net_transform_tesr_tesrmerge_selectcells (Tesrs, "partial-absent", CellIds,
                                            CellStates, CellQty, &cells,
                                            &tesrs, &cellqty);
  net_transform_tesr_tesrmerge_copycells (Tesrs, cells, tesrs, cellqty, pTesr);

  ut_free_1d_int (&cells);
  ut_free_1d_int (&tesrs);

  return;
}
