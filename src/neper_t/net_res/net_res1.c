/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_res_.h"

void
net_res (struct IN_T In, struct TESS FTess, struct TESR Tesr)
{
  FILE *file = NULL;

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "tess"))
  {
    file = ut_file_open (In.tess, "w");
    neut_tess_fprintf (file, FTess);
    ut_file_close (file, In.tess, "w");
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "geo"))
  {
    file = ut_file_open (In.geo, "w");
    neut_tess_fprintf_gmsh (file, FTess);
    ut_file_close (file, In.geo, "w");
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "ply"))       // gmsh ply file
  {
    if (FTess.CellQty != 0)
    {
      file = ut_file_open (In.ply, "w");
      neut_tess_fprintf_ply (file, FTess);
      ut_file_close (file, In.ply, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n",
                        In.ply);
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "obj"))       // obj file
  {
    if (FTess.CellQty != 0)
    {
      file = ut_file_open (In.obj, "w");
      neut_tess_fprintf_obj (file, FTess);
      ut_file_close (file, In.obj, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n",
                        In.obj);
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "3dec"))      // Itasca 3DEC file
  {
    if (FTess.PolyQty != 0)
    {
      file = ut_file_open (In.dec, "w");
      neut_tess_fprintf_dec (file, FTess);
      ut_file_close (file, In.dec, "w");
    }
    else
      ut_print_message (1, 0, "Tess is void; cannot export; skipping\n");
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "stl"))
  {
    if (FTess.PolyQty != 0)
    {
      file = ut_file_open (In.stl, "w");
      neut_tess_fprintf_stl (file, FTess);
      ut_file_close (file, In.stl, "w");
    }
    else
      ut_print_message (1, 0, "Tess is void; cannot export; skipping\n");
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "stl:bycell"))
  {
    if (FTess.PolyQty != 0)
      neut_tess_name_fprintf_stl_bycell (In.body, FTess);
    else
      ut_print_message (1, 0, "Tess is void; cannot export; skipping\n");
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "fe"))        // SurfaceEvolver fe
  {
    if (FTess.PolyQty != 0)
    {
      file = ut_file_open (In.fe, "w");
      neut_tess_fprintf_fe (file, FTess);
      ut_file_close (file, In.fe, "w");
    }
    else
      ut_print_message (1, 0, "Tess is void; cannot export; skipping\n");
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "tesr"))      // tesr file
  {
    if (Tesr.CellQty != 0)
    {
      file = ut_file_open (In.tesr, "w");
      neut_tesr_fprintf (file, In.tesrformat, Tesr);
      ut_file_close (file, In.tesr, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n",
                        In.tesr);
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "raw"))       // tesr/raw file
  {
    if (Tesr.CellQty != 0)
      neut_tesr_name_fprintf_raw (In.tesr, In.raw, In.tesrformat, Tesr);
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n",
                        In.raw);
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "vtk"))       // vtk file
  {
    if (Tesr.CellQty != 0)
    {
      file = ut_file_open (In.vtk, "w");
      neut_tesr_fprintf_ami (file, In.tesrformat, Tesr);
      ut_file_close (file, In.vtk, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n",
                        In.vtk);
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "olmap"))     // olmap file
  {
    if (Tesr.CellQty != 0)
    {
      file = ut_file_open (In.olmap, "w");
      neut_tesr_fprintf_olmap (file, Tesr);
      ut_file_close (file, In.olmap, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n",
                        In.olmap);
  }

  if (FTess.VerQty > 0
      && ut_list_testelt (In.format, NEUT_SEP_NODEP, "debug"))
  {
    file = ut_file_open (In.debug, "w");
    neut_debug_tess (file, FTess);
    ut_file_close (file, In.debug, "w");
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "ori"))
    net_res_ori (In, FTess);

  return;
}
