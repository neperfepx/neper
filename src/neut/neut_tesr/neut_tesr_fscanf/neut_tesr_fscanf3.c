/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_fscanf_.h"

void
neut_tesr_fscanf_data_default (struct TESR *pTesr, char *format, int readfromfile, FILE * file)
{
  int i, j, k;

  for (k = 1; k <= (*pTesr).size[2]; k++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
      {
        if (!strcmp (format, "ascii"))
        {
          int data;
          if (fscanf (file, "%d", &data) != 1)
            abort ();

          (*pTesr).VoxCell[i][j][k] = data;
        }
        else if (!strcmp (format, "binary8"))
        {
          unsigned char data;
          if (fread (&data, sizeof (unsigned char), 1, file) != 1)
            abort ();

          (*pTesr).VoxCell[i][j][k] = data;
        }
        else if ((!strcmp (format, "binary16") && !ut_sys_endian ())
                 || (!strcmp (format, "binary16_big") && ut_sys_endian ()))
        {
          short data;
          if (fread (&data, sizeof (short), 1, file) != 1)
            abort ();

          (*pTesr).VoxCell[i][j][k] = (int) data;
        }
        else if ((!strcmp (format, "binary16") && ut_sys_endian ())
                 || (!strcmp (format, "binary16_big") && !ut_sys_endian ()))
        {
          short data;
          short val2;
          char *pval = NULL;
          char *pval2 = (char *) &val2;

          if (fread (&data, sizeof (short), 1, file) != 1)
            abort ();

          pval = (char *) &data;
          pval2[1] = pval[0];
          pval2[0] = pval[1];

          (*pTesr).VoxCell[i][j][k] = (int) val2;
        }
        else if ((!strcmp (format, "binary32") && !ut_sys_endian ())
                 || (!strcmp (format, "binary32_big") && ut_sys_endian ()))
        {
          int data;
          if (fread (&data, sizeof (int), 1, file) != 1)
            abort ();

          (*pTesr).VoxCell[i][j][k] = data;
        }
        else if ((!strcmp (format, "binary32") && ut_sys_endian ())
                 || (!strcmp (format, "binary32_big") && !ut_sys_endian ()))
        {
          int val2 = 0;
          char *pval = NULL;
          char *pval2 = (char *) &val2;

          int data;

          if (fread (&data, sizeof (int), 1, file) != 1)
            abort ();

          pval = (char *) &data;
          pval2[3] = pval[0];
          pval2[2] = pval[1];
          pval2[1] = pval[2];
          pval2[0] = pval[3];

          (*pTesr).VoxCell[i][j][k] = (int) val2;
        }
        else
          ut_print_message (2, 2, "Unknown format `%s'.\n", format);
      }

  if (readfromfile && !ut_file_isendoffile (file))
    ut_print_message (2, 4, "File contains more data than needed.\n");

  return;
}

void
neut_tesr_fscanf_oridata_default (struct TESR *pTesr, char *desconv, char *oridataformat,
                                  int readfromfile, FILE * file)
{
  int i, j, k, id, qty = ut_array_1d_int_prod ((*pTesr).size, 3);
  char *des = NULL, *conv = NULL;
  double **q = ut_alloc_2d (qty, 4);

  neut_ori_expr_desconv (desconv, &des, &conv);

  neut_ori_fscanf (file, desconv, oridataformat, q, NULL, qty, NULL);

  if (readfromfile && !ut_file_isendoffile (file))
    ut_print_message (2, 4, "File contains more data than needed.\n");

  id = 0;

  if (!strcmp (conv, "active"))
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
          ut_array_1d_memcpy (q[id++], 4, (*pTesr).VoxOri[i][j][k]);

  if (!strcmp (conv, "passive"))
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
          ol_q_inverse (q[id++], (*pTesr).VoxOri[i][j][k]);

  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);
  ut_free_2d (&q, qty);

  return;
}

void
neut_tesr_fscanf_data_bounds (struct TESR *pTesr, int *bounds, char *format,
                              FILE * file)
{
  int i, j, k, ii, jj, kk;

  for (k = 1; k <= (*pTesr).size[2]; k++)
  {
    kk = k - bounds[4] + 1;

    for (j = 1; j <= (*pTesr).size[1]; j++)
    {
      jj = j - bounds[2] + 1;

      for (i = 1; i <= (*pTesr).size[0]; i++)
      {
        ii = i - bounds[0] + 1;

        if (!strcmp (format, "ascii"))
        {
          int data;
          if (fscanf (file, "%d", &data) != 1)
            abort ();

          if (k >= bounds[4] && k <= bounds[5] && j >= bounds[2]
              && j <= bounds[3] && i >= bounds[0] && i <= bounds[1])
            (*pTesr).VoxCell[ii][jj][kk] = data;
        }
        else if (!strcmp (format, "binary8"))
        {
          unsigned char data;
          if (fread (&data, sizeof (unsigned char), 1, file) != 1)
            abort ();

          if (k >= bounds[4] && k <= bounds[5] && j >= bounds[2]
              && j <= bounds[3] && i >= bounds[0] && i <= bounds[1])
            (*pTesr).VoxCell[ii][jj][kk] = data;
        }
        else if (!strcmp (format, "binary16"))
        {
          short data;
          if (fread (&data, sizeof (short), 1, file) != 1)
            abort ();

          if (k >= bounds[4] && k <= bounds[5] && j >= bounds[2]
              && j <= bounds[3] && i >= bounds[0] && i <= bounds[1])
            (*pTesr).VoxCell[ii][jj][kk] = (int) data;
        }
        else if (!strcmp (format, "binary16*"))
        {
          short data;
          short val2;
          char *pval = NULL;
          char *pval2 = (char *) &val2;

          if (fread (&data, sizeof (short), 1, file) != 1)
            abort ();

          pval = (char *) &data;
          pval2[1] = pval[0];
          pval2[0] = pval[1];

          if (k >= bounds[4] && k <= bounds[5] && j >= bounds[2]
              && j <= bounds[3] && i >= bounds[0] && i <= bounds[1])
            (*pTesr).VoxCell[ii][jj][kk] = (int) val2;
        }
        else if (!strcmp (format, "binary32"))
        {
          int data;
          if (fread (&data, sizeof (int), 1, file) != 1)
            abort ();

          if (k >= bounds[4] && k <= bounds[5] && j >= bounds[2]
              && j <= bounds[3] && i >= bounds[0] && i <= bounds[1])
            (*pTesr).VoxCell[ii][jj][kk] = data;
        }
        else if (!strcmp (format, "binary32*"))
        {
          int val2 = 0;
          char *pval = NULL;
          char *pval2 = (char *) &val2;

          int data;

          if (fread (&data, sizeof (int), 1, file) != 1)
            abort ();

          pval = (char *) &data;
          pval2[3] = pval[0];
          pval2[2] = pval[1];
          pval2[1] = pval[2];
          pval2[0] = pval[3];

          if (k >= bounds[4] && k <= bounds[5] && j >= bounds[2]
              && j <= bounds[3] && i >= bounds[0] && i <= bounds[1])
            (*pTesr).VoxCell[ii][jj][kk] = (int) val2;
        }
        else
          abort ();
      }
    }
  }

  return;
}

void
neut_tesr_fscanf_data_scale (struct TESR *pTesr, double *scale, char *format,
                             int readfromfile, FILE * file)
{
  neut_tesr_fscanf_data_default (pTesr, format, readfromfile, file);

  neut_tesr_rasterscale (pTesr, scale[0], scale[1], scale[2]);

  return;
}
