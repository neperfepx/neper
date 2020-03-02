/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_gen_.h"

int
neut_mtess_tess_poly_mid (struct MTESS MTess, struct TESS Tess, int poly,
                          char **pid)
{
  int i, status, domqty = Tess.Level + 1;
  int **doms = NULL;

  if (Tess.Level == 0)
  {
    ut_string_string ("none", pid);
    status = -1;
  }

  else
  {
    doms = ut_alloc_2d_int (domqty, 2);

    ut_free_1d_char (pid);
    (*pid) = ut_alloc_1d_char (1000);

    neut_mtess_tess_doms (MTess, Tess, doms);

    for (i = 2; i < domqty; i++)
      sprintf (*pid, "%s%s%d", *pid, i == 2 ? "" : "::", doms[i][1]);
    sprintf (*pid, "%s%s%d", *pid, strlen (*pid) == 0 ? "" : "::", poly);

    (*pid) = ut_realloc_1d_char ((*pid), strlen (*pid) + 1);

    status = 0;

    ut_free_2d_int (&doms, domqty);
  }

  return status;
}
