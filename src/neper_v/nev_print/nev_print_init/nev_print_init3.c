/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_init_.h"

void
nev_print_init_data_csys (struct IN_V In, struct DATA *pData)
{
  int i;

  (*pData).Col = ut_alloc_2d_int (2, 3);
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
    else if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
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
    ut_string_string ("$X_1$", (*pData).Label);
    ut_string_string ("$X_2$", (*pData).Label + 1);
    ut_string_string ("$X_3$", (*pData).Label + 2);
  }

  if ((*pData).FontSize == -1)
    (*pData).FontSize = 1;

  return;
}

void
nev_print_init_data_mesh (struct IN_V In, struct NODES Nodes, struct MESH Mesh, char *crysym,
                          double size, int Qty, char *entity, int dim,
                          struct DATA *pData)
{
  int i;

  if (Qty == 0)
    ut_print_neperbug ();

  if (Mesh.EltQty == 0)
    return;

  if (!(*pData).Col)
    (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  if (!(*pData).Trs)
    (*pData).Trs = ut_alloc_1d ((*pData).Qty + 1);

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

  if (!(*pData).Weight)
  {
    (*pData).Weight = ut_alloc_1d ((*pData).Qty + 1);
    if (Mesh.Dimension < dim)
      ut_array_1d_set ((*pData).Weight + 1, (*pData).Qty, 1);
    else
    {
      if (!strstr (entity, "elset"))
        ut_array_1d_set ((*pData).Weight + 1, (*pData).Qty, 1);
      else
        for (i = 1; i <= (*pData).Qty; i++)
          neut_mesh_elset_size (Nodes, Mesh, i, (*pData).Weight + i);
    }
  }

  if (Mesh.Dimension == 0)
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty,
                     pow (size,
                          1. / 3) * 0.02 * sqrt (Mesh.Dimension +
                                                 1) / pow (Qty, 0.25));
  else if (Mesh.Dimension == 1)
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty,
                     pow (size, 1. / 3) * 0.01414 / pow (Qty, 0.25));
  else if (Mesh.Dimension == 2) // used by pf
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, 0.02);
  else if (Mesh.Dimension == 3) // used by pf
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, 0.02);

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
    {
      if (!strcmp (In.space, "real"))
        (*pData).BRad = pow (size, 1. / 3) * 0.01000 / pow (Mesh.ElsetQty, 0.25);
      else
        (*pData).BRad = 0.01;
    }
    else if (Mesh.Dimension == 3)
    {
      if (!strcmp (In.space, "real"))
        (*pData).BRad = pow (size, 1. / 3) * 0.00707 / pow (Mesh.ElsetQty, 0.25);
      else
        (*pData).BRad = 0.01;
    }
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
      neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL, (*pData).Qty,
                            (*pData).Scale, (*pData).ColScheme,
                            (*pData).Col, &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty,
                           crysym, (*pData).ColScheme, (*pData).Col);

    else if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataName);
  }

  // by default, we color the elts based on their elset ids
  else if ((*pData).Dim == dim)
  {
    (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, 1);
    if (!entity || !strcmp (entity, "elt"))
      for (i = 1; i <= (*pData).Qty; i++)
        (*pData).ColData[i][0] = !Mesh.ElsetId ? Mesh.EltElset[i] : Mesh.ElsetId[Mesh.EltElset[i]];
    else if (!strcmp (entity, "elset"))
      for (i = 1; i <= (*pData).Qty; i++)
        (*pData).ColData[i][0] = !Mesh.ElsetId ? Mesh.ElsetId[i] : i;
    else if (!strcmp (entity, "mesh"))
      for (i = 1; i <= (*pData).Qty; i++)
        (*pData).ColData[i][0] = i;

    neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                        (*pData).Col);

    ut_string_string ("int", &(*pData).ColDataType);
  }

  if ((*pData).RadData)
    neut_data_real_real ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);

  if ((*pData).WeightData)
    neut_data_real_real ((*pData).WeightData, (*pData).Qty,
                         (*pData).Weight);

  if ((*pData).TrsData)
    neut_data_real_real ((*pData).TrsData, (*pData).Qty, (*pData).Trs);

  neut_data_symboldata_symbol (pData);

  return;
}

