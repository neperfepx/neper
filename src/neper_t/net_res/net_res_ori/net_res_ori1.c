/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_res_ori_.h"

void
net_res_ori (struct IN_T In, struct TESS Tess)
{
  FILE *file = NULL;
  int i, j, status;
  char *des = NULL, *conv = NULL;

  file = ut_file_open (In.orif, "w");

  status = neut_ori_expr_desconv (In.orides, &des, &conv);
  if (status)
    ut_print_exprbug (In.orides);

  if (strcmp (In.oriformat, "plain") == 0)
    neut_ori_fprintf (file, Tess.CellOriDes, Tess.CellOri + 1, Tess.CellQty, NULL);

  else if (strcmp (In.oriformat, "geof") == 0)
  {
    char *cell = NULL;
    double *e = ol_e_alloc ();

    neut_tess_cell (Tess, &cell);
    for (i = 1; i <= Tess.CellQty; i++)
    {
      fprintf (file, "**elset %s%d  *rotation ", cell, i);
      ol_q_e (Tess.CellOri[i], e);
      ol_e_e (e, e);
      for (j = 0; j < 3; j++)
        fprintf (file, "%17.12f ", e[j]);
      fprintf (file, "\n");
    }
    ol_e_free (e);

    ut_free_1d_char (&cell);
  }

  ut_file_close (file, In.orif, "w");

  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);

  return;
}
