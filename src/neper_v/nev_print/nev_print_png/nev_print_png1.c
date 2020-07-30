/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_.h"

void
nev_print_png (char *basename, struct PRINT *pPrint, struct SIM Sim,
               struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
               struct DATA *pTesrData, struct NODES Nodes, struct MESH *Mesh,
               int SQty, struct NODES *SNodes, struct MESH *SMesh2D,
               struct DATA *pNodeData, struct DATA *MeshData,
               struct DATA *pCsysData, struct POINT Point,
               struct DATA *pPointData, struct DATA *SNodeData,
               struct DATA **SMeshData, int **SElt2dElt3d)
{
  int i, valqty, imageheight, imagewidth;
  FILE *file = NULL;
  char *filename = NULL, *filename2 = NULL;
  char **vals = NULL;
  char *outdir = NULL;

  neut_print_outdir (*pPrint, Sim, "png", &outdir);
  if (strcmp (outdir, "."))
    ut_sys_mkdir (outdir);

  ut_list_break ((*pPrint).imagesize, NEUT_SEP_DEP, &vals, &valqty);

  if (valqty != 2)
    ut_print_message (2, 2, "Expression `%s' could not be processed.\n",
                      (*pPrint).imagesize);
  ut_string_int (vals[0], &imagewidth);
  ut_string_int (vals[1], &imageheight);
  ut_free_2d_char (&vals, valqty);

  if (!strcmp (outdir, "."))
    filename = ut_string_addextension (basename, "pov");
  else
    filename = ut_string_paste4 (outdir, "/", basename, ".pov");

  ut_print_message (0, 1, "Printing image...\n");
  file = ut_file_open (filename, "w");
  nev_print_png_header (file, *pPrint);

  if ((*pPrint).showcsys == 1)
    nev_print_png_csys (file, *pCsysData, (*pPrint));

  if ((*pPrint).showpoint[0] > 0)
    nev_print_png_point (file, Point, *pPointData, (*pPrint));

  // tessellation ------------------------------------------------------
  if ((*pPrint).showtess == 1)
  {
    ut_print_message (0, 2, "Printing tessellation...\n");
    nev_print_png_tess (file, (*pPrint), Tess, TessData);
  }

  // raster tessellation -----------------------------------------------
  if ((*pPrint).showtesr == 1)
  {
    ut_print_message (0, 2, "Printing raster tessellation...\n");
    nev_print_png_tesr (file, *pPrint, Tesr, *pTesrData);
  }

  // mesh --------------------------------------------------------------
  if ((*pPrint).showmesh == 1)
  {
    ut_print_message (0, 2, "Printing mesh...\n");
    nev_print_png_mesh (file, *pPrint, Tess, Nodes, Mesh, *pNodeData, MeshData);
  }

  if (SQty > 0 && (*pPrint).showslice)
  {
    ut_print_message (0, 2, "Printing mesh slice%s...\n", SQty > 1? "s" : "");

    for (i = 0; i < SQty; i++)
    {
      int j, *showelt2d = ut_alloc_1d_int (SMesh2D[i].EltQty + 1);

      if ((*pPrint).showelt3d[0] != -1)
        for (j = 1; j <= SMesh2D[i].EltQty; j++)
          showelt2d[j] = (*pPrint).showelt3d[SElt2dElt3d[i][j]];
      else
        ut_array_1d_int_set (showelt2d + 1, SMesh2D[i].EltQty, 1);

      if (ut_string_strcmp (SMeshData[i][2].ColDataType, "from_nodes"))
        nev_print_png_mesh2d (file, SNodes[i], SMesh2D[i], showelt2d,
                          SMeshData[i][2].Col, "elt", (*pPrint).showshadow);
      else
        nev_print_png_mesh2d (file, SNodes[i], SMesh2D[i], showelt2d,
                              SNodeData[i].Col, "node", (*pPrint).showshadow);

      ut_free_1d_int (&showelt2d);
    }
  }

  nev_print_png_foot (file, *pPrint);
  ut_file_close (file, filename, "w");