void
nev_print_init_data_nodes (struct IN_V In, struct NODES Nodes, char *crysym, int Qty, struct DATA *pData)
{
  double noderad;

  if (!(*pData).Col)
    (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  if (!(*pData).Rad)
    (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Weight)
    (*pData).Weight = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Coo)
    (*pData).Coo = ut_alloc_2d ((*pData).Qty + 1, 3);

  noderad = 0.0168 / pow (Qty, 0.25);
  ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, noderad);

  ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 0.);
  ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, noderad);
  ut_array_1d_set ((*pData).Weight + 1, (*pData).Qty, 1);

  ut_array_2d_memcpy (Nodes.NodeCoo + 1, (*pData).Qty, 3,
                      (*pData).Coo + 1);

  if ((*pData).ColData)
  {
    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty,
                           (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL, (*pData).Qty,
                            (*pData).Scale, (*pData).ColScheme,
                            (*pData).Col, &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty,
                           crysym, (*pData).ColScheme, (*pData).Col);

    else if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  if ((*pData).RadData)
    neut_data_real_real ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);

  if ((*pData).WeightData)
    neut_data_real_real ((*pData).WeightData, (*pData).Qty,
                         (*pData).Weight);

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
nev_print_init_data_points (struct IN_V In, struct POINT Point, char *crysym, struct DATA *pData)
{
  int i;
  double pointrad;

  if (!(*pData).Col)
    (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  if (!(*pData).Rad)
    (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Weight)
    (*pData).Weight = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Trs)
    (*pData).Trs = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Coo)
    (*pData).Coo = ut_alloc_2d ((*pData).Qty + 1, 3);

  pointrad = 0.0168 / pow ((*pData).Qty, 0.25);
  ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, pointrad);
  ut_array_1d_set ((*pData).Weight + 1, (*pData).Qty, 1);
  ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 128);
  ut_array_2d_memcpy (Point.Coo + 1, (*pData).Qty, 3,
                      (*pData).Coo + 1);

  if ((*pData).ColData)
  {
    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty,
                           (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL,
                            (*pData).Qty, (*pData).Scale,
                            (*pData).ColScheme, (*pData).Col,
                            &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty,
                           crysym, (*pData).ColScheme, (*pData).Col);

    else if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  // by default, we color the points based on their ids
  else
  {
    (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, 1);
    for (i = 1; i <= (*pData).Qty; i++)
      (*pData).ColData[i][0] = i;
    neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                        (*pData).Col);

    ut_string_string ("int", &(*pData).ColDataType);
  }

  if (!(*pData).RadDataType)
    ut_string_string ("rad", &((*pData).RadDataType));
  if ((*pData).RadData)
    neut_data_real_real ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);
  else
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, 0.02);

  if ((*pData).WeightData)
    neut_data_real_real ((*pData).WeightData, (*pData).Qty,
                         (*pData).Weight);

  if ((*pData).TrsData)
    neut_data_real_real ((*pData).TrsData, (*pData).Qty,
                    (*pData).Trs);

  if ((*pData).CooDataType)
  {
    if (!strcmp ((*pData).CooDataType, "coo"))
      neut_data_coo_coo (Point.Coo, (*pData).CooData,
                        (*pData).CooFact, (*pData).Qty,
                        (*pData).Coo);
    else if (!strcmp ((*pData).CooDataType, "disp"))
      neut_data_disp_coo (Point.Coo, (*pData).CooData,
                         (*pData).CooFact, (*pData).Qty,
                         (*pData).Coo);
    else
      abort ();
  }

  if ((*pData).BRad < 0)
    (*pData).BRad = 0.01;

  if (!(*pData).BCol)
    (*pData).BCol = ut_alloc_1d_int (3);

  neut_data_symboldata_symbol (pData);

  return;
}

/*
void
nev_print_init_data_crystal (struct PRINT Print, , struct DATA *pData)
{
  double pointrad;

  (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);
  (*pData).Trs = ut_alloc_1d ((*pData).Qty + 1);
  (*pData).Coo = ut_alloc_2d ((*pData).Qty + 1, 3);

  pointrad = 0.0168 / pow ((*pData).Qty, 0.25);
  ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, pointrad);
  ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 128);
  ut_array_2d_memcpy (Crystal.Coo + 1, (*pData).Qty, 3,
                      (*pData).Coo + 1);

  if ((*pData).ColData)
  {
    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty,
                           (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL,
                            (*pData).Qty, (*pData).Scale,
                            (*pData).ColScheme, (*pData).Col,
                            &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty,
                           (*pData).ColScheme, (*pData).Col);

    else if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  if (!(*pData).RadDataType)
    ut_string_string ("rad", &((*pData).RadDataType));
  if ((*pData).RadData)
    neut_data_real_real ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);

  if ((*pData).TrsData)
    neut_data_real_real ((*pData).TrsData, (*pData).Qty,
                    (*pData).Trs);

  if (!(*pData).BCol)
    (*pData).BCol = ut_alloc_1d_int (3);

  return;
}
*/

