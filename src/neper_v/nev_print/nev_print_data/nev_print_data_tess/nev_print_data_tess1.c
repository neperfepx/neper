/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_tess_.h"

void
nev_print_data_tess (struct SIM Sim, struct TESS *pTess, char *entity, char *attribute,
                     char *datastring, struct DATA *TessData)
{
  int dim, entityqty;
  struct DATA *pData = NULL;
  char *datatype = NULL, *datavalue = NULL;

  struct DATAINPUT DataInput;
  neut_datainput_set_default (&DataInput);
  ut_string_string ("tess", &DataInput.input);
  DataInput.pSim = &Sim;
  DataInput.pTess = pTess;

  nev_print_data_tess_init (*pTess, TessData, entity, attribute, datastring,
                            &dim, &entityqty, &datatype, &datavalue, &pData);

  // radius: `default' variable
  if (!strcmp (attribute, "rad") && strcmp (entity, "celledge")
      && strcmp (entity, "crystaledge") && datavalue)
  {
    double defrad;
    char *tmp = NULL;

    nev_print_tess_defrad (*pTess, dim, &defrad);
    nev_print_rad_default (datavalue, defrad, &tmp);
    ut_string_string (tmp, &datavalue);
    ut_free_1d_char (&tmp);
  }

  if (!strcmp (attribute, ""))
    ut_string_string (datastring, &(*pData).Value);

  else if (!strcmp (attribute, "colscheme"))
    ut_string_string (datastring, &(*pData).ColScheme);

  else if (!strcmp (attribute, "scale"))
    ut_string_string (datastring, &(*pData).Scale);

  else if (!strcmp (attribute, "scaletitle"))
    ut_string_string (datastring, &(*pData).ScaleTitle);

  else if (!strcmp (entity, "crystaledge"))
  {
    if (!strcmp (attribute, "col"))
    {
      TessData[5].BCol = ut_alloc_1d_int (3);
      ut_array_1d_int_fnscanf_wcard (datavalue, TessData[5].BCol, 3, "color", "r");
    }
    else if (!strcmp (attribute, "rad"))
      TessData[5].BRad = atof (datavalue);
    else
      ut_print_exprbug (attribute);
  }

  else if (!strcmp (entity, "celledge"))
  {
    if (!strcmp (attribute, "col"))
    {
      TessData[dim].BCol = ut_alloc_1d_int (3);
      ut_array_1d_int_fnscanf_wcard (datavalue, TessData[dim].BCol, 3, "color", "r");
    }
    else if (!strcmp (attribute, "rad"))
      TessData[dim].BRad = atof (datavalue);
    else
      ut_print_exprbug (attribute);
  }

  else
    neut_data_fscanf_general (DataInput, entity, dim, entityqty, attribute, datatype,
                          datavalue, pData);

  ut_free_1d_char (&datatype);
  ut_free_1d_char (&datavalue);

  neut_datainput_free (&DataInput);

  return;
}
