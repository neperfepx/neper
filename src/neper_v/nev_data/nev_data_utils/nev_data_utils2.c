/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_utils_.h"

void
nev_data_fscanf_ori_file (char *value, int qty,
                          double ***pColData, char **pColDataType)
{
  int i, qty0 = ol_des_size (*pColDataType + 3);
  if (qty0 == -1)
    ut_print_message (2, 2, "Failed to process expression `%s'.\n", *pColDataType + 3);

  double *tmpd = ut_alloc_1d (4);
  int *tmpi = ut_alloc_1d_int (6);
  double **tmpdd = ut_alloc_2d (3, 3);
  FILE *file = ut_file_open (value, "r");

  ut_file_nbwords_testwmessage (value, qty * qty0);

  for (i = 1; i <= qty; i++)
    if (!strcmp (*pColDataType + 3, "e"))
    {
      ol_e_fscanf (file, tmpd);
      ol_e_q (tmpd, (*pColData)[i]);
    }
    else if (!strcmp (*pColDataType + 3, "ek"))
    {
      ol_e_fscanf (file, tmpd);
      ol_ek_e (tmpd, tmpd);
      ol_e_q (tmpd, (*pColData)[i]);
    }
    else if (!strcmp (*pColDataType + 3, "er"))
    {
      ol_e_fscanf (file, tmpd);
      ol_er_e (tmpd, tmpd);
      ol_e_q (tmpd, (*pColData)[i]);
    }
    else if (!strcmp (*pColDataType + 3, "q"))
      ol_q_fscanf (file, (*pColData)[i]);
    else if (!strcmp (*pColDataType + 3, "R"))
    {
      ol_R_fscanf (file, tmpd);
      ol_R_q (tmpd, (*pColData)[i]);
    }
    else if (!strcmp (*pColDataType + 3, "m"))
    {
      ol_m_fscanf (file, tmpi);
      ol_m_q (tmpi, (*pColData)[i]);
    }
    else if (!strcmp (*pColDataType + 3, "g"))
    {
      ol_g_fscanf (file, tmpdd);
      ol_g_q (tmpdd, (*pColData)[i]);
    }
    else
      ut_error_reportbug ();

  ut_free_1d (tmpd);
  ut_free_1d_int (tmpi);
  ut_free_2d (tmpdd, 3);

  return;
}
