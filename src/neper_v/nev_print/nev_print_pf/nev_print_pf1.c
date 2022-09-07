/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_pf_.h"

void
nev_print_pf (char *basename, struct PRINT Print, struct SIM Sim,
              struct TESS Tess, struct TESR Tesr,
              struct DATA *TessData, struct DATA *TesrData,
              struct DATA CsysData,
              struct POINT *Points, int PointQty, struct DATA *PointData)
{
  int i, ptqty, imagewidth, imageheight, modeqty, ipfptqty, *ptcells = NULL, density_plotted;
  double *wgts = NULL, **pts = NULL, *ptwgts = NULL, **ipfpts = NULL;
  char **modes = NULL, *outdir = NULL, *filename = NULL, *filename2 = NULL;
  char *pole_string = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  struct DATA *pData = NULL;
  struct SIMRES SimRes;
  double **data = NULL;
  char *type = NULL, *value = NULL;

  struct DATAINPUT DataInput;
  neut_datainput_set_default (&DataInput);
  DataInput.pSim = &Sim;
  DataInput.pTess = &Tess;
  DataInput.pTesr = &Tesr;

  neut_simres_set_zero (&SimRes);

  neut_print_outdir (Print, Sim, "pf", &outdir);
  if (strcmp (outdir, "."))
    ut_sys_mkdir (outdir);

  ut_file_dir_basename_extension_filename (outdir, basename, "asy", &filename);
  ut_file_dir_basename_extension_filename (outdir, basename, "level", &filename2);

  ut_print_message (0, 1, "Printing pole figure...\n");
  file = ut_file_open (filename, ut_list_testelt (Print.imageformat,
                       NEUT_SEP_NODEP, "asy") ? "w" : "W");

  nev_print_pf_header (file, Print);

  nev_print_pf_background (file, Print);

  nev_print_pf_border (file, Print, &ipfpts, &ipfptqty);

  nev_print_pf_csys (file, Print, CsysData);

  ut_list_break (Print.pfmode, NEUT_SEP_NODEP, &modes, &modeqty);

  density_plotted = 0;

  if (Print.showtess == 1)
  {
    ut_string_string ("tess", &DataInput.input);

    pData = TessData + Tess.Dim;
    wgts = ut_alloc_1d (Tess.CellQty);
    for (i = 1; i <= Tess.CellQty; i++)
      neut_tess_cell_size (Tess, i, wgts + i - 1);

    if (!(*pData).Value || !strcmp ((*pData).Value, "ori"))
    {
      ut_string_string ("ori", &type);
      data = Tess.CellOri + 1;
    }
    else
    {
      neut_data_datastring_type_value ("entity", "", (*pData).Value, &type, &value);
      neut_data_fscanf_general (DataInput, "cell", 3, (*pData).Qty, "", type, value, pData);
      data = (*pData).Data + 1;
    }

    nev_print_pf_pts (type, data, wgts, Tess.CellQty,
                      Tess.Dim == 3 ? Print.showpoly + 1 : Print.showface + 1,
                      Print, i == 0 ? modes : NULL, i == 0 ? modeqty : 0,
                      &pts, &ptwgts, &ptcells, &ptqty);

    nev_print_pf_ptsprint (file, basename, Print, *pData, ipfpts, ipfptqty,
                           pts, ptwgts, ptcells, ptqty, modes, modeqty, &density_plotted);

    if (!(*pData).Value || !strcmp ((*pData).Value, "ori"))
    {
      sprintf (pole_string, "$\\left\\{%s\\right\\}$", Print.pfpolestring);
      ut_string_fnrs (pole_string, NEUT_SEP_DEP, "", INT_MAX);
    }
    else if (!strncmp ((*pData).Value, "vector", 6))
      ut_string_string ("directions", &pole_string);
  }

