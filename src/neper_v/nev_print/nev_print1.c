/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_.h"

void
nev_print (char **expargv, int *pi, struct PRINT Print,
	   struct TESS Tess, struct TESSDATA TessData,
	   struct TESR Tesr, struct TESRDATA TesrData,
	   struct NODES Nodes, struct MESH *Mesh,
	   struct NODEDATA NodeData, struct MESHDATA *MeshData,
	   int SQty, struct NODES *SNodes, struct MESH *SMesh2D,
	   struct NODEDATA *SNodeData,
	   struct MESHDATA **SMeshData,
	   int **SElt2dElt3d, struct CSYSDATA CsysData,
	   struct POINT Point, struct POINTDATA PointData)
{
  FILE *file = NULL;
  char *filename = ut_alloc_1d_char (strlen (expargv[(*pi) + 1]) + 10);
  char *filename2 = ut_alloc_1d_char (strlen (expargv[(*pi) + 1]) + 100);
  char *command = ut_alloc_1d_char (1000);
  int i;
  int imageheight, imagewidth;
  char **vals = NULL;
  int qty;

  ut_string_separate (Print.imagesize, NEUT_SEP_DEP, &vals, &qty);

  if (qty != 2)
    ut_print_message (2, 2, "Expression `%s' could not be processed.\n",
		      Print.imagesize);
  ut_string_int (vals[0], &imagewidth);
  ut_string_int (vals[1], &imageheight);
  ut_free_2d_char (vals, qty);

  (*pi)++;
  sprintf (filename, "%s.pov", expargv[(*pi)]);

  ut_print_message (0, 1, "Printing image...\n");
  file = ut_file_open (filename, "w");
  nev_print_header (file, Print);

  if (Print.showcsys == 1)
    nev_print_csys (file, CsysData, Print);

  if (Print.showpoint[0] > 0)
    nev_print_point (file, Point, PointData, Print);

  // tessellation ------------------------------------------------------
  if (Print.showtess == 1)
  {
    ut_print_message (0, 2, "Printing tessellation...\n");
    nev_print_tess (file, Print, Tess, TessData);
  }

  // raster tessellation -----------------------------------------------
  if (Print.showtesr == 1)
  {
    ut_print_message (0, 2, "Printing raster tessellation...\n");
    nev_print_tesr (file, Print, Tesr, TesrData);
  }

  // mesh --------------------------------------------------------------
  if (Print.showmesh == 1)
  {
    ut_print_message (0, 2, "Printing mesh...\n");
    nev_print_mesh (file, Print, Tess, Nodes, Mesh, NodeData, MeshData);
  }

  if (SQty > 0 && Print.showslice)
  {
    ut_print_message (0, 2, "Printing mesh %s...\n",
		      (SQty <= 1) ? "slice" : "slices");

    for (i = 0; i < SQty; i++)
    {
      int j, *showelt2d = ut_alloc_1d_int (SMesh2D[i].EltQty + 1);

      if (Print.showelt3d[0] != -1)
	for (j = 1; j <= SMesh2D[i].EltQty; j++)
	  showelt2d[j] = Print.showelt3d[SElt2dElt3d[i][j]];
      else
	ut_array_1d_int_set (showelt2d + 1, SMesh2D[i].EltQty, 1);

      if (!(SMeshData[i][2].ColDataType)
	  || strcmp (SMeshData[i][2].ColDataType, "from_nodes") != 0)
	nev_print_mesh2d (file, SNodes[i], SMesh2D[i], showelt2d,
			  SMeshData[i][2].Col, "elt", Print.showshadow);
      else
	nev_print_mesh2d (file, SNodes[i], SMesh2D[i], showelt2d,
			  SNodeData[i].Col, "node", Print.showshadow);

      ut_free_1d_int (showelt2d);
    }
  }

  nev_print_foot (file, Print);
  ut_file_close (file, filename, "w");

