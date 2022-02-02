/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_utils_.h"

int
nev_show_genexpr (char *expr, int *val, int valqty)
{
  int status = -1;
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
    file = ut_file_open (expr, "r");
    ut_array_1d_int_fscanf (file, val + 1, valqty);
    ut_file_close (file, expr, "r");

    return 0;
  }

  return status;
}
