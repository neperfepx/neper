/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_ori_.h"

void
nev_print_ori (struct IN_V In, struct ORI *pOri, char *basename, struct PRINT Print,
               int *steps, int stepqty, int steppos,
               struct SIM Sim, struct TESS Tess, struct TESR Tesr,
               struct DATA *TessData, struct DATA *TesrData,
               struct NODES Nodes, struct MESH *Mesh, struct DATA **MeshData,
               struct DATA CsysData,
               struct POINT *Points, int PointQty, struct DATA *PointData)
{
  (void) steps;

  int i, j, ptqty, imagewidth, imageheight, *ptcells = NULL, density_plotted, id, status;
  double *wgts = NULL, **pts = NULL, *ptwgts = NULL;
  int *cellptqty = NULL;
  double ***cellpts = NULL;
  char *outdir = NULL, *final_png = NULL, *scalefile = NULL, *scalefile_png = NULL;
  char *pole_string = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  struct DATA *pData = NULL;
  double **data = NULL;
  char *type = NULL, *value = NULL, *tmp_png = NULL;
  int cellqty;
  int isfirststep = (steppos == 0);
  int islaststep = (steppos == stepqty - 1);
  char *space = NULL, *fr = NULL;

  struct DATAINPUT DataInput;
  neut_datainput_set_default (&DataInput);
  DataInput.pSim = &Sim;
  DataInput.pTess = &Tess;
  DataInput.pTesr = &Tesr;

  int *show = NULL;

  ut_string_function_oneargument (In.space, &space, &fr);

  if (strcmp (space, "rodrigues"))
    ut_print_message (2, 2, "Cannot process `%s'.", In.space);

  if (!(*pOri).space)
  {
    ut_string_string (space, &(*pOri).space);
    neut_ospace_fnscanf ((*pOri).crysym, space, fr, &(*pOri).Sp, "R");
  }

  neut_print_outdir (In.outdir, Sim, "ori", &outdir);

  char **tmp = NULL;
  char *extension1 = ut_alloc_1d_char (1000);
  char *extension2 = ut_alloc_1d_char (1000);
  int qty;

  ut_list_break ((*pOri).layout, NEUT_SEP_NODEP, &tmp, &qty);

  char **povs = ut_alloc_2d_char (qty, 1);
  char **pngs = ut_alloc_2d_char (qty, 1);

  ut_file_dir_basename_extension_filename (outdir, basename, "png", &final_png);

