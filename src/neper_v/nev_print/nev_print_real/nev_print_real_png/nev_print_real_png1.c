/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_png_.h"

void
nev_print_real_png (struct IN_V In, char *basename, struct PRINT Print, struct SIM Sim,
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

  ut_file_dir_basename_extension_filename (outdir, basename, "pov", &filename);

  neut_print_imagesize (In.imagesize, &imagewidth, &imageheight);

  ut_print_message (0, 1, "Printing image...\n");
  file = ut_file_open (filename, ut_list_testelt (In.imageformat,
                       NEUT_SEP_NODEP, "pov") ? "w" : "W");

  neut_print_povheader (In.imageformat, Print, file);

  if (Print.showcsys == 1)
    nev_print_real_png_csys (file, *pCsysData, Print, NULL);

  for (i = 0; i < PointQty; i++)
    if (Print.showpoint && Print.showpoint[i] && ut_array_1d_int_sum (Print.showpoint[i] + 1, Points[i].Qty))
    {
      ut_print_message (0, 2, "Printing %s...\n", Points[i].Name);
      nev_print_real_png_point (file, Points[i], PointData[i], Print, Print.showpoint[i]);
    }

  // tessellation ------------------------------------------------------
  if (Print.showtess == 1)
  {
    ut_print_message (0, 2, "Printing tessellation...\n");
    nev_print_real_png_tess (file, Print, Tess, TessData);
  }

  // raster tessellation -----------------------------------------------
  if (Print.showtesr == 1)
  {
    ut_print_message (0, 2, "Printing raster tessellation...\n");
    nev_print_real_png_tesr (file, Print, Tesr, TesrData);
  }

  // mesh --------------------------------------------------------------
  if (Print.showmesh == 1)
  {
    ut_print_message (0, 2, "Printing mesh...\n");
    nev_print_real_png_mesh (file, Print, Tess, Nodes, Mesh, *pNodeData, MeshData);
  }


  if (SQty > 0 && Print.showslice)
  {
    ut_print_message (0, 2, "Printing mesh slice%s...\n", SQty > 1? "s" : "");
    nev_print_real_png_meshslices (file, Print, MeshData, SQty, SNodes, SMesh,
                                   SNodeData, SMeshData, SElt2dElt3d);
  }

  neut_print_povfoot (In.includepov, In.imageformat, file);
  ut_file_close (file, filename, ut_list_testelt (In.imageformat,
                 NEUT_SEP_NODEP, "pov") ? "w" : "W");

  if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
    neut_print_pov2png (In.povray, filename, imagewidth, imageheight,
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
