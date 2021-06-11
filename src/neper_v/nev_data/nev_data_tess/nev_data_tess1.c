/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_tess_.h"

void
nev_data_tess (struct SIM Sim, struct TESS *pTess, char *entity_in, char *attribute,
                     char *datastring, struct DATA *TessData)
{
  int dim, entityqty;
  struct DATA *pData = NULL;
  char *entity = NULL, *datatype = NULL, *datavalue = NULL;

  ut_string_string (entity_in, &entity);

  pData = nev_data_tess_init (*pTess, TessData, &entity, attribute,
                                     datastring, &dim, &entityqty, &datatype,
                                     &datavalue);

  if (!strcmp (attribute, "col"))
    neut_data_fscanf_col (Sim, pTess, NULL, NULL, NULL, "tess", entity, dim, entityqty,
                              datatype, datavalue, pData);

  else if (!strcmp (attribute, "trs"))
    neut_data_fscanf_trs (entityqty, datatype, datavalue, pData);

  else if (!strcmp (attribute, "rad"))
    neut_data_fscanf_rad (entityqty, datatype, datavalue, pData);

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
  ut_free_1d_char (&entity);

  return;
}
