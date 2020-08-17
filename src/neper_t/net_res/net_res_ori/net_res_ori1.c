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
  int partqty;
  char **parts = NULL;
  char *filename = ut_alloc_1d_char (strlen (In.orif) + 10);

  ut_list_break (In.oriformat, NEUT_SEP_NODEP, &parts, &partqty);

  for (i = 0; i < partqty; i++)
  {
    if (partqty == 1)
      sprintf (filename, "%s", In.orif);
    else
      sprintf (filename, "%s-%s", In.orif, parts[i]);

    file = ut_file_open (filename, "w");

    status = neut_ori_expr_desconv (In.orides, &des, &conv);
    if (status)
      ut_print_exprbug (In.orides);

    if (!strcmp (parts[i], "plain"))
      neut_ori_fprintf (file, Tess.CellOriDes, Tess.CellOri + 1, Tess.CellQty, NULL);

    else if (!strcmp (parts[i], "geof"))
    {
      char *cell = NULL;
      double *e = ol_e_alloc ();

      if (Tess.Dim >= 2)
        neut_tess_cell (Tess, &cell);
      else
        ut_string_string ("poly", &cell);

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

    ut_file_close (file, filename, "w");
  }

  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);
  ut_free_2d_char (&parts, partqty);
  ut_free_1d_char (&filename);

  return;
}
