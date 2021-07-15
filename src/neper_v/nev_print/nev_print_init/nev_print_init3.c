/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_init_.h"

void
nev_print_init_data_csys (struct PRINT Print, struct DATA *pData)
{
  int i;

  (*pData).Col = ut_alloc_2d_int (2, 3);
  (*pData).Rad = ut_alloc_1d (2);
  (*pData).Coo = ut_alloc_2d (2, 3);
  (*pData).Rad = ut_alloc_1d (2);
  (*pData).Length = ut_alloc_1d (2);
  (*pData).Rad[1] = 0.01;
  (*pData).Length[1] = 0.2;

  if ((*pData).ColData)
  {
    if (!strcmp ((*pData).ColDataType, "col"))
      for (i = 0; i < 3; i++)
        (*pData).Col[1][i] = ut_num_d2ri ((*pData).ColData[1][i]);
    else if (ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  if ((*pData).RadData)
    (*pData).Rad[1] = (*pData).RadData[1][0];

  if ((*pData).LengthData)
    (*pData).Length[1] = (*pData).LengthData[1][0];

  if ((*pData).CooData)
    for (i = 0; i < 3; i++)
      (*pData).Coo[1][i] = (*pData).CooData[1][i];

  if (!(*pData).Label)
  {
    (*pData).Label = ut_alloc_1d_pchar (3);
    ut_string_string ("X1", (*pData).Label);
    ut_string_string ("X2", (*pData).Label + 1);
    ut_string_string ("X3", (*pData).Label + 2);
  }

  if ((*pData).FontSize == -1)
    (*pData).FontSize = 1;

  return;
}

void
nev_print_init_data_mesh (struct PRINT Print, struct MESH Mesh, double size, int Qty,
                          int dim, struct DATA *pData)
{
  int i;

  if (Qty == 0)
    ut_print_neperbug ();

  if (Mesh.EltQty == 0)
    return;

  if (!(*pData).Col)
    (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);

  if (Mesh.Dimension == 0)
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 0);
  else if (Mesh.Dimension == 1)
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 0);
  else if (Mesh.Dimension == 2)
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 255);
  else if (Mesh.Dimension == 3)
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 255);

  if (!(*pData).Rad)
    (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);

  if (Mesh.Dimension == 0)
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty,
                     pow (size,
                          1. / 3) * 0.02 * sqrt (Mesh.Dimension +
                                                 1) / pow (Qty, 0.25));
  else if (Mesh.Dimension == 1)
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty,
                     pow (size, 1. / 3) * 0.01414 / pow (Qty, 0.25));
  else if (Mesh.Dimension == 2)
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty,
                     pow (size, 1. / 3) * 0.01000 / pow (Qty, 0.25));
  else if (Mesh.Dimension == 3)
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty,
                     pow (size, 1. / 3) * 0.00707 / pow (Qty, 0.25));

  if (!(*pData).BCol)
  {
    (*pData).BCol = ut_alloc_1d_int (3);

    if (Mesh.Dimension == 0)
      ut_array_1d_int_set ((*pData).BCol, 3, 0);
    else if (Mesh.Dimension == 1)
      ut_array_1d_int_set ((*pData).BCol, 3, 0);
    else if (Mesh.Dimension == 2)
      ut_array_1d_int_set ((*pData).BCol, 3, 0);
    else if (Mesh.Dimension == 3)
      ut_array_1d_int_set ((*pData).BCol, 3, 0);
  }

  if ((*pData).BRad < 0)
  {
    if (Mesh.Dimension == 0)
      (*pData).BRad =
        pow (size, 1. / 3) * 0.02 / pow (Mesh.ElsetQty, 0.25);
    else if (Mesh.Dimension == 1)
      (*pData).BRad =
        pow (size, 1. / 3) * 0.01414 / pow (Mesh.ElsetQty, 0.25);
    else if (Mesh.Dimension == 2)
      (*pData).BRad =
        pow (size, 1. / 3) * 0.01000 / pow (Mesh.ElsetQty, 0.25);
    else if (Mesh.Dimension == 3)
      (*pData).BRad =
        pow (size, 1. / 3) * 0.00707 / pow (Mesh.ElsetQty, 0.25);
  }

  if ((*pData).ColDataType)
  {
    if (!strcmp ((*pData).ColDataType, "from_nodes"))
    {
    }                           // do nothing

    else if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty,
                           (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL, (*pData).Qty,
                            (*pData).Scale, (*pData).ColScheme,
                            (*pData).Col, &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty,
                           (*pData).ColScheme, (*pData).Col);

    else if (ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataName);
  }

  // by default, we color the elts based on their elset ids
  else if ((*pData).Dim == dim)
  {
    (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, 1);
    for (i = 1; i <= (*pData).Qty; i++)
      (*pData).ColData[i][0] = !Mesh.ElsetId ? Mesh.EltElset[i] : Mesh.ElsetId[Mesh.EltElset[i]];

    neut_data_int_color ((*pData).ColData, (*pData).Qty,
                        (*pData).Col);

    ut_string_string ("int", &(*pData).ColDataType);
  }

  if ((*pData).RadData)
    neut_data_rad_radius ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);

  return;
}

