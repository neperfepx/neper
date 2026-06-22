/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_ori_.h"

void
nev_print_ori_inputs (struct PRINT Print, struct POINT *Points,
                     int PointQty, struct ORI *pOri)
{
  int i;

  if (Print.showtess == 1)
  {
    (*pOri).inputqty++;
    (*pOri).inputs = ut_realloc_1d_pchar_null ((*pOri).inputs, (*pOri).inputqty, 1);
    ut_string_string ("tess", (*pOri).inputs + (*pOri).inputqty - 1);
  }

  if (Print.showtesr == 1)
  {
    (*pOri).inputqty++;
    (*pOri).inputs = ut_realloc_1d_pchar_null ((*pOri).inputs, (*pOri).inputqty, 1);
    ut_string_string ("tesr", (*pOri).inputs + (*pOri).inputqty - 1);
  }

  if (Print.showmesh == 1)
  {
    (*pOri).inputqty++;
    (*pOri).inputs = ut_realloc_1d_pchar_null ((*pOri).inputs, (*pOri).inputqty, 1);
    ut_string_string ("mesh", (*pOri).inputs + (*pOri).inputqty - 1);
  }

  for (i = 0; i < PointQty; i++)
    if (ut_array_1d_int_sum (Print.showpoint[i] + 1, Points[i].Qty) > 0)
    {
      (*pOri).inputqty++;
      (*pOri).inputs = ut_realloc_2d_char_addline ((*pOri).inputs, (*pOri).inputqty, 100);
      sprintf ((*pOri).inputs[(*pOri).inputqty - 1], "point%d", i);
    }

  return;
}

void
nev_print_ori_pre_tess (struct PRINT Print, struct TESS Tess,
                       struct DATA *TessData, struct DATAINPUT *pDataInput,
                       struct DATA **ppData, int *pcellqty, double **pwgts,
                       double ***pdata, char **ptype,
                       struct OL_SET *pOSet, int **pshow)
{
  int i;
  char *value = NULL;
  struct SIMRES SimRes;
  neut_simres_set_zero (&SimRes);

  ut_string_string ("tess", &(*pDataInput).input);
  *ppData = TessData + Tess.Dim;

  (*pcellqty) = Tess.CellQty;
  (*pwgts) = ut_alloc_1d (*pcellqty);
  for (i = 1; i <= *pcellqty; i++)
    (*pwgts)[i - 1] = (**ppData).Weight[i];

  (*pdata) = ut_alloc_2d (*pcellqty, 4);

  neut_tess_olset (Tess, pOSet);

  if (!(**ppData).Value || !strcmp ((**ppData).Value, "ori"))
  {
    ut_string_string ("ori", ptype);
    neut_tess_cellori (Tess, *pdata - 1);
  }
  else
  {
    neut_data_datastring_type_value ("cell", "", (**ppData).Value, ptype, &value);
    neut_data_fscanf_general (*pDataInput, "cell", 3, (**ppData).Qty, "", *ptype, value, *ppData);
    ut_array_2d_memcpy ((**ppData).Data + 1, *pcellqty, (**ppData).DataSize, *pdata);
  }

  ut_string_string ("none", &((**ppData).RadDataType));
  (**ppData).SymbolDataType = NULL;

  *pshow = Tess.Dim == 3 ? Print.showpoly + 1 : Print.showface + 1;

  neut_simres_free (&SimRes);

  ut_free_1d_char (&value);

  return;
}

void
nev_print_ori_pre_tesr (struct PRINT Print, struct TESR Tesr,
                       struct DATA *TesrData, struct DATAINPUT *pDataInput,
                       struct DATA **ppData, int *pcellqty, double **pwgts,
                       double ***pdata, char **ptype, struct OL_SET *pOSet, int **pshow)
{
  int i;
  char *value = NULL;
  struct SIMRES SimRes;
  neut_simres_set_zero (&SimRes);

  ut_string_string ("tesr", &(*pDataInput).input);
  *ppData = TesrData;

  (*pcellqty) = Tesr.CellQty;
  (*pwgts) = ut_alloc_1d (*pcellqty);
  for (i = 1; i <= *pcellqty; i++)
    (*pwgts)[i - 1] = (**ppData).Weight[i];

  (*pdata) = ut_alloc_2d (*pcellqty, 4);

  neut_tesr_olset (Tesr, pOSet);

  if (!(**ppData).Value || !strcmp ((**ppData).Value, "ori"))
  {
    ut_string_string ("ori", ptype);
    neut_tesr_cellori (Tesr, *pdata - 1);
  }

  else
  {
    neut_data_datastring_type_value ("cell", "", (**ppData).Value, ptype, &value);
    neut_data_fscanf_general (*pDataInput, "cell", 3, (**ppData).Qty, "", *ptype, value, *ppData);
    ut_array_2d_memcpy ((**ppData).Data + 1, *pcellqty, (**ppData).DataSize, *pdata);
  }

  ut_free_1d_char (&value);

  *pshow = Tesr.Dim == 3 ? Print.showpoly + 1 : Print.showface + 1;

  neut_simres_free (&SimRes);

  return;
}