  for (i = 0; i < qty; i++)
  {
    sprintf (extension1, "%s.pov", tmp[i]);
    ut_file_dir_basename_extension_filename (outdir, basename, extension1, povs + i);
    sprintf (extension1, "%s.png", tmp[i]);
    ut_file_dir_basename_extension_filename (outdir, basename, extension1, pngs + i);

    sprintf (extension2, "%s.scale.pov", tmp[i]);
    ut_file_dir_basename_extension_filename (outdir, basename, extension2, &scalefile);
    sprintf (extension2, "%s.scale.png", tmp[i]);
    ut_file_dir_basename_extension_filename (outdir, basename, extension2, &scalefile_png);
    ut_file_dir_basename_extension_filename (outdir, basename, "tmp.png", &tmp_png);

    if (isfirststep)
    {
      ut_print_message (0, 1, "Printing orientation space...\n");
      ut_file_openmessage (final_png, "w");

      nev_print_ori_inputs (Print, Points, PointQty, pOri);
      file = ut_file_open (povs[i], ut_list_testelt (In.imageformat,
                           NEUT_SEP_NODEP, "pov") ? "w" : "W");

      neut_print_povheader (In.imageformat, Print, file);

      ut_print_message (0, 2, "Printing fundamental region...\n");

      nev_print_ori_space (In, *pOri, Print, file);

      if (Print.showcsys != 0)
        nev_print_ori_csys (Print, *pOri, file, CsysData);
    }

    else
      file = ut_file_open (povs[i], "A");

    density_plotted = 0;

    for (j = 0; j < (*pOri).inputqty; j++)
    {
      struct OL_SET OSet;

      ol_set_zero (&OSet);

      if (!strcmp ((*pOri).inputs[j], "tess"))
        nev_print_ori_pre_tess (Print, Tess, TessData, &DataInput, &pData,
                               &cellqty, &wgts, &data, &type, &OSet, &show);

      else if (!strcmp ((*pOri).inputs[j], "tesr"))
        nev_print_ori_pre_tesr (Print, Tesr, TesrData, &DataInput, &pData,
                               &cellqty, &wgts, &data, &type, &OSet, &show);

      else if (!strcmp ((*pOri).inputs[j], "mesh"))
        nev_print_ori_pre_mesh (Print, Nodes, Mesh, MeshData, &DataInput, &pData,
                               &cellqty, &wgts, &data, &type, &OSet, &show);

      else if (!strncmp ((*pOri).inputs[j], "point", 5))
      {
        sscanf ((*pOri).inputs[j], "point%d", &id);
        nev_print_ori_pre_points (Print, Points[id], PointData + id, id, &DataInput,
                                 &pData, &cellqty, &wgts, &data, &type, &OSet, &show);
      }

      cellptqty = ut_alloc_1d_int (cellqty + 1);
      cellpts = ut_alloc_1d_ppdouble (cellqty + 1);

      nev_print_ori_pts (j, pOri, type, data, wgts, cellqty, show, 0,
                        &pts, &ptwgts, &ptcells, &ptqty, cellpts, cellptqty);

      nev_print_ori_ptsprint (In, tmp[i], j, pOri, povs[i], file, Print, *pData,
                             pts, ptwgts, ptcells, ptqty, &OSet,
                             &density_plotted);

      ut_free_2d (&data, cellqty);
      ut_free_1d_char (&type);
      // free cellptqty
      // free cellpts
      ol_set_free (&OSet);
    }

    if (islaststep)
      neut_print_povfoot (In.includepov, In.imageformat, file);

    if (islaststep)
      ut_file_close (file, povs[i], ut_list_testelt (In.imageformat,
                     NEUT_SEP_NODEP, "pov") ? "w" : "W");
    else
      ut_file_close (file, povs[i], "W");

    if (islaststep)
    {
      neut_print_imagesize (In.imagesize, &imagewidth, &imageheight);

      if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
        neut_print_pov2png (In.povray, povs[i], imagewidth, imageheight,
                           In.povrayantialiasing, -1);
    }

    if (islaststep && density_plotted && Print.showscale && i == qty - 1 && strstr (In.imageformat, "png"))
      neut_print_pov2png (In.povray, scalefile, 0.3 * imageheight, imageheight,
                         In.povrayantialiasing, -1);

    remove (scalefile);
  }

  if (islaststep && strstr (In.imageformat, "png"))
  {
    char *command = NULL;
    char *command2 = ut_alloc_1d_char (1000);

    ut_string_string ("convert +append", &command);
    for (i = 0; i < qty; i++)
    {
      sprintf (command2, "%s %s", command, pngs[i]);
      ut_string_string (command2, &command);
    }
    if (density_plotted && Print.showscale && scalefile_png)
    {
      sprintf (command2, "%s %s", command, scalefile_png);
      ut_string_string (command2, &command);
    }
    sprintf (command2, "%s %s", command, final_png);
    ut_string_string (command2, &command);

    status = system (command);
    if (status)
      ut_print_message (2, 2, "Failed to append png file (`convert' missing?)");

    for (i = 0; i < qty; i++)
      remove (pngs[i]);

    remove (scalefile_png);
    ut_free_1d_char (&command);
    ut_free_1d_char (&command2);
  }

  ut_file_closemessage (final_png, "w");

  if (!ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov"))
  {
    for (i = 0; i < qty; i++)
      remove (povs[i]);
  }

  ut_free_2d_char (&pngs, qty + 1);
  ut_free_2d_char (&povs, qty + 1);
  ut_free_1d_char (&scalefile);
  ut_free_1d_char (&scalefile_png);
  ut_free_1d (&wgts);
  ut_free_2d (&pts, ptqty);
  ut_free_1d_int (&ptcells);
  ut_free_1d (&ptwgts);
  ut_free_1d_char (&outdir);
  ut_free_1d_char (&pole_string);
  ut_free_1d_char (&type);
  ut_free_1d_char (&value);
  ut_free_1d_char (&tmp_png);
  ut_free_1d_char (&final_png);
  ut_free_1d_char (&space);
  ut_free_1d_char (&fr);

  return;
}
