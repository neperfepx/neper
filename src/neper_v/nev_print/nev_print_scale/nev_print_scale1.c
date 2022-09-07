/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_scale_.h"

void
nev_print_scale (char *basename, struct PRINT Print, struct SIM Sim,
                 struct DATA *TessData, struct DATA *pTesrData, struct DATA *pNodeData,
                 struct DATA *MeshData, int PointQty, struct DATA *PointData)

{
  int i, imageheight;
  FILE *file = NULL;
  char *filename2 = ut_alloc_1d_char (10000);
  char *outdir = NULL;
  char *suffix = ut_alloc_1d_char (10);

  neut_print_outdir (Print, Sim, "png", &outdir);
  if (strcmp (outdir, "."))
    ut_sys_mkdir (outdir);

  neut_print_imagesize (Print, NULL, &imageheight);

  ut_print_message (0, 1, "Printing scale...\n");

  for (i = 0; i <= 3; i++)
    if (!ut_string_strcmp (MeshData[i].ColDataType, "real"))
    {
      ut_print_message (0, 2, "Printing scale for dimension %d...\n", i);

      sprintf (filename2, "%s-scale%dd", basename, i);
      ut_file_dir_basename_extension_filename (outdir, filename2, "pov", &filename2);

      file = ut_file_open (filename2, "w");
      nev_print_png_scale (file, MeshData[i].ColScheme, MeshData[i].Scale,
                       MeshData[i].ScaleTitle);
      ut_file_close (file, filename2, "w");

      if (ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "png"))
        nev_print_png_convert (Print.povray, filename2, 0.3 * imageheight,
                           imageheight, Print.povrayantialiasing, 3);

      if (!ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov")
          && !ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov:objects"))
        remove (filename2);
    }

  if (!ut_string_strcmp ((*pNodeData).ColDataType, "real"))
  {
    ut_print_message (0, 2, "Printing scale for points...\n", i);

    sprintf (filename2, "%s-scalen", basename);
    ut_file_dir_basename_extension_filename (outdir, filename2, "pov", &filename2);

    file = ut_file_open (filename2, "w");
    nev_print_png_scale (file, (*pNodeData).ColScheme, (*pNodeData).Scale,
                     (*pNodeData).ScaleTitle);
    ut_file_close (file, filename2, "w");

    if (ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "png"))
      nev_print_png_convert (Print.povray, filename2, 0.3 * imageheight,
                         imageheight, Print.povrayantialiasing, 3);

    if (!ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov")
        && !ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov:objects"))
      remove (filename2);
  }

  for (i = 0; i <= 5; i++)
    if (!ut_string_strcmp (TessData[i].ColDataType, "real"))
    {
      ut_print_message (0, 2, "Printing scale for dim %d...\n", i);

      if (i < 4)
        sprintf (suffix, "%d", i);
      else if (i == 4)
        sprintf (suffix, "s");
      else if (i == 5)
        sprintf (suffix, "c");
      else
        abort ();

      sprintf (filename2, "%s-scale%s", basename, suffix);
      ut_file_dir_basename_extension_filename (outdir, filename2, "pov", &filename2);

      file = ut_file_open (filename2, "w");
      nev_print_png_scale (file, TessData[i].ColScheme, TessData[i].Scale,
                       TessData[i].ScaleTitle);
      ut_file_close (file, filename2, "w");

      if (ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "png"))
        nev_print_png_convert (Print.povray, filename2, 0.3 * imageheight,
                           imageheight, Print.povrayantialiasing, 3);

      if (!ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov")
          && !ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov:objects"))
        remove (filename2);
    }

  if (!ut_string_strcmp ((*pTesrData).ColDataType, "real"))
  {
    ut_print_message (0, 2, "Printing scale...\n");

    sprintf (filename2, "%s-scale", basename);
    ut_file_dir_basename_extension_filename (outdir, filename2, "pov", &filename2);

    file = ut_file_open (filename2, "w");
    nev_print_png_scale (file, (*pTesrData).ColScheme, (*pTesrData).Scale,
                         (*pTesrData).ScaleTitle);
    ut_file_close (file, filename2, "w");

    if (ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "png"))
      nev_print_png_convert (Print.povray, filename2, 0.3 * imageheight,
                         imageheight, Print.povrayantialiasing, 3);

    if (!ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov")
        && !ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov:objects"))
      remove (filename2);
  }

  for (i = 0; i < PointQty; i++)
    if (!ut_string_strcmp (PointData[0].ColDataType, "real"))
    {
      ut_print_message (0, 2, "Printing scale for points...\n", i);

      sprintf (filename2, "%s-scalep", basename);
      ut_file_dir_basename_extension_filename (outdir, filename2, "pov", &filename2);

      file = ut_file_open (filename2, "w");
      nev_print_png_scale (file, PointData[0].ColScheme, PointData[0].Scale,
                       PointData[0].ScaleTitle);
      ut_file_close (file, filename2, "w");

      if (ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "png"))
        nev_print_png_convert (Print.povray, filename2, 0.3 * imageheight,
                           imageheight, Print.povrayantialiasing, 3);

      if (!ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov")
          && !ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "pov:objects"))
        remove (filename2);
    }

  ut_free_1d_char (&filename2);
  ut_free_1d_char (&outdir);
  ut_free_1d_char (&suffix);

  return;
}
