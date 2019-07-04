/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_sort_.h"

void
net_sort_tess (struct TESS *pTess, char *expr)
{
  int i, j, status;
  int var_qty;
  char **vars = NULL;
  double *vals = NULL;
  double *res = NULL;
  char *cell = NULL;
  int *tmp = NULL;
  int *tmp2 = NULL;
  int qty2;

  neut_tess_cell (*pTess, &cell);

  neut_tess_var_list (*pTess, cell, &vars, &var_qty);
  vals = ut_alloc_1d (var_qty);

  res = ut_alloc_1d ((*pTess).CellQty + 1);
  (*pTess).CellId = ut_alloc_1d_int ((*pTess).CellQty + 1);
  for (i = 1; i <= (*pTess).CellQty; i++)
  {
    for (j = 0; j < var_qty; j++)
      if (strstr (expr, vars[j]))
	neut_tess_var_val_one (*pTess, NULL, NULL, NULL, cell, i, vars[j],
			       vals + j, NULL);

    status = ut_math_eval (expr, var_qty, vars, vals, &(res[i]));
    if (status != 0)
      ut_print_message (2, 2, "net_sort_tess: failed to process expression.\n");
  }

  tmp = ut_alloc_1d_int ((*pTess).CellQty);
  ut_array_1d_sort_index (res + 1, (*pTess).CellQty, tmp);
  ut_array_1d_int_inv (tmp, (*pTess).CellQty, &tmp2, &qty2);
  ut_array_1d_int_memcpy ((*pTess).CellId + 1, (*pTess).CellQty, tmp2);
  ut_array_1d_int_addval ((*pTess).CellId + 1, (*pTess).CellQty, 1,
			  (*pTess).CellId + 1);

  ut_free_1d_int (tmp);
  ut_free_1d_int (tmp2);
  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);
  ut_free_1d (res);
  ut_free_1d_char (cell);

  return;
}
