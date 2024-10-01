/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_.h"

void
nev_print_png (struct IN_V In, char *basename, struct PRINT Print, struct SIM Sim,
               struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
               struct DATA *TesrData, struct NODES Nodes, struct MESH *Mesh,
               int SQty, struct NODES *SNodes, struct MESH **SMesh,
               struct DATA *pNodeData, struct DATA **MeshData,
               struct DATA *pCsysData, struct POINT *Points, int PointQty,
               struct DATA *PointData, struct DATA *SNodeData,
               struct DATA ***SMeshData, int **SElt2dElt3d)
{
  int i, imageheight, imagewidth;
  FILE *file = NULL;
  char *filename = NULL, *filename2 = ut_alloc_1d_char (10000);
  char *outdir = NULL;
  char *suffix = ut_alloc_1d_char (10);

  neut_print_outdir (In.outdir, Sim, "png", &outdir);
  if (strcmp (outdir, "."))
    ut_sys_mkdir (outdir);

  neut_print_imagesize (In.imagesize, &imagewidth, &imageheight);

  ut_file_dir_basename_extension_filename (outdir, basename, "pov", &filename);

  ut_print_message (0, 1, "Printing image...\n");
  file = ut_file_open (filename, ut_list_testelt (In.imageformat,
                       NEUT_SEP_NODEP, "pov") ? "w" : "W");

  nev_print_png_header (In, Print, file);

  if (Print.showcsys == 1)
    nev_print_png_csys (file, *pCsysData, Print);

  for (i = 0; i < PointQty; i++)
    if (Print.showpoint && Print.showpoint[i] && ut_array_1d_int_sum (Print.showpoint[i] + 1, Points[i].Qty))
    {
      ut_print_message (0, 2, "Printing %s...\n", Points[i].Name);
      nev_print_png_point (file, Points[i], PointData[i], Print, Print.showpoint[i]);
    }

  // tessellation ------------------------------------------------------
  if (Print.showtess == 1)
  {
    ut_print_message (0, 2, "Printing tessellation...\n");
    nev_print_png_tess (file, Print, Tess, TessData);
  }

  // raster tessellation -----------------------------------------------
  if (Print.showtesr == 1)
  {
    ut_print_message (0, 2, "Printing raster tessellation...\n");
    nev_print_png_tesr (file, Print, Tesr, TesrData);
  }

  // mesh --------------------------------------------------------------
  if (Print.showmesh == 1)
  {
    ut_print_message (0, 2, "Printing mesh...\n");
    nev_print_png_mesh (file, Print, Tess, Nodes, Mesh, *pNodeData, MeshData);
  }

  if (SQty > 0 && Print.showslice)
  {
    ut_print_message (0, 2, "Printing mesh slice%s...\n", SQty > 1? "s" : "");

    for (i = 0; i < SQty; i++)
    {
      int j, *showelt2d = ut_alloc_1d_int (SMesh[i][2].EltQty + 1);

      if (Print.showelt3d[0] != -1)
        for (j = 1; j <= SMesh[i][2].EltQty; j++)
          showelt2d[j] = Print.showelt3d[SElt2dElt3d[i][j]];
      else
        ut_array_1d_int_set (showelt2d + 1, SMesh[i][2].EltQty, 1);

      if (ut_string_strcmp (SMeshData[i][2][0].ColDataType, "from_nodes"))
        nev_print_png_mesh2d (file, SNodes[i], SMesh[i][2], showelt2d,
                          SMeshData[i][2][0].Col, NULL, "elt", Print);
      else
        nev_print_png_mesh2d (file, SNodes[i], SMesh[i][2], showelt2d,
                              SNodeData[i].Col, NULL, "node", Print);

      if (Print.showsliceelt1d)
      {
        double *Rad = ut_alloc_1d (SMesh[i][1].EltQty + 1);
        ut_array_1d_set (Rad + 1, SMesh[i][1].EltQty, MeshData[1][0].Rad[1]);
        int **Col = ut_alloc_2d_int (SMesh[i][1].EltQty + 1, 3);
        for (j = 1; j <= SMesh[i][1].EltQty; j++)
          ut_array_1d_int_memcpy (MeshData[1][0].Col[1], 3, Col[j]);

        nev_print_png_mesh1d (file, SNodes[i], SMesh[i][1], NULL,
                              Col, Rad, Print);
        ut_free_1d (&Rad);
        ut_free_2d_int (&Col, SMesh[i][1].EltQty + 1);
      }

      ut_free_1d_int (&showelt2d);
    }
  }

  nev_print_png_foot (In, file);
  ut_file_close (file, filename, ut_list_testelt (In.imageformat,
                 NEUT_SEP_NODEP, "pov") ? "w" : "W");

  if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
    nev_print_png_convert (In.povray, filename, imagewidth, imageheight,
                       In.povrayantialiasing, 2);

  if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov") == 0
      && ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects") == 0)
    remove (filename);

  ut_free_1d_char (&filename);
  ut_free_1d_char (&filename2);
  ut_free_1d_char (&outdir);
  ut_free_1d_char (&suffix);

  return;
}