  if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "png"))
    nev_print_png_pov2png ((*pPrint).povray, filename, imagewidth, imageheight,
                       (*pPrint).imageantialias, 2);

  if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov") == 0
      && ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov:objects") == 0)
    remove (filename);

  ut_print_message (0, 1, "Printing scale...\n");

  for (i = 0; i <= 3; i++)
    if (!ut_string_strcmp (MeshData[i].ColDataType, "real"))
    {
      ut_print_message (0, 2, "Printing scale for dimension %d...\n", i);

      filename2 = ut_alloc_1d_char (strlen (basename) + strlen (outdir) + 100);

      if (!strcmp (outdir, "."))
        sprintf (filename2, "%s-scale%dd.pov", basename, i);
      else
        sprintf (filename2, "%s/%s-scale%dd.pov", outdir, basename, i);

      file = ut_file_open (filename2, "w");
      nev_print_png_scale (file, MeshData[i].ColScheme, MeshData[i].Scale,
                       MeshData[i].ScaleTitle);
      ut_file_close (file, filename2, "w");

      if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "png"))
        nev_print_png_pov2png ((*pPrint).povray, filename2, 0.3 * imageheight,
                           imageheight, (*pPrint).imageantialias, 3);

      if (!ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov")
          && !ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov:objects"))
        remove (filename2);
    }

  if (!ut_string_strcmp ((*pNodeData).ColDataType, "real"))
  {
    ut_print_message (0, 2, "Printing scale for points...\n", i);

    if (!strcmp (outdir, "."))
      filename2 = ut_string_paste (basename, "-scalen.pov");
    else
      filename2 = ut_string_paste4 (outdir, "/", basename, "-scalen.pov");

    file = ut_file_open (filename2, "w");
    nev_print_png_scale (file, (*pNodeData).ColScheme, (*pNodeData).Scale,
                     (*pNodeData).ScaleTitle);
    ut_file_close (file, filename2, "w");

    if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "png"))
      nev_print_png_pov2png ((*pPrint).povray, filename2, 0.3 * imageheight,
                         imageheight, (*pPrint).imageantialias, 3);

    if (!ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov")
        && !ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov:objects"))
      remove (filename2);
  }

  for (i = 0; i <= 4; i++)
    if (!ut_string_strcmp (TessData[i].ColDataType, "real"))
    {
      ut_print_message (0, 2, "Printing scale for dim %d...\n", i);

      filename2 = ut_alloc_1d_char (strlen (basename) + strlen (outdir) + 100);

      if (!strcmp (outdir, "."))
        sprintf (filename2, "%s-scale%d.pov", basename, i);
      else
        sprintf (filename2, "%s/%s-scale%d.pov", outdir, basename, i);

      file = ut_file_open (filename2, "w");
      nev_print_png_scale (file, TessData[i].ColScheme, TessData[i].Scale,
                       TessData[i].ScaleTitle);
      ut_file_close (file, filename2, "w");

      if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "png"))
        nev_print_png_pov2png ((*pPrint).povray, filename2, 0.3 * imageheight,
                           imageheight, (*pPrint).imageantialias, 3);

      if (!ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov")
          && !ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov:objects"))
        remove (filename2);
    }

  if (!ut_string_strcmp ((*pTesrData).ColDataType, "real"))
  {
    ut_print_message (0, 2, "Printing scale...\n");

    if (!strcmp (outdir, "."))
      filename2 = ut_string_paste (basename, "-scale.pov");
    else
      filename2 = ut_string_paste4 (outdir, "/", basename, "-scale.pov");

    file = ut_file_open (filename2, "w");
    nev_print_png_scale (file, (*pTesrData).ColScheme, (*pTesrData).Scale,
                         (*pTesrData).ScaleTitle);
    ut_file_close (file, filename2, "w");

    if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "png"))
      nev_print_png_pov2png ((*pPrint).povray, filename2, 0.3 * imageheight,
                         imageheight, (*pPrint).imageantialias, 3);

    if (!ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov")
        && !ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov:objects"))
      remove (filename2);
  }

  if (!ut_string_strcmp ((*pPointData).ColDataType, "real"))
  {
    ut_print_message (0, 2, "Printing scale for points...\n", i);

    if (!strcmp (outdir, "."))
      filename2 = ut_string_paste (basename, "-scalep.pov");
    else
      filename2 = ut_string_paste4 (outdir, "/", basename, "-scalep.pov");

    file = ut_file_open (filename2, "w");
    nev_print_png_scale (file, (*pPointData).ColScheme, (*pPointData).Scale,
                     (*pPointData).ScaleTitle);
    ut_file_close (file, filename2, "w");

    if (ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "png"))
      nev_print_png_pov2png ((*pPrint).povray, filename2, 0.3 * imageheight,
                         imageheight, (*pPrint).imageantialias, 3);

    if (!ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov")
        && !ut_list_testelt ((*pPrint).format, NEUT_SEP_NODEP, "pov:objects"))
      remove (filename2);
  }

  ut_free_1d_char (&filename);
  ut_free_1d_char (&filename2);
  ut_free_1d_char (&outdir);

  return;
}