void
nev_print_init_data_nodes (struct PRINT Print, struct NODES Nodes, int Qty, struct DATA *pData)
{
  double noderad;

  (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);
  (*pData).Coo = ut_alloc_2d ((*pData).Qty + 1, 3);

  noderad = 0.0168 / pow (Qty, 0.25);
  ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, noderad);

  ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 0.);
  ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, noderad);

  ut_array_2d_memcpy (Nodes.NodeCoo + 1, (*pData).Qty, 3,
                      (*pData).Coo + 1);

  if ((*pData).ColData)
  {
    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty,
                           (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL, (*pData).Qty,
                            (*pData).Scale, (*pData).ColScheme,
                            (*pData).Col, &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty,
                           (*pData).ColScheme, (*pData).Col);

    else if (ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  if ((*pData).RadData)
    neut_data_rad_radius ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);

  if ((*pData).CooDataType)
  {
    if (!strcmp ((*pData).CooDataType, "coo"))
      neut_data_coo_coo (Nodes.NodeCoo, (*pData).CooData,
                        (*pData).CooFact, (*pData).Qty,
                        (*pData).Coo);

    else if (!strcmp ((*pData).CooDataType, "disp"))
      neut_data_disp_coo (Nodes.NodeCoo, (*pData).CooData,
                         (*pData).CooFact, (*pData).Qty,
                         (*pData).Coo);
    else
      abort ();
  }

  return;
}

void
nev_print_init_data_points (struct PRINT Print, struct POINT Point, struct DATA *pData)
{
  double pointrad;

  (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);
  (*pData).Trs = ut_alloc_1d ((*pData).Qty + 1);
  (*pData).Coo = ut_alloc_2d ((*pData).Qty + 1, 3);

  pointrad = 0.0168 / pow ((*pData).Qty, 0.25);
  ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, pointrad);
  ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 128);
  ut_array_2d_memcpy (Point.PointCoo + 1, (*pData).Qty, 3,
                      (*pData).Coo + 1);

  if ((*pData).ColData)
  {
    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty,
                           (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL,
                            (*pData).Qty, (*pData).Scale,
                            (*pData).ColScheme, (*pData).Col,
                            &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty,
                           (*pData).ColScheme, (*pData).Col);

    else if (ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  if (!(*pData).RadDataType)
    ut_string_string ("rad", &((*pData).RadDataType));
  if ((*pData).RadData)
    neut_data_rad_radius ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);

  if ((*pData).TrsData)
    neut_data_tr_tr ((*pData).TrsData, (*pData).Qty,
                    (*pData).Trs);

  if ((*pData).CooDataType)
  {
    if (!strcmp ((*pData).CooDataType, "coo"))
      neut_data_coo_coo (Point.PointCoo, (*pData).CooData,
                        (*pData).CooFact, (*pData).Qty,
                        (*pData).Coo);
    else if (!strcmp ((*pData).CooDataType, "disp"))
      neut_data_disp_coo (Point.PointCoo, (*pData).CooData,
                         (*pData).CooFact, (*pData).Qty,
                         (*pData).Coo);
    else
      abort ();
  }

  if (!(*pData).BCol)
    (*pData).BCol = ut_alloc_1d_int (3);

  return;
}