void
nev_print_ori_pre_mesh (struct PRINT Print, struct NODES Nodes,
                       struct MESH *Mesh, struct DATA **MeshData,
                       struct DATAINPUT *pDataInput, struct DATA **ppData,
                       int *pcellqty, double **pwgts, double ***pdata,
                       char **ptype, struct OL_SET *pOSet, int **pshow)
{
  int i, dim = neut_mesh_array_dim (Mesh);
  char *value = NULL;
  struct SIMRES SimRes;
  neut_simres_set_zero (&SimRes);

  ut_string_string ("mesh", &(*pDataInput).input);
  *ppData = MeshData[dim] + 1;

  (*pcellqty) = Mesh[dim].ElsetQty;
  (*pwgts) = ut_alloc_1d (*pcellqty);
  for (i = 1; i <= *pcellqty; i++)
    (*pwgts)[i - 1] = (**ppData).Weight[i];

  (*pdata) = ut_alloc_2d (*pcellqty, 4);

  neut_mesh_olset (Nodes, Mesh[3], pOSet);

  if (!(**ppData).Value || !strcmp ((**ppData).Value, "ori"))
  {
    ut_string_string ("ori", ptype);
    neut_mesh_elsetori (Mesh[dim], *pdata - 1);
  }

  else
  {
    neut_data_datastring_type_value ("elset", "", (**ppData).Value, ptype, &value);
    neut_data_fscanf_general (*pDataInput, "elset", 3, (**ppData).Qty, "", *ptype, value, *ppData);
    ut_array_2d_memcpy ((**ppData).Data + 1, *pcellqty, (**ppData).DataSize, *pdata);
  }

  ut_free_1d_char (&value);

  *pshow = dim == 3 ? Print.showelset3d + 1 : Print.showelset2d + 1;

  neut_simres_free (&SimRes);

  return;
}

void
nev_print_ori_pre_points (struct PRINT Print,
                         struct POINT Points, struct DATA *PointData, int id,
                         struct DATAINPUT *pDataInput, struct DATA **ppData,
                         int *pcellqty, double **pwgts, double ***pdata,
                         char **ptype, struct OL_SET *pOSet, int **pshow)
{
  int i;

  ut_string_string ("point", &(*pDataInput).input);
  *ppData = PointData;

  (*pcellqty) = Points.Qty;
  (*pwgts) = ut_alloc_1d (*pcellqty);
  for (i = 1; i <= *pcellqty; i++)
    (*pwgts)[i - 1] = (**ppData).Weight[i];

  (*pdata) = ut_alloc_2d (*pcellqty, 4);

  neut_point_olset (Points, pOSet);

  if (!strcmp (Points.Type, "none"))
  {
    for (i = 0; i < *pcellqty; i++)
      ol_R_q (Points.Coo[i + 1], (*pdata)[i]);

    ut_string_string ("ori", &(**ppData).Value);
  }

  else if (!strcmp (Points.Type, "ori"))
  {
    ut_array_2d_memcpy (Points.Coo + 1, *pcellqty, 4, *pdata);
    ut_string_string (strcmp (Points.Type, "none") ? Points.Type : "ori", &(**ppData).Value);

    ut_string_string ("ori", &(**ppData).Value);
  }

  else
  {
    ut_array_2d_memcpy (Points.Coo + 1, *pcellqty, 3, *pdata);

    ut_string_string (Points.Type, &(**ppData).Value);
  }

  ut_string_string ((**ppData).Value, ptype);

  *pshow = Print.showpoint[id] + 1;

  return;
}

