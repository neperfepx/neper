/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_fprintf_.h"

void
neut_tesr_fprintf_head (FILE * file, struct TESR Tesr)
{
  int i, origin = 0;

  fprintf (file, "***tesr\n");
  fprintf (file, " **format\n");
  fprintf (file, "   2.1\n");
  fprintf (file, " **general\n");
  fprintf (file, "   %d\n", Tesr.Dim);
  fprintf (file, "   ");
  ut_array_1d_int_fprintf (file, Tesr.size, Tesr.Dim, "%d");
  fprintf (file, "   ");
  ut_array_1d_fprintf (file, Tesr.vsize, Tesr.Dim, REAL_PRINT_FORMAT);

  for (i = 0; i < Tesr.Dim; i++)
    if (Tesr.Origin[i] != 0)
    {
      origin = 1;
      break;
    }

  if (origin)
  {
    fprintf (file, "  *origin\n");
    fprintf (file, "   ");
    ut_array_1d_fprintf (file, Tesr.Origin, Tesr.Dim, REAL_PRINT_FORMAT);
  }

  return;
}

void
neut_tesr_fprintf_data (FILE * file, char *format, struct TESR Tesr)
{
  if (!Tesr.CellId)
    return;

  fprintf (file, " **data\n");
  fprintf (file, "   %s\n", format);
  neut_tesr_fprintf_data_noheader (file, format, NULL, Tesr, NULL);

  return;
}

void
neut_tesr_fprintf_oridata (FILE * file, char *format,
                           struct TESR Tesr)
{
  fprintf (file, " **oridata\n");
  fprintf (file, "   %s\n", Tesr.CellOriDes);
  fprintf (file, "   %s\n", !strcmp (format, "ascii") ? "ascii" : "binary16");
  neut_tesr_fprintf_oridata_noheader (file, format, Tesr);
  if (strcmp (format, "ascii"))
    fprintf (file, "\n");

  return;
}

void
neut_tesr_fprintf_oridef (FILE * file, char *format, struct TESR Tesr)
{
  fprintf (file, " **oridef\n");
  fprintf (file, "   %s\n", format);
  neut_tesr_fprintf_oridef_noheader (file, format, NULL, Tesr);

  return;
}

void
neut_tesr_fprintf_cell (FILE * file, struct TESR Tesr)
{
  int i, j, count = 0;

  if (Tesr.CellQty)
  {
    double *e = ol_e_alloc ();

    fprintf (file, " **cell\n");
    fprintf (file, "   %d\n", Tesr.CellQty);

    if (Tesr.CellId)
    {
      fprintf (file, "  *id\n   ");
      for (i = 1; i <= Tesr.CellQty; i++)
        ut_print_wnc_int (file, Tesr.CellId[i], &count, 72);
      fprintf (file, "\n");
    }

    if (Tesr.SeedCoo && Tesr.SeedWeight)
    {
      fprintf (file, "  *seed\n");
      for (i = 1; i <= Tesr.CellQty; i++)
      {
        fprintf (file, " %3d ", i);
        for (j = 0; j < Tesr.Dim; j++)
          fprintf (file, REAL_PRINT_FORMAT " ", Tesr.SeedCoo[i][j]);
        fprintf (file, REAL_PRINT_FORMAT "\n", Tesr.SeedWeight[i]);
      }
    }

    if (Tesr.CellOri)
    {
      fprintf (file, "  *ori\n");
      fprintf (file, "   %s\n", Tesr.CellOriDes);
      neut_ori_fprintf (file, Tesr.CellOriDes, "ascii", Tesr.CellOri + 1, NULL, NULL, Tesr.CellQty, NULL);
    }

    if (neut_tesr_hascellorispread (Tesr))
    {
      fprintf (file, "  *orispread\n");
      for (i = 1; i <= Tesr.CellQty; i++)
        fprintf (file, "   %s\n", Tesr.CellOriDistrib[i]);
    }

    if (Tesr.CellGroup)
    {
      fprintf (file, "  *group\n   ");
      for (i = 1; i <= Tesr.CellQty; i++)
        ut_print_wnc_int (file, Tesr.CellGroup[i], &count, 72);
      fprintf (file, "\n");
    }

    if (Tesr.CellCrySym)
    {
      fprintf (file, "  *crysym\n");
      fprintf (file, "   %s\n", Tesr.CellCrySym);
    }

    ol_e_free (e);
  }

  return;
}

void
neut_tesr_fprintf_data_raw (FILE * file, char *rawname, char *format,
                            char *wildcard, struct TESR Tesr)
{
  fprintf (file, " **data\n");
  fprintf (file, "   %s\n", format);
  fprintf (file, "  *file %s\n", rawname);

  FILE *file2 = ut_file_open (rawname, "w");
  neut_tesr_fprintf_data_noheader (file2, format, wildcard, Tesr, NULL);
  ut_file_close (file2, rawname, "w");

  return;
}

void
neut_tesr_fprintf_oridata_raw (FILE * file, char *rawname,
                               char *format, struct TESR Tesr)
{
  fprintf (file, " **oridata\n");
  fprintf (file, "   %s\n", Tesr.CellOriDes);
  fprintf (file, "  *file %s", rawname);

  FILE *file2 = ut_file_open (rawname, "w");
  neut_tesr_fprintf_oridata_noheader (file2, format, Tesr);
  ut_file_close (file2, rawname, "w");

  return;
}

void
neut_tesr_fprintf_oridef_noheader (FILE * file, char *format, char *wildcard,
                                   struct TESR Tesr)
{
  int i, j, k, count;
  int *size = ut_alloc_1d_int (3);
  // char* progress = ut_alloc_1d_char (10);

