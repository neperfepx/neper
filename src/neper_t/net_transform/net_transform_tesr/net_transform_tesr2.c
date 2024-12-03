/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tesr_.h"

void
net_transform_tesr_tesrrenumber (struct IN_T In, char *string, struct TESR *pTesr)
{
  int i, j, id, valqty, parentcell, *parentcells = NULL;
  int *pos = ut_alloc_1d_int (3), *pos2 = ut_alloc_1d_int (3);
  int *old_new = ut_alloc_1d_int ((*pTesr).CellQty + 1);
  double *ratio = ut_alloc_1d ((*pTesr).CellQty + 1);
  double *coo = ut_alloc_1d (3);
  char *fct = NULL, **vals = NULL, *filename = NULL;
  FILE *file = NULL;
  struct TESR Tesr2;

  neut_tesr_set_zero (&Tesr2);

  ut_string_function (string, &fct, NULL, &vals, &valqty);

  neut_tesr_fnscanf (vals[0], &Tesr2);

  parentcells = ut_alloc_1d_int (Tesr2.CellQty + 1);

  for (i = 1; i <= (*pTesr).CellQty; i++)
  {
    ut_array_1d_int_zero (parentcells, Tesr2.CellQty + 1);

    for (pos[0] = (*pTesr).CellBBox[i][0][0]; pos[0] <= (*pTesr).CellBBox[i][0][1]; pos[0]++)
      for (pos[1] = (*pTesr).CellBBox[i][1][0]; pos[1] <= (*pTesr).CellBBox[i][1][1]; pos[1]++)
        for (pos[2] = (*pTesr).CellBBox[i][2][0]; pos[2] <= (*pTesr).CellBBox[i][2][1]; pos[2]++)
          if ((*pTesr).VoxCell[pos[0]][pos[1]][pos[2]] == i)
          {
            neut_tesr_pos_coo (*pTesr, pos, coo);
            neut_tesr_coo_pos (Tesr2, coo, 0, pos2);
            parentcell = Tesr2.VoxCell[pos2[0]][pos2[1]][pos2[2]];
            parentcells[parentcell]++;
          }

    id = 1 + ut_array_1d_int_max_index (parentcells + 1, Tesr2.CellQty);

    ratio[i] = (double) parentcells[id] / neut_tesr_cell_voxqty (*pTesr, i);

    old_new[i] = id;
  }

  // checking for bijectivity
  int new_max = ut_array_1d_int_max (old_new + 1, (*pTesr).CellQty);
  int **new_olds = ut_alloc_1d_pint (new_max + 1);
  int *new_oldqty = ut_alloc_1d_int (new_max + 1);

  for (i = 1; i <= (*pTesr).CellQty; i++)
    ut_array_1d_int_list_addval_nocheck (new_olds + old_new[i], new_oldqty + old_new[i], i);

  if (valqty == 2 && !strcmp (vals[1], "log"))
  {
    filename = ut_string_addextension (In.body, ".logtesrrenumber");
    file = ut_file_open (filename, "w");

    for (i = 1; i <= (*pTesr).CellQty; i++)
      fprintf (file, "%d %d %f\n", i, old_new[i], ratio[i]);

    ut_file_close (file, filename, "w");
  }

  for (i = 1; i <= new_max; i++)
    if (new_oldqty[i] > 1)
    {
      ut_print_message (1, 3, "Attempting to merge %d cells as %d(%d):", new_oldqty[i], i, Tesr2.CellId[i]);
      for (j = 0; j < new_oldqty[i]; j++)
        printf (" %d(%d/%f)", new_olds[i][j], (*pTesr).CellId[new_olds[i][j]], ratio[new_olds[i][j]]);
      printf ("\n");
    }

  if (ut_array_1d_int_max (new_oldqty + 1, new_max) > 1)
    abort ();
  ut_free_2d_int (&new_olds, new_max);

  // settings ids
  for (i = 1; i <= (*pTesr).CellQty; i++)
    (*pTesr).CellId[i] = Tesr2.CellId[old_new[i]];

  ut_free_1d_int (&parentcells);
  ut_free_1d_int (&pos);
  ut_free_1d_int (&pos2);
  ut_free_1d_int (&old_new);
  ut_free_1d (&coo);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, valqty);
  ut_free_1d_char (&filename);
  neut_tesr_free (&Tesr2);

  return;
}

void
net_transform_tesr_orirotate (char *string, struct TESR *pTesr)
{
  int i, valqty, cellqty, *cells = NULL;
  double theta, *r = ol_r_alloc ();
  char *fct = NULL, **vars = NULL, **vals = NULL, *cellexpr = NULL, *csysexpr = NULL;

  ut_string_string ("1", &cellexpr);
  ut_string_string ("ref", &csysexpr);

  ut_string_function (string, &fct, &vars, &vals, &valqty);

  if (strcmp (fct, "orirotate") || valqty < 4)
    abort ();

  for (i = 0; i < 3; i++)
    sscanf (vals[i], "%lf", r + i);
  ol_r_set_unit (r);
  sscanf (vals[3], "%lf", &theta);

  for (i = 4; i < valqty; i++)
  {
    if (!ut_string_strcmp (vars[i], "cell"))
      ut_string_string (vals[i], &cellexpr);
    else if (!ut_string_strcmp (vars[i], "csys"))
      ut_string_string (vals[i], &csysexpr);
    else
      ut_print_exprbug (vars[i]);
  }

  neut_tesr_expr_cells (pTesr, cellexpr, &cells, &cellqty);

  for (i = 0; i < cellqty; i++)
    neut_tesr_cell_orirotate (pTesr, cells[i], r, theta, csysexpr);

  ut_free_1d_int (&cells);
  ol_r_free (r);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);
  ut_free_1d_char (&cellexpr);
  ut_free_1d_char (&csysexpr);

  return;
}

void
net_transform_tesr_ori (char *ori, struct TESR *pTesr)
{
  int nblines, valqty;
  char *fct = NULL, **vars = NULL, **vals = NULL, *filename = ut_alloc_1d_char (1000);
  struct OL_SET OSet;

  ut_string_function (ori, &fct, &vars, &vals, &valqty);

  if (strcmp (fct, "ori"))
    abort ();

  ut_string_string (ori, &filename);
  ut_string_fnrs (filename, "ori(", "file(", 1);

  nblines = ut_file_nblines (vals[0]);

  OSet = ol_set_alloc (nblines, (*pTesr).CellCrySym);

  net_ori_file (filename, &OSet);

  if (nblines != (*pTesr).CellQty)
    ut_print_message (2, 4, "Wrong number of entries in file `%s'.\n", vals[0]);

  ut_array_2d_memcpy (OSet.q, (*pTesr).CellQty, 4, (*pTesr).CellOri + 1);

  ol_set_free (&OSet);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);
  ut_free_1d_char (&filename);

  return;
}