void
nev_print_ori_csys (struct PRINT Print, struct ORI Ori, FILE *file, struct DATA CsysData)
{
  double **pos = ut_alloc_2d (3, 3);
  double hex_ratio = 32. / 20;
  double hex_ratio2 = 1.0165;

  if (!Ori.space)
    ut_print_message (2, 3, "Orientation space undefined.\n");

  if (!strcmp (Ori.space, "rodrigues") && !strcmp (Ori.crysym, "cubic"))
  {
    ut_array_1d_set_3 (pos[0], 0.12, 0, 0);
    ut_array_1d_set_3 (pos[1], 0.055, 0.065, 0);
    ut_array_1d_set_3 (pos[2], 0.027, -0.027, 0.02);
  }

  else if (!strcmp (Ori.space, "rodrigues") && !strcmp (Ori.crysym, "hexagonal"))
  {
    ut_array_1d_set_3 (pos[0], 0.12, 0, 0);
    ut_array_1d_set_3 (pos[1], 0.055, 0.065, 0);
    ut_array_1d_set_3 (pos[2], 0.027, -0.027, 0.02);
    for (int i = 0; i < 3; i++)
      ut_array_1d_scale (pos[i], 3, hex_ratio * hex_ratio2);
  }

  else
  {
    printf ("Ori.space = %s\n", Ori.space);
    printf ("Ori.crysym = %s\n", Ori.crysym);
    abort ();
  }

  nev_print_real_png_csys (file, CsysData, Print, pos);

  ut_free_2d (&pos, 3);

  return;
}

void
nev_print_ori_pts (int input, struct ORI *pOri, char *type_in,
                  double **data_in, double *weights, int qty, int *show,
                  int clustering, double ***ppts, double **pptwgts,
                  int **pptcells, int *pptqty, double ***cellpts,
                  int *cellptqty)
{
  int i;
  char *type = NULL;
  double **data = NULL;
  (void) clustering;

  *pptqty = 0;

  if (!type_in || !strcmp (type_in, "none")) // if none, we assume rodrigues vectors
  {
    data = ut_alloc_2d (qty, 4);
    for (i = 0; i < qty; i++)
      ol_R_q (data_in[i], data[i]);
    ut_string_string ("ori", &type);
  }
  else
  {
    type = type_in;
    data = data_in;
  }

  if (!strcmp (type, "ori"))
    nev_print_ori_file_data_ori (input, pOri, data, weights, qty, show,
                                ppts, pptwgts, pptcells, pptqty);

  else
    ut_print_exprbug (type);

  // record cellptqty and cellpts

  int ptcell;

  for (i = 0; i < *pptqty; i++)
  {
    ptcell = (*pptcells)[i];

    cellptqty[ptcell]++;
    cellpts[ptcell] = ut_realloc_2d_addline (cellpts[ptcell],
                                             cellptqty[ptcell], 2);
    ut_array_1d_memcpy ((*ppts)[i], 2, cellpts[ptcell][cellptqty[ptcell] - 1]);
  }

  if (type != type_in)
    ut_free_1d_char (&type);
  if (data != data_in)
    ut_free_2d (&data, qty);

  return;
}

void
nev_print_ori_ptsprint (struct IN_V In, char *layout, int input, struct ORI *pOri,
                        char *filename, FILE *file,
                        struct PRINT Print, struct DATA Data,
                        double **pts, double *ptwgts, int *ptcells, int ptqty,
                        struct OL_SET *pOSet, int *pdensity_plotted)
{
  int i, modeqty;
  char **modes = NULL;
  (void) ptwgts;

  ut_list_break ((*pOri).mode, NEUT_SEP_NODEP, &modes, &modeqty);

  for (i = 0; i < modeqty; i++)
  {
    if (!strcmp (modes[i], "symbol"))
    {
      ut_print_message (0, 2, "Computing symbols...\n");

      nev_print_ori_ptsprint_symbols (In, Print, input, pOri, file, Data, pts, ptcells,
                                     ptqty);
    }

    else if (!strcmp (modes[i], "density"))
    {
      ut_print_message (0, 2, "Computing densities...\n");

      if (!(*pdensity_plotted))
      {
        nev_print_ori_ptsprint_density (In, layout, filename, file, Print, Data, pOri, pOSet);
        (*pdensity_plotted) = 1;
      }
    }

    else
      ut_print_exprbug (modes[i]);
  }

  ut_free_2d_char (&modes, modeqty);

