/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_tess_.h"

void
nev_print_data_tess_init (struct TESS Tess, struct DATA *TessData,
                          char *entity, char *attribute,
                          char *datastring, int *pdim, int *pentityqty,
                          char **ptype, char **pvalue, struct DATA **ppData)
{
  ut_print_message (0, 1, "Reading data (%s%s%s)...\n", entity, strlen (attribute) ? ", " : "", attribute);

  neut_tess_entity_dim (Tess, entity, pdim);

  neut_tess_entity_qty (Tess, entity, pentityqty);

  if (*pdim < 0 || *pentityqty < 0)
    abort ();

  neut_data_datastring_type_value (entity, attribute, datastring, ptype, pvalue);

  (*ppData) = TessData + (*pdim);

  return;
}

void
nev_print_data_tess_celledge (struct DATA *pData, char *attribute, char *datastring)
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
