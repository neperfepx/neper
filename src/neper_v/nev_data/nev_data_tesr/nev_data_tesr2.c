/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_tesr_.h"

void
nev_data_tesr_cell (struct SIM Sim, struct TESR *pTesr, struct DATA *pData, char *attribute,
                          char *datastring)
{
  int argqty;
  char **args = NULL;
  char *datatype = NULL, *datavalue = NULL;

  neut_data_datastring_type_value ("cell", attribute, datastring, &datatype, &datavalue);

  ut_list_break (datastring, NEUT_SEP_DEP, &args, &argqty);

  if (!strcmp (attribute, "col"))
    neut_data_fscanf_col (Sim, pTesr, NULL, NULL, NULL, "tesr", "cell", (*pTesr).Dim,
                         (*pTesr).CellQty, datatype, datavalue, pData);

  else if (!strcmp (attribute, "trs"))
    neut_data_fscanf_trs ((*pTesr).CellQty, datatype, datavalue, pData);

  else if (!strcmp (attribute, "colscheme"))
    ut_string_string (datastring, &(*pData).ColScheme);

  else if (!strcmp (attribute, "scale"))
    ut_string_string (datastring, &(*pData).Scale);

  else if (!strcmp (attribute, "scaletitle"))
    ut_string_string (datastring, &(*pData).ScaleTitle);

  else
    ut_print_exprbug (attribute);

  ut_free_2d_char (&args, argqty);
  ut_free_1d_char (&datatype);
  ut_free_1d_char (&datavalue);

  return;
}

void
nev_data_tesr_vox (struct SIM Sim, struct TESR *pTesr, struct DATA *pData, char *attribute,
                         char *datastring)
{
  char *datatype = NULL, *datavalue = NULL;

  neut_data_datastring_type_value ("vox", attribute, datastring, &datatype, &datavalue);

  if (!strcmp (attribute, "col"))
    neut_data_fscanf_col (Sim, pTesr, NULL, NULL, NULL, "tesr", "vox", (*pTesr).Dim,
                         neut_tesr_totvoxqty (*pTesr), datatype, datavalue, pData);

  else if (!strcmp (attribute, "colscheme"))
    ut_string_string (datastring, &(*pData).ColScheme);

  else if (!strcmp (attribute, "scale"))
    ut_string_string (datastring, &(*pData).Scale);

  else if (!strcmp (attribute, "scaletitle"))
    ut_string_string (datastring, &(*pData).ScaleTitle);

  else
    ut_print_exprbug (attribute);

  ut_free_1d_char (&datatype);
  ut_free_1d_char (&datavalue);

  return;
}

void
nev_data_tesr_edge (struct DATA *pData, char *attribute, char *datastring)
{
  if (!strcmp (attribute, "rad"))
  {
    ut_string_string ("expr", &(*pData).RadDataType);
    ut_string_string (datastring, &(*pData).RadDataName);
  }
  else
    ut_print_exprbug (attribute);

  return;
}


void
nev_data_tesr_voxedge (struct DATA *pData, char *attribute, char *datastring)
{
  if (!strcmp (attribute, "col"))
  {
    (*pData).BCol = ut_alloc_1d_int (3);
    ut_array_1d_int_fnscanf_wcard (datastring, (*pData).BCol, 3, "color", "r");
  }
  else if (!strcmp (attribute, "rad"))
    (*pData).BRad = atof (datastring);
  else
    ut_print_exprbug (attribute);

  return;
}

void
nev_data_tesr_voidvox (struct DATA *pData, char *attribute, char *datastring)
{
  if (!strcmp (attribute, "col"))
  {
    (*pData).VoidCol = ut_alloc_1d_int (3);
    ut_array_1d_int_fnscanf_wcard (datastring, (*pData).VoidCol, 3, "color", "r");
  }
  else
    ut_print_exprbug (attribute);

  return;
}

void
nev_data_tesr_cell2vox (struct TESR Tesr, char *attribute,
                       struct DATA TesrDataCell, struct DATA *pData)
{
  int i, j, k, id, size, vox;

  (*pData).ColDataDef = ut_alloc_1d_int ((*pData).Qty + 1);
  id = 0;
  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
        (*pData).ColDataDef[++id] = (Tesr.VoxCell[i][j][k] > 0);

  ut_string_string ("cell", &(*pData).type);

  if (!strcmp (attribute, "col"))
  {
    ut_string_string (TesrDataCell.ColDataType, &(*pData).ColDataType);

    size = -1;
    if (!strcmp ((*pData).ColDataType, "int"))
      size = 1;
    else if (!strcmp ((*pData).ColDataType, "col")
             || !strncmp ((*pData).ColDataType, "ori", 3))
      size = 3;
    else if (!strcmp ((*pData).ColDataType, "rad"))
      size = 1;
    else if (!strcmp ((*pData).ColDataType, "real"))
      size = 1;
    else
      abort ();

    (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, size);
    vox = 0;
    for (k = 1; k <= Tesr.size[2]; k++)
      for (j = 1; j <= Tesr.size[1]; j++)
        for (i = 1; i <= Tesr.size[0]; i++)
        {
          vox++;
          (*pData).ColData[vox] = TesrDataCell.ColData[Tesr.VoxCell[i][j][k]];
        }
  }
  else if (!strcmp (attribute, "colscheme"))
    ut_string_string (TesrDataCell.ColScheme, &(*pData).ColScheme);
  else if (!strcmp (attribute, "scale"))
    ut_string_string (TesrDataCell.Scale, &(*pData).Scale);
  else if (!strcmp (attribute, "scaletitle"))
    ut_string_string (TesrDataCell.ScaleTitle, &(*pData).ScaleTitle);
  else
    ut_print_exprbug (attribute);

  return;
}