  return;
}

void
nev_print_ori_space (struct IN_V In, struct ORI Ori, struct PRINT Print, FILE *file)
{
  int i, dim, *rgb = ut_alloc_1d_int (3);
  struct DATA *TessData;
  struct PRINT TessPrint;
  char *basecol = NULL, *crysym = NULL;

  ut_string_string (Ori.crysym, &crysym);

  if (strcmp (In.oriedgecol, "default"))
    ut_string_string (In.oriedgecol, &basecol);
  else if (!strcmp (In.colormode, "bright"))
    ut_string_string ("black", &basecol);
  else if (!strcmp (In.colormode, "dark"))
    ut_string_string ("white", &basecol);
  else
    abort ();

  neut_print_set_default (&TessPrint);

  if (!neut_tess_isvoid (Ori.Sp.Tess))
  {
    TessPrint.lightambient = Print.lightambient;
    TessPrint.lightdiffuse = Print.lightdiffuse;
    TessPrint.lightreflection = Print.lightreflection;

    // We show the edges and the space itself
    TessPrint.showtess = 1;
    TessPrint.showcrystal = ut_alloc_1d_int (Ori.Sp.Tess.SeedQty + 1);
    TessPrint.showseed = ut_alloc_1d_int (Ori.Sp.Tess.SeedQty + 1);
    TessPrint.showver  = ut_alloc_1d_int (Ori.Sp.Tess.VerQty  + 1);
    TessPrint.showedge = ut_alloc_1d_int (Ori.Sp.Tess.EdgeQty + 1);
    TessPrint.showface = ut_alloc_1d_int (Ori.Sp.Tess.FaceQty + 1);
    TessPrint.showpoly = ut_alloc_1d_int (Ori.Sp.Tess.PolyQty + 1);
    ut_array_1d_int_set (TessPrint.showedge + 1, Ori.Sp.Tess.EdgeQty, 1);

    if (!strstr (Ori.mode, "density"))
      ut_array_1d_int_set (TessPrint.showpoly + 1, Ori.Sp.Tess.PolyQty, 1);
    else
      ut_array_1d_int_set (TessPrint.showpoly + 1, Ori.Sp.Tess.PolyQty, 0);

    // Initializing TessData

    TessData = calloc (6, sizeof (DATA));
    for (i = 0; i < 6; i++)
      neut_data_set_default (TessData + i);

    if (!neut_tess_isvoid (Ori.Sp.Tess))
      for (dim = 0; dim <= 5; dim++)
        neut_data_init_tess (Ori.Sp.Tess, dim, TessData + dim);

    // Space properties (color and trs)
    TessData[3].Col = ut_alloc_2d_int (TessData[1].Qty + 3, 3);
    ut_color_name_rgb (In.oricol, TessData[3].Col[1]);
    TessData[3].Trs = ut_alloc_1d (TessData[1].Qty + 3);
    TessData[3].Trs[1] = In.oritrs;

    // Space edges properties
    TessData[1].Col = ut_alloc_2d_int (TessData[1].Qty + 1, 3);
    ut_color_name_rgb (basecol, rgb);
    for (i = 1; i <= TessData[1].Qty; i++)
      ut_array_1d_int_memcpy (rgb, 3, TessData[1].Col[i]);

    TessData[1].Trs = ut_alloc_1d (TessData[1].Qty + 1);
    ut_array_1d_set (TessData[1].Trs + 1, TessData[1].Qty, In.oriedgetrs);

    TessData[1].Rad = ut_alloc_1d (TessData[1].Qty + 1);
    double rad;
    if (!strcmp (In.oriedgerad, "default"))
    {
      if (!strcmp (crysym, "cubic"))
        rad = 0.002;
      else if (!strcmp (crysym, "hexagonal"))
        rad = 0.0035;
      else
        abort ();
    }
    else
      rad = atof (In.oriedgerad);

    ut_array_1d_set (TessData[1].Rad + 1, TessData[1].Qty, rad);

    // Printing tess
    nev_print_real_png_tess (file, TessPrint, Ori.Sp.Tess, TessData);

    for (i = 0; i < 6; i++)
      neut_data_free (TessData + i);
  }

  neut_print_free (&TessPrint);
  ut_free_1d_int (&rgb);
  ut_free_1d_char (&basecol);
  ut_free_1d_char (&crysym);

  return;
}