  if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "png"))
  {
    nev_print_pov2png (Print.povray, filename, imagewidth,
		       imageheight, Print.imageantialias, 2);
  }

  if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov") == 0
   && ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov:objects") == 0)
    remove (filename);

  ut_print_message (0, 1, "Printing scale...\n");
  char *ent = ut_alloc_1d_char (100);

  for (i = 0; i <= 3; i++)
    if (MeshData[i].ColDataType != NULL
        && !strcmp (MeshData[i].ColDataType, "scal"))
    {
      ut_print_message (0, 2, "Printing scale for dimension %d...\n", i);

      sprintf (filename2, "%s-scale%s.pov", expargv[(*pi)], ent);

      file = ut_file_open (filename2, "w");
      nev_print_scale (file, MeshData[i].ColScheme, MeshData[i].Scale,
                       MeshData[i].ScaleTitle);
      ut_file_close (file, filename2, "w");

      if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "png"))
        nev_print_pov2png (Print.povray, filename2, 0.3 * imageheight,
                           imageheight, Print.imageantialias, 3);

      if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov") == 0
       && ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov:objects") == 0)
        remove (filename2);
    }

  if (NodeData.ColDataType != NULL && !strcmp (NodeData.ColDataType, "scal"))
  {
    ut_print_message (0, 2, "Printing scale for points...\n", i);

    sprintf (filename2, "%s-scalen.pov", expargv[(*pi)]);

    file = ut_file_open (filename2, "w");
    nev_print_scale (file, NodeData.ColScheme, NodeData.Scale,
                     NodeData.ScaleTitle);
    ut_file_close (file, filename2, "w");

    if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "png"))
      nev_print_pov2png (Print.povray, filename2, 0.3 * imageheight,
                         imageheight, Print.imageantialias, 3);

    if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov") == 0
     && ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov:objects") == 0)
      remove (filename2);
  }

  for (i = 0; i <= 3; i++)
    if (TessData.ColDataType[i] != NULL
        && !strcmp (TessData.ColDataType[i], "scal"))
    {
      ut_print_message (0, 2, "Printing scale for dim %d...\n", i);

      sprintf (filename2, "%s-scale%d.pov", expargv[(*pi)], i);

      file = ut_file_open (filename2, "w");
      nev_print_scale (file, TessData.ColScheme[i], TessData.Scale[i],
                       TessData.ScaleTitle[i]);
      ut_file_close (file, filename2, "w");

      if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "png"))
        nev_print_pov2png (Print.povray, filename2, 0.3 * imageheight,
                           imageheight, Print.imageantialias, 3);

      if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov") == 0
       && ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov:objects") == 0)
        remove (filename2);
    }

  if (TesrData.ColDataType != NULL
      && !strcmp (TesrData.ColDataType, "scal"))
  {
    ut_print_message (0, 2, "Printing scale...\n");

    sprintf (filename2, "%s-scale.pov", expargv[(*pi)]);

    file = ut_file_open (filename2, "w");
    nev_print_scale (file, TesrData.ColScheme, TesrData.Scale,
                     TesrData.ScaleTitle);
    ut_file_close (file, filename2, "w");

    if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "png"))
      nev_print_pov2png (Print.povray, filename2, 0.3 * imageheight,
                         imageheight, Print.imageantialias, 3);

    if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov") == 0
     && ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov:objects") == 0)
      remove (filename2);
  }

  if (PointData.ColDataType != NULL
      && !strcmp (PointData.ColDataType, "scal"))
  {
    ut_print_message (0, 2, "Printing scale for points...\n", i);

    sprintf (filename2, "%s-scalep.pov", expargv[(*pi)]);

    file = ut_file_open (filename2, "w");
    nev_print_scale (file, PointData.ColScheme, PointData.Scale,
                     PointData.ScaleTitle);
    ut_file_close (file, filename2, "w");

    if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "png"))
      nev_print_pov2png (Print.povray, filename2, 0.3 * imageheight,
                         imageheight, Print.imageantialias, 3);

    if (ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov") == 0
     && ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov:objects") == 0)
      remove (filename2);
  }

  ut_free_1d_char (ent);
  ut_free_1d_char (command);
  ut_free_1d_char (filename);
  ut_free_1d_char (filename2);

  return;
}