  if (Print.showtesr == 1)
  {
    ut_string_string ("tesr", &DataInput.input);

    pData = TesrData;
    wgts = ut_alloc_1d (Tesr.CellQty);
    for (i = 1; i <= Tesr.CellQty; i++)
      neut_tesr_cell_size (Tesr, i, wgts + i - 1);

    data = ut_alloc_2d (Tesr.CellQty + 1, 4);

    if (!(*pData).Value || !strcmp ((*pData).Value, "ori"))
    {
      ut_string_string ("ori", &type);

      if (Tesr.CellOri)
        ut_array_2d_memcpy (Tesr.CellOri + 1, Tesr.CellQty, 4, data + 1);
      else
      {
        FILE *file = NULL;
        neut_sim_simres (Sim, "cell", "ori", &SimRes);
        file = ut_file_open (SimRes.file, "R");
        neut_ori_fscanf (file, Sim.OriDes, "ascii", data + 1, NULL, Tesr.CellQty, NULL);
        ut_file_close (file, SimRes.file, "R");
      }
    }

    else
    {
      neut_data_datastring_type_value ("cell", "", (*pData).Value, &type, &value);
      neut_data_fscanf_general (DataInput, "cell", 3, (*pData).Qty, "", type, value, pData);
      ut_array_2d_memcpy ((*pData).Data + 1, Tesr.CellQty, (*pData).DataSize, data + 1);
    }

    nev_print_pf_pts (type, data + 1, !strcmp (type, "ori") ? wgts : NULL, Tesr.CellQty,
                      Tesr.Dim == 3 ? Print.showpoly + 1 : Print.showface + 1,
                      Print, i == 0 ? modes : NULL, i == 0 ? modeqty : 0,
                        &pts, &ptwgts, &ptcells, &ptqty);

    nev_print_pf_ptsprint (file, basename, Print, *pData, ipfpts, ipfptqty,
                           pts, ptwgts, ptcells, ptqty, modes, modeqty, &density_plotted);

    if (!(*pData).Value || !strcmp ((*pData).Value, "ori"))
    {
      sprintf (pole_string, "$\\left\\{%s\\right\\}$", Print.pfpolestring);
      ut_string_fnrs (pole_string, NEUT_SEP_DEP, "", INT_MAX);
    }
    else if (!strncmp ((*pData).Value, "vector", 6))
      ut_string_string ("directions", &pole_string);

    ut_free_2d (&data, Tesr.CellQty);
  }

  for (i = 0; i < PointQty; i++)
    if (ut_array_1d_int_sum (Print.showpoint[i] + 1, Points[i].Qty) > 0)
    {
      pData = PointData + i;
      nev_print_pf_pts (Points[i].Type, Points[i].Coo + 1, NULL, Points[i].Qty,
                        Print.showpoint[i] + 1, Print,
                        i == 0 ? modes : NULL, i == 0 ? modeqty : 0,
                        &pts, &ptwgts, &ptcells, &ptqty);

      nev_print_pf_ptsprint (file, basename, Print, *pData, ipfpts, ipfptqty,
                             pts, ptwgts, ptcells, ptqty, modes, modeqty, &density_plotted);

      if (strcmp (Points[i].Type, "vector"))
      {
        sprintf (pole_string, "$\\!\\left\\{%s\\right\\}$", Print.pfpolestring);
        ut_string_fnrs (pole_string, NEUT_SEP_DEP, "", INT_MAX);
      }
      else
        ut_string_string ("directions", &pole_string);
    }

  nev_print_pf_pole_proj (file, Print, pole_string);

  // nev_print_pf_compress (InPF, &pts, &ptwgts, &ptqty);

  ut_file_close (file, filename, ut_list_testelt (Print.imageformat,
                 NEUT_SEP_NODEP, "asy") ? "w" : "W");

  neut_print_imagesize (Print, &imagewidth, &imageheight);
  neut_asy_convert (Print.asymptote, filename, imagewidth, imageheight,
                    Print.imageformat, 2);

  if (!ut_list_testelt (Print.imageformat, NEUT_SEP_NODEP, "asy"))
  {
    remove (filename);
    if (density_plotted)
      remove (filename2);
  }

  ut_free_2d (&ipfpts, ipfptqty);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&filename2);
  ut_free_1d (&wgts);
  ut_free_2d (&pts, ptqty);
  ut_free_1d_int (&ptcells);
  ut_free_1d (&ptwgts);
  ut_free_2d_char (&modes, modeqty);
  ut_free_1d_char (&outdir);
  ut_free_1d_char (&pole_string);
  neut_simres_free (&SimRes);
  ut_free_1d_char (&type);
  ut_free_1d_char (&value);

  return;
}