void
nev_print_init_data_tesr (struct IN_V In, struct TESR Tesr, struct DATA *pData)
{
  int i, j, k, id;
  double size, rad;

  if (!(*pData).Col)
    (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  if (!(*pData).Trs)
    (*pData).Trs = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Rad)
    (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Weight)
    (*pData).Weight = ut_alloc_1d ((*pData).Qty + 1);

  ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 255);
  ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
  ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, 0.02);

  for (i = 1; i <= Tesr.CellQty; i++)
    neut_tesr_cell_size (Tesr, i, (*pData).Weight + i);

  if (!(*pData).VoidCol)
  {
    (*pData).VoidCol = ut_alloc_1d_int (3);
    ut_array_1d_int_set ((*pData).VoidCol, 3, 128);
  }

  if ((*pData).ColData)
  {
    ut_print_message (0, 1, "Computing colors...\n");

    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty, (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol, (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, (*pData).ColDataDef,
                            (*pData).Qty, (*pData).Scale, (*pData).ColScheme,
                            (*pData).Col, &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori")
          || !strcmp ((*pData).ColDataType, "disori"))
    {
      neut_data_ori_color ((*pData).ColData, (*pData).Qty, Tesr.CellCrySym,
                           (*pData).ColScheme, (*pData).Col);
      id = 0;
      if (Tesr.VoxOriDef)
        for (k = 1; k <= Tesr.size[2]; k++)
          for (j = 1; j <= Tesr.size[1]; j++)
            for (i = 1; i <= Tesr.size[0]; i++)
            {
              id++;
              if (!strcmp ((*pData).Entity, "vox") && Tesr.VoxOriDef[i][j][k] == 0)
                ut_array_1d_int_memcpy ((*pData).VoidCol, 3, (*pData).Col[id]);
            }
    }

    else if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  // by default, we color the cells based on their ids
  else
  {
    (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, 1);

    if (!strcmp ((*pData).Entity, "cell"))
    {
      if (!Tesr.CellId)
        for (i = 1; i <= (*pData).Qty; i++)
          (*pData).ColData[i][0] = i;
      else
        for (i = 1; i <= (*pData).Qty; i++)
          (*pData).ColData[i][0] = Tesr.CellId[i];
    }

    else
    {
      id = 0;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
          {
            if (Tesr.CellQty)
            {
              (*pData).ColData[++id][0] = Tesr.VoxCell[i][j][k];
              if (Tesr.CellId)
                (*pData).ColData[id][0] = Tesr.CellId[ut_num_d2ri ((*pData).ColData[id][0])];
            }
          }
    }

    neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                        (*pData).Col);
    for (i = 1; i <= (*pData).Qty; i++)
    {
      if (ut_array_1d_int_max ((*pData).Col[i], 3) == 0)
        ut_array_1d_int_memcpy ((*pData).VoidCol, 3, (*pData).Col[i]);
    }

    ut_string_string ("int", &(*pData).ColDataType);
  }

  if ((*pData).RadData)
    neut_data_real_real ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);

  if ((*pData).WeightData)
    neut_data_real_real ((*pData).WeightData, (*pData).Qty,
                         (*pData).Weight);

  if ((*pData).TrsData)
  {
    ut_print_message (0, 1, "Computing transparency...\n");
    neut_data_real_real ((*pData).TrsData, (*pData).Qty, (*pData).Trs);
  }

  if ((*pData).BRad < 0)
    (*pData).BRad = (!strcmp (In.space, "pf") || !strcmp (In.space, "ipf")) ? 0.01 : 0;

  if (!(*pData).BCol)
    (*pData).BCol = ut_alloc_1d_int (3);

  if (!(*pData).RadDataName)
  {
    (*pData).RadDataName = ut_alloc_1d_char (100);
    neut_tesr_size (Tesr, &size);
    rad = pow (size, 1. / 3) * 0.01414 / pow (Tesr.CellQty, 0.25);
    sprintf ((*pData).RadDataName, "%g", rad);
  }

  neut_data_symboldata_symbol (pData);

  return;
}

