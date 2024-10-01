/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_pf_.h"

void
nev_print_pf (struct IN_V In, struct PF *pPf, char *basename, struct PRINT Print,
              int *steps, int stepqty, int steppos,
              struct SIM Sim, struct TESS Tess, struct TESR Tesr,
              struct DATA *TessData, struct DATA *TesrData,
              struct MESH *Mesh, struct DATA **MeshData,
              struct DATA CsysData,
              struct POINT *Points, int PointQty, struct DATA *PointData)
{
  int i, ptqty, imagewidth, imageheight, *ptcells = NULL, density_plotted, id;
  double *wgts = NULL, **pts = NULL, *ptwgts = NULL;
  int *cellptqty = NULL;
  double ***cellpts = NULL;
  char *outdir = NULL, *filename = NULL, *filename2 = NULL;
  char *pole_string = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  struct DATA *pData = NULL;
  double **data = NULL;
  char *type = NULL, *value = NULL;
  int cellqty, clustering;
  int isfirststep = (steppos == 0);
  int islaststep = (steppos == stepqty - 1);

  struct DATAINPUT DataInput;
  neut_datainput_set_default (&DataInput);
  DataInput.pSim = &Sim;
  DataInput.pTess = &Tess;
  DataInput.pTesr = &Tesr;

  int *show = NULL;

  (void) steps;

  neut_print_outdir (In.outdir, Sim, "pf", &outdir);
  if (strcmp (outdir, "."))
    ut_sys_mkdir (outdir);

  ut_file_dir_basename_extension_filename (outdir, basename, "asy", &filename);
  ut_file_dir_basename_extension_filename (outdir, basename, "level", &filename2);

  if (isfirststep)
  {
    ut_print_message (0, 1, "Printing pole figure...\n");

    nev_print_pf_inputs (Print, Points, PointQty, pPf);

    file = ut_file_open (filename, ut_list_testelt (In.imageformat,
                         NEUT_SEP_NODEP, "asy") ? "w" : "W");

    nev_print_pf_header (*pPf, file);

    nev_print_pf_background (In, *pPf, file, Print);

    nev_print_pf_csys (*pPf, file, CsysData);

    nev_print_pf_border (file, pPf);
  }

  else
    file = ut_file_open (filename, "A");

  density_plotted = 0;

  for (i = 0; i < (*pPf).inputqty; i++)
  {
    if (!strcmp ((*pPf).inputs[i], "tess"))
      nev_print_pf_pre_tess (Print, Tess, TessData, &DataInput, &pData,
                             &cellqty, &wgts, &data, &type, &show);

    else if (!strcmp ((*pPf).inputs[i], "tesr"))
      nev_print_pf_pre_tesr (Print, Tesr, TesrData, &DataInput, &pData,
                             &cellqty, &wgts, &data, &type, &show);

    else if (!strcmp ((*pPf).inputs[i], "mesh"))
      nev_print_pf_pre_mesh (Print, Mesh, MeshData, &DataInput, &pData,
                             &cellqty, &wgts, &data, &type, &show);

    else if (!strncmp ((*pPf).inputs[i], "point", 5))
    {
      sscanf ((*pPf).inputs[i], "point%d", &id);
      nev_print_pf_pre_points (Print, Points[id], PointData + id, id, &DataInput,
                               &pData, &cellqty, &wgts, &data, &type, &show);
    }

    nev_print_pf_pre_clustering (*pPf, cellqty, &clustering);

    cellptqty = ut_alloc_1d_int (cellqty + 1);
    cellpts = ut_alloc_1d_ppdouble (cellqty + 1);

    nev_print_pf_pts (i, pPf, type, data, wgts, cellqty, show, clustering,
                      &pts, &ptwgts, &ptcells, &ptqty, cellpts, cellptqty);

    nev_print_pf_ptsprint (In, i, pPf, file, basename, Print, *pData,
                           pts, ptwgts, ptcells, ptqty,
                           &density_plotted);

    ut_free_2d (&data, cellqty);
    ut_free_1d_char (&type);
    // free cellptqty
    // free cellpts
  }

  if (isfirststep)
  {
    nev_print_pf_polestring (*pPf, pData, &pole_string);

    nev_print_pf_pole_proj (*pPf, file, pole_string);
  }

  // nev_print_pf_compress (InPF, &pts, &ptwgts, &ptqty);

  if (islaststep)
    ut_file_close (file, filename, ut_list_testelt (In.imageformat,
                   NEUT_SEP_NODEP, "asy") ? "w" : "W");
  else
    ut_file_close (file, filename, "W");

  if (islaststep)
  {
    neut_print_imagesize (In.imagesize, &imagewidth, &imageheight);
    neut_asy_convert (In.asymptote, filename, imagewidth, imageheight,
                      In.imageformat, 2);

    if (!ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "asy"))
    {
      remove (filename);
      if (density_plotted)
        remove (filename2);
    }
  }

  ut_free_1d_char (&filename);
  ut_free_1d_char (&filename2);
  ut_free_1d (&wgts);
  ut_free_2d (&pts, ptqty);
  ut_free_1d_int (&ptcells);
  ut_free_1d (&ptwgts);
  ut_free_1d_char (&outdir);
  ut_free_1d_char (&pole_string);
  ut_free_1d_char (&type);
  ut_free_1d_char (&value);

  return;
}
