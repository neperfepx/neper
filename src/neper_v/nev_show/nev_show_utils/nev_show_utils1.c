/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_utils_.h"

int
nev_show_genexpr (char *expr, int *CellId, int *val, int valqty)
{
  int id, status = -1;
  FILE *file = NULL;

  ut_array_1d_int_zero (val + 1, valqty);

  if (!strcmp (expr, "all") || !strcmp (expr, "1"))
  {
    ut_array_1d_int_set (val + 1, valqty, 1);
    return 0;
  }

  else if (!strcmp (expr, "none") || !strcmp (expr, "0"))
  {
    ut_array_1d_int_set (val + 1, valqty, 0);
    return 0;
  }

  else if (ut_string_isfilename (expr))
  {
    int CellIdMax, *CellIdInv = NULL;

    if (CellId)
      ut_array_1d_int_inv (CellId, valqty + 1, &CellIdInv, &CellIdMax);

    file = ut_file_open (expr, "r");
    while (fscanf (file, "%d", &id) != EOF)
      val[CellId ? CellIdInv[id] : id] = 1;

    ut_free_1d_int (&CellIdInv);

    ut_file_close (file, expr, "r");
    return 0;
  }

  return status;
}