void
nev_print_init_data_tess (struct IN_V In, struct TESS Tess, struct DATA *pData)
{
  int i, dim = (*pData).Dim;
  double size, rad;

  neut_tess_size (Tess, &size);

  if (!(*pData).Col)
    (*pData).Col = ut_alloc_2d_int ((*pData).Qty + 1, 3);
  if (!(*pData).Trs)
    (*pData).Trs = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Rad)
    (*pData).Rad = ut_alloc_1d ((*pData).Qty + 1);
  if (!(*pData).Weight)
    (*pData).Weight = ut_alloc_1d ((*pData).Qty + 1);

  // don't else if
  if (dim == 0)
  {
    rad = pow (size, 1. / 3) * 0.02000 / pow (Tess.CellQty, 0.25);
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 0);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, rad);
  }

  if (dim == 1)
  {
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 0);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
    rad = pow (size, 1. / 3) * 0.01414 / pow (Tess.CellQty, 0.25);
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, rad);
  }

  if (dim == 2 || dim == 3)
  {
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 255);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
  }

  if (dim == Tess.Dim)
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, 0.02);

  if (dim == 4)
  {
    rad = pow (size, 1. / 3) * 0.04000 / pow (Tess.CellQty, 0.25);
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 128);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, rad);
  }

  if (dim == 5)
  {
    rad = pow (size, 1. / 3) * 0.04000 / pow (Tess.CellQty, 0.25);
    ut_array_2d_int_set ((*pData).Col + 1, (*pData).Qty, 3, 128);
    ut_array_1d_set ((*pData).Trs + 1, (*pData).Qty, 0);
    ut_array_1d_set ((*pData).Rad + 1, (*pData).Qty, rad);
  }

  if ((*pData).Dim == Tess.Dim)
    for (i = 1; i <= Tess.CellQty; i++)
      neut_tess_cell_size (Tess, i, (*pData).Weight + i);

  if ((*pData).ColData)
  {
    if (!strcmp ((*pData).ColDataType, "col"))
      neut_data_col_color ((*pData).ColData, (*pData).Qty,
                           (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "int"))
      neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                          (*pData).Col);

    else if (!strcmp ((*pData).ColDataType, "real"))
      neut_data_real_color ((*pData).ColData, NULL, (*pData).Qty,
                            (*pData).Scale, (*pData).ColScheme,
                            (*pData).Col,
                            &((*pData).Scale));

    else if (!strcmp ((*pData).ColDataType, "ori"))
      neut_data_ori_color ((*pData).ColData, (*pData).Qty, Tess.CellCrySym,
                           (*pData).ColScheme, (*pData).Col);

    else if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
     || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
      ut_print_exprbug ((*pData).ColDataType);
  }

  // by default, we color the cells and crystals based on their ids
  else if ((*pData).Dim == Tess.Dim || (*pData).Dim == 5)
  {
    (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, 1);
    if (!Tess.CellId)
      for (i = 1; i <= (*pData).Qty; i++)
        (*pData).ColData[i][0] = i;
    else
      for (i = 1; i <= (*pData).Qty; i++)
        (*pData).ColData[i][0] = Tess.CellId[i];
    neut_data_int_color ((*pData).ColData, (*pData).Qty, (*pData).VoidCol,
                        (*pData).Col);

    ut_string_string ("int", &(*pData).ColDataType);
  }

  if (dim == 5 && !(*pData).BCol)
      (*pData).BCol = ut_alloc_1d_int (3);

  if ((*pData).RadData)
    neut_data_real_real ((*pData).RadData, (*pData).Qty,
                         (*pData).Rad);
  else if (dim == 5)
    for (i = 1; i <= Tess.CellQty; i++)
      neut_tess_cell_radeq (Tess, i, (*pData).Rad + i);

  if ((*pData).WeightData)
    neut_data_real_real ((*pData).WeightData, (*pData).Qty,
                         (*pData).Weight);

  if ((*pData).TrsData)
    neut_data_real_real ((*pData).TrsData, (*pData).Qty, (*pData).Trs);

  if (dim == 5 && (*pData).BRad < 0)
  {
    neut_tess_cellavradeq (Tess, Tess.CellQty, &(*pData).BRad);
    (*pData).BRad *= 0.01;
  }

  if (dim == Tess.Dim)
  {
    if ((*pData).BRad == -1)
      (*pData).BRad = 0.01;

    if (!(*pData).BCol)
      (*pData).BCol = ut_alloc_1d_int (3);
  }

  neut_data_symboldata_symbol (pData);

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
    else if (dim >= 3)
      ut_array_1d_set_3 (coo, 0, 0, 1);
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
