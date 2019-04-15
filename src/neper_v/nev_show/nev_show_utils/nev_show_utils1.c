/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_utils_.h"

int
nev_show_genexpr (char *expr, int *val, int valqty)
{
  int id;
  FILE *file = NULL;

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

  else if (ut_string_filename (expr))
  {
    file = ut_file_open (expr, "r");
    while (fscanf (file, "%d", &id) != EOF)
      val[id] = 1;
    ut_file_close (file, expr, "r");
    return 0;
  }

  return -1;
}
