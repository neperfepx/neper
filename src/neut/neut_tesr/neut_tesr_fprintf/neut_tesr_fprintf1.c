/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_fprintf_.h"

void
neut_tesr_fprintf (FILE * file, char *format, struct TESR Tesr)
{
  char *format2 = ut_alloc_1d_char (100);

  if (format == NULL || strlen (format) == 0 || !strcmp (format, "default"))
  {
    if (Tesr.CellQty < 256)
      strcpy (format2, "binary8");
    else if (Tesr.CellQty < 65536)
      strcpy (format2, "binary16");
    else
      strcpy (format2, "binary32");
  }
  else
    strcpy (format2, format);

  neut_tesr_fprintf_head (file, format2, Tesr);
  neut_tesr_fprintf_cell (file, Tesr);
  neut_tesr_fprintf_data (file, format2, Tesr);
  if (Tesr.VoxOri)
    neut_tesr_fprintf_oridata (file, "q", format, Tesr);
  neut_tesr_fprintf_foot (file);

  ut_free_1d_char (format2);

  return;
}

void
neut_tesr_name_fprintf_raw (char *tesr, char *raw, char *format,
			    struct TESR Tesr)
{
  char *format2 = ut_alloc_1d_char (100);
  FILE *fp1 = NULL;

  fp1 = ut_file_open (tesr, "w");

  if (format == NULL || strlen (format) == 0 || !strcmp (format, "default"))
  {
    if (Tesr.CellQty < 256)
      strcpy (format2, "binary8");
    else if (Tesr.CellQty < 65536)
      strcpy (format2, "binary16");
    else
      strcpy (format2, "binary32");
  }
  else
    strcpy (format2, format);

  neut_tesr_fprintf_head (fp1, format2, Tesr);
  neut_tesr_fprintf_cell (fp1, Tesr);
  neut_tesr_fprintf_data_raw (fp1, raw, format2, Tesr);
  neut_tesr_fprintf_foot (fp1);

  ut_file_close (fp1, tesr, "w");

  ut_free_1d_char (format2);

  return;
}

void
neut_tesr_fprintf_ami (FILE * file, char *format, struct TESR Tesr)
{
  char *format_ami = NULL;
  char *format2 = ut_alloc_1d_char (100);

  // .vtk files are always written in big endians.  So, setting format2 appropriately below.

  if (!strcmp (format, "binary8") || (!strcmp (format, "ascii") && Tesr.CellQty < pow (2, 8) - 1))
  {
    ut_string_string ("unsigned_char", &format_ami);
    ut_string_string ("binary8", &format2);
  }
  else if (!strncmp (format, "binary16", 8) || (!strcmp (format, "ascii") && Tesr.CellQty < pow (2, 16) - 1))
  {
    ut_string_string ("short", &format_ami);
    ut_string_string (!ut_sys_endian ()? "binary16_big" : "binary16", &format2);
  }
  else if (!strncmp (format, "binary32", 8) || (!strcmp (format, "ascii") && Tesr.CellQty < pow (2, 32) - 1))
  {
    ut_string_string ("int", &format_ami);
    ut_string_string (!ut_sys_endian ()? "binary32_big" : "binary32", &format2);
  }
  else
    abort ();

  if (!strcmp (format, "ascii"))
    ut_print_message (1, 3, "Using type `%s' (not `ascii')...\n", format_ami);

  fprintf (file, "# vtk DataFile Version 4.5\n");
  fprintf (file, "Materiau\n");
  fprintf (file, "BINARY\n");
  fprintf (file, "DATASET STRUCTURED_POINTS\n");
  fprintf (file, "DIMENSIONS    %d   %d   %d\n", Tesr.size[0] + 1, Tesr.size[1] + 1, Tesr.size[2] + 1);
  fprintf (file, "ORIGIN    %.12f %.12f %.12f\n", Tesr.Origin[0], Tesr.Origin[1], Tesr.Origin[2]);
  fprintf (file, "SPACING    %.12f    %.12f   %.12f\n", Tesr.vsize[0], Tesr.vsize[1], Tesr.vsize[2]);
  fprintf (file, "CELL_DATA   %d\n", ut_array_1d_int_prod (Tesr.size, 3));
  fprintf (file, "SCALARS MaterialId %s\n", format_ami);
  fprintf (file, "LOOKUP_TABLE default\n");

  neut_tesr_fprintf_data_noheader (file, format2, Tesr, Tesr.CellId);

  ut_free_1d_char (format_ami);
  ut_free_1d_char (format2);

  return;
}