  ut_array_1d_int_set (size, 3, 1);
  ut_array_1d_int_memcpy (Tesr.size, Tesr.Dim, size);

  if (!strcmp (format, "ascii"))
  {
    count = 0;
    for (k = 1; k <= size[2]; k++)
      for (j = 1; j <= size[1]; j++)
        for (i = 1; i <= size[0]; i++)
          ut_print_wnc_int (file, Tesr.VoxOriDef[i][j][k], &count, 72);
  }

  else if (!strncmp (format, "binary", 6))
  {
    unsigned char data;

    for (k = 1; k <= size[2]; k++)
      for (j = 1; j <= size[1]; j++)
        for (i = 1; i <= size[0]; i++)
        {
          data = Tesr.VoxOriDef[i][j][k];
          fwrite (&data, sizeof (unsigned char), 1, file);
        }
  }

  else
    abort ();

  if (ut_string_strcmp (wildcard, "nonl"))
    fprintf (file, "\n");

  // ut_free_1d_char (&progress);
  ut_free_1d_int (&size);

  return;
}
void
neut_tesr_fprintf_data_noheader (FILE * file, char *format, char *wildcard,
                                 struct TESR Tesr, int *CellId)
{
  int i, j, k, count;
  int *size = ut_alloc_1d_int (3);
  // char* progress = ut_alloc_1d_char (10);

  ut_array_1d_int_set (size, 3, 1);
  ut_array_1d_int_memcpy (Tesr.size, Tesr.Dim, size);

  if (!strcmp (format, "ascii"))
  {
    count = 0;
    for (k = 1; k <= size[2]; k++)
      for (j = 1; j <= size[1]; j++)
        for (i = 1; i <= size[0]; i++)
          ut_print_wnc_int (file,
                            CellId ? CellId[Tesr.VoxCell[i][j][k]] : Tesr.
                            VoxCell[i][j][k], &count, 72);
  }

  else if (!strcmp (format, "binary8"))
  {
    unsigned char data;

    for (k = 1; k <= size[2]; k++)
      for (j = 1; j <= size[1]; j++)
        for (i = 1; i <= size[0]; i++)
        {
          data =
            CellId ? CellId[Tesr.VoxCell[i][j][k]] : Tesr.VoxCell[i][j][k];
          fwrite (&data, sizeof (unsigned char), 1, file);
        }
  }

  else if ((!strcmp (format, "binary16") && !ut_sys_endian ())
           || (!strcmp (format, "binary16_big") && ut_sys_endian ()))
  {
    short data;

    for (k = 1; k <= size[2]; k++)
      for (j = 1; j <= size[1]; j++)
        for (i = 1; i <= size[0]; i++)
        {
          data =
            CellId ? CellId[Tesr.VoxCell[i][j][k]] : Tesr.VoxCell[i][j][k];
          fwrite (&data, sizeof (short), 1, file);
        }
  }

  else if ((!strcmp (format, "binary16") && ut_sys_endian ())
           || (!strcmp (format, "binary16_big") && !ut_sys_endian ()))
  {
    short data;
    short val2;
    char *pval = NULL;
    char *pval2 = (char *) &val2;

    for (k = 1; k <= size[2]; k++)
      for (j = 1; j <= size[1]; j++)
        for (i = 1; i <= size[0]; i++)
        {
          data =
            CellId ? CellId[Tesr.VoxCell[i][j][k]] : Tesr.VoxCell[i][j][k];
          pval = (char *) &data;
          pval2[1] = pval[0];
          pval2[0] = pval[1];

          fwrite (pval2, sizeof (short), 1, file);
        }
  }

  else if ((!strcmp (format, "binary32") && !ut_sys_endian ())
           || (!strcmp (format, "binary32_big") && ut_sys_endian ()))
  {
    int data;

    for (k = 1; k <= size[2]; k++)
      for (j = 1; j <= size[1]; j++)
        for (i = 1; i <= size[0]; i++)
        {
          data =
            CellId ? CellId[Tesr.VoxCell[i][j][k]] : Tesr.VoxCell[i][j][k];
          fwrite (&data, sizeof (int), 1, file);
        }
  }

  else if ((!strcmp (format, "binary32") && ut_sys_endian ())
           || (!strcmp (format, "binary32_big") && !ut_sys_endian ()))
  {
    int data, val2 = 0;
    char *pval = NULL;
    char *pval2 = (char *) &val2;

    for (k = 1; k <= size[2]; k++)
      for (j = 1; j <= size[1]; j++)
        for (i = 1; i <= size[0]; i++)
        {
          data =
            CellId ? CellId[Tesr.VoxCell[i][j][k]] : Tesr.VoxCell[i][j][k];
          pval = (char *) &data;
          pval2[3] = pval[0];
          pval2[2] = pval[1];
          pval2[1] = pval[2];
          pval2[0] = pval[3];
          fwrite (pval2, sizeof (int), 1, file);
        }
  }

  else
    abort ();

  if (ut_string_strcmp (wildcard, "nonl"))
    fprintf (file, "\n");

  // ut_free_1d_char (&progress);
  ut_free_1d_int (&size);

  return;
}

void
neut_tesr_fprintf_oridata_noheader (FILE * file, char *format,
                                    struct TESR Tesr)
{
  int i, j, k;

  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
        neut_ori_fprintf (file, Tesr.CellOriDes, format, &(Tesr.VoxOri[i][j][k]), NULL, NULL, 1, NULL);

  return;
}

void
neut_tesr_fprintf_foot (FILE * file)
{
  fprintf (file, "***end\n");

  return;
}
