/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tesr_tesrtranslate_.h"

void
net_transform_tesr_tesrtranslate_pre (char *string, struct TESR *pTesr,
                                      struct TESR *pTesr2, int **pcells,
                                      int **prefcells, int *pcellqty,
                                      char ***palgos, int *palgoqty)
{
  int i, valqty;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  neut_tesr_set_zero (pTesr2);

  // pre

  ut_string_function (string, &fct, &vars, &vals, &valqty);

  if (strcmp (fct, "tesrtranslate") || valqty < 1)
    abort ();

  *pcellqty = 0;
  *palgoqty = 0;

  for (i = 1; i < valqty; i++)
    if (!strcmp (vars[i], "cell"))
    {
      neut_tesr_expr_cells (pTesr, vals[i], pcells, pcellqty);
      if (*pcellqty == 0)
        ut_print_message (2, 3, "No cell matching expression `%s'.\n", vals[i]);
    }
    else if (!strcmp (vars[i], "val"))
      ut_list_break (vals[i], NEUT_SEP_DEP, palgos, palgoqty);

    else
      ut_print_exprbug (vars[i]);

  if (!*pcellqty)
  {
    *pcellqty = (*pTesr).CellQty;
    (*pcells) = ut_alloc_1d_int (*pcellqty + 1);
    ut_array_1d_int_set_id (*pcells, *pcellqty + 1);
  }

  if (!*palgoqty)
  {
    *palgoqty = 1;
    (*palgos) = ut_alloc_1d_pchar (1);
    ut_string_string ("centre", *palgos);
  }

  neut_tesr_fnscanf (vals[0], pTesr2);

  (*prefcells) = ut_alloc_1d_int (*pcellqty);
  for (i = 0; i < *pcellqty; i++)
    neut_tesr_cellid_cell (*pTesr2, neut_tesr_cell_id (*pTesr, (*pcells)[i]), (*prefcells) + i);

  ut_print_message (0, 4, "Using %d cells...\n", *pcellqty);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);

  return;
}

void
net_transform_tesr_tesrtranslate_centres (struct TESR *pTesr, int *cells,
                                          int *refcells, int cellqty, struct TESR Tesr2)
{
  double *refcoo = ut_alloc_1d (3);
  double *coo = ut_alloc_1d (3);
  double *s = ut_alloc_1d (3);

  neut_tesr_cells_centre (*pTesr, cells, cellqty, coo);
  neut_tesr_cells_centre (Tesr2, refcells, cellqty, refcoo);

  ut_array_1d_sub (coo, refcoo, 3, s);
  ut_array_1d_add ((*pTesr).Origin, s, 3, (*pTesr).Origin);

  ut_free_1d (&coo);
  ut_free_1d (&refcoo);
  ut_free_1d (&s);

  return;
}