void
nev_print_init_data_tesr (struct PRINT Print, struct TESR Tesr, struct DATA *pData)
{
  int i, j, k, id, Qty = ut_array_1d_int_prod (Tesr.size, 3);
  double size, rad;

  (*pData).Col = ut_alloc_2d_int (Qty + 1, 3);
  (*pData).Trs = ut_alloc_1d (Qty + 1);

  ut_array_2d_int_set ((*pData).Col + 1, Qty, 3, 255);
  ut_array_1d_set ((*pData).Trs + 1, Qty, 0);

  if ((*pData).ColData)
  {
    ut_print_message (0, 1, "Computing colors...\n");

    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, Qty, (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, Qty, (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, (*pData).ColDataDef,
                            Qty, (*pData).Scale, (*pData).ColScheme,
                            (*pData).Col, &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori")
          || !strcmp ((*pData).ColDataType, "disori"))
      neut_data_ori_color ((*pData).ColData, Qty, (*pData).ColScheme,
                           (*pData).Col);

    else if (ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  // by default, we color the cells based on their ids
  else
  {
    (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, 1);
    id = 0;
    for (k = 1; k <= Tesr.size[2]; k++)
      for (j = 1; j <= Tesr.size[1]; j++)
        for (i = 1; i <= Tesr.size[0]; i++)
        {
          (*pData).ColData[++id][0] = Tesr.VoxCell[i][j][k];
          if (Tesr.CellId)
            (*pData).ColData[id][0] = Tesr.CellId[ut_num_d2ri ((*pData).ColData[id][0])];
        }

    neut_data_int_color ((*pData).ColData, (*pData).Qty,
                        (*pData).Col);

    ut_string_string ("int", &(*pData).ColDataType);
  }

  if ((*pData).TrsData)
  {
    ut_print_message (0, 1, "Computing transparency...\n");
    neut_data_tr_tr ((*pData).TrsData, Qty, (*pData).Trs);
  }

  if ((*pData).BRad < 0)
    (*pData).BRad = 0;

  if (!(*pData).BCol)
    (*pData).BCol = ut_alloc_1d_int (3);

  if (!(*pData).VoidCol)
  {
    (*pData).VoidCol = ut_alloc_1d_int (3);
    ut_array_1d_int_set ((*pData).VoidCol, 3, 128);
  }

  if (!(*pData).RadDataName)
  {
    (*pData).RadDataName = ut_alloc_1d_char (100);
    neut_tesr_size (Tesr, &size);
    rad = pow (size, 1. / 3) * 0.01414 / pow (Tesr.CellQty, 0.25);
    sprintf ((*pData).RadDataName, "%g", rad);
  }

  return;
}

void
nev_print_init_data_tess (struct PRINT Print, struct TESS Tess, struct DATA *pData)
{
  int i;
  double size, rad, dim = (*pData).Dim;

  neut_tess_size (Tess, &size);

  (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  (*pData).Trs = ut_alloc_1d ((*pData).Qty + 1);
  (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);

  if (dim == 0)
  {
    rad = pow (size, 1. / 3) * 0.02000 / pow (Tess.CellQty, 0.25);
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 0);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, rad);
  }
  else if (dim == 1)
  {
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 0);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
    rad = pow (size, 1. / 3) * 0.01414 / pow (Tess.CellQty, 0.25);
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, rad);
  }
  else if (dim == 2 || dim == 3)
  {
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 255);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
  }
  else if (dim == 4)
  {
    rad = pow (size, 1. / 3) * 0.04000 / pow (Tess.CellQty, 0.25);
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 128);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, rad);
  }

  if ((*pData).ColData)
  {
    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty,
                           (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL, (*pData).Qty,
                            (*pData).Scale, (*pData).ColScheme,
                            (*pData).Col,
                            &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty, (*pData).ColScheme,
                            (*pData).Col);

    else if (ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (Print.format, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  // by default, we color the cells based on their ids
  else if ((*pData).Dim == Tess.Dim)
  {
    (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, 1);
    if (!Tess.CellId)
      for (i = 1; i <= (*pData).Qty; i++)
        (*pData).ColData[i][0] = i;
    else
      for (i = 1; i <= (*pData).Qty; i++)
        (*pData).ColData[i][0] = Tess.CellId[i];
    neut_data_int_color ((*pData).ColData, (*pData).Qty,
                        (*pData).Col);

    ut_string_string ("int", &(*pData).ColDataType);
  }

  if ((*pData).TrsData)
    neut_data_tr_tr ((*pData).TrsData, (*pData).Qty, (*pData).Trs);

  if ((*pData).RadData)
    neut_data_rad_radius ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);

  return;
}

void
nev_print_init_camera_coo_tess (struct TESS Tess, char *expr, double *coo)
{
  double *v = ut_alloc_1d (3);
  double **bbox = ut_alloc_2d (3, 2);

  nev_print_init_camera_v (Tess.Dim, v);

  if (strchr (expr, 'x') || strchr (expr, 'y') || strchr (expr, 'z'))
    neut_tess_bbox (Tess, bbox);

  nev_print_init_camera_coo (bbox, NULL, v, expr, coo);

  ut_free_1d (&v);
  ut_free_2d (&bbox, 3);

  return;
}

void
nev_print_init_camera_coo_mesh (struct NODES Nodes, struct MESH Mesh, char *expr,
                          double *coo)
{
  double *v = ut_alloc_1d (3);
  double **bbox = ut_alloc_2d (3, 2);
  (void) Mesh;

  nev_print_init_camera_v (neut_nodes_dim (Nodes), v);

  if (strchr (expr, 'x') || strchr (expr, 'y') || strchr (expr, 'z'))
  {
    if (Nodes.NodeQty > 0)
      neut_nodes_bbox (Nodes, bbox);
    else
      ut_print_neperbug ();
  }

  nev_print_init_camera_coo (bbox, NULL, v, expr, coo);

  ut_free_1d (&v);
  ut_free_2d (&bbox, 3);

  return;
}

void
nev_print_init_camera_coo_tesr (struct TESR Tesr, char *expr, double *coo)
{
  double *v = ut_alloc_1d (3);
  double **bbox = ut_alloc_2d (3, 2);
  double *centre = NULL;

  nev_print_init_camera_v (Tesr.Dim, v);

  if (strchr (expr, 'x') || strchr (expr, 'y') || strchr (expr, 'z'))
  {
    neut_tesr_bbox (Tesr, bbox);
    centre = ut_alloc_1d (3);
    neut_tesr_centre (Tesr, centre);
  }

  nev_print_init_camera_coo (bbox, centre, v, expr, coo);

  ut_free_1d (&v);
  ut_free_2d (&bbox, 3);
  ut_free_1d (&centre);

  return;
}

void
nev_print_init_camera_coo_points (struct POINT Point, char *expr, double *coo)
{
  double *v = ut_alloc_1d (3);
  double **bbox = ut_alloc_2d (3, 2);

  nev_print_init_camera_v (Point.Dim, v);

  if (strchr (expr, 'x') || strchr (expr, 'y') || strchr (expr, 'z'))
    neut_point_bbox (Point, bbox);

  nev_print_init_camera_coo (bbox, NULL, v, expr, coo);

  ut_free_1d (&v);
  ut_free_2d (&bbox, 3);

  return;
}

void
nev_print_init_camera_sky (char *expr, int dim, double *coo)
{
  int i, qty = 0;
  char **vals = NULL;

  if (!strcmp (expr, "default"))
  {
    if (dim == 2)
      ut_array_1d_set_3 (coo, 0, 1, 0);
    else if (dim == 3)
      ut_array_1d_set_3 (coo, 0, 0, 1);
    else
      abort ();
  }

  else
  {
    ut_list_break (expr, NEUT_SEP_DEP, &vals, &qty);

    if (qty != 3)
      ut_print_message (2, 2, "Expression `%s' could not be processed.\n",
                        expr);

    for (i = 0; i < 3; i++)
      ut_string_real (vals[i], coo + i);
  }

  ut_free_2d_char (&vals, qty);

  return;
}
