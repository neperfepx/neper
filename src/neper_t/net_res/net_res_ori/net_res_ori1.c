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
    neut_ori_fprintf (file, Tess.CellOriDes, Tess.CellOri + 1, Tess.CellQty);

  else if (strcmp (In.oriformat, "fepx:legacy") == 0)
  {
    fprintf (file, "grain-orientations\n%d\n", Tess.CellQty);
    neut_ori_fprintf (file, "ek:active", Tess.CellOri + 1, Tess.CellQty);
    fprintf (file, "EOF\n");
  }

  else if (strcmp (In.oriformat, "fepx") == 0)
  {
    int status = neut_ori_expr_desconv_fepx (Tess.CellOriDes, &des, &conv);
    if (status)
      ut_print_exprbug (Tess.CellOriDes);

    fprintf (file, "orientation_parameterization %s\n", des);
    fprintf (file, "orientation_convention %s\n", conv);

    neut_ori_fprintf (file, Tess.CellOriDes, Tess.CellOri + 1, Tess.CellQty);
  }

  else if (strcmp (In.oriformat, "geof") == 0)
  {
    double *e = ol_e_alloc ();
    for (i = 1; i <= Tess.CellQty; i++)
    {
      fprintf (file, "**elset poly%d  *rotation ", i);
      ol_q_e (Tess.CellOri[i], e);
      ol_e_e (e, e);
      for (j = 0; j < 3; j++)
        fprintf (file, "%17.12f ", e[j]);
      fprintf (file, "\n");
    }
    ol_e_free (e);
  }

  ut_file_close (file, In.orif, "w");

  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);

  return;
}
