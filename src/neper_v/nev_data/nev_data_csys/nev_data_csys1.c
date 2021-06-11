/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_csys_.h"

void
nev_data_csys (struct SIM Sim, char *attribute, char *datastring, struct DATA *pData)
{
  char *datatype = NULL, *datavalue = NULL;

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", "csys", attribute);

  neut_data_datastring_type_value ("csys", attribute, datastring, &datatype, &datavalue);

  if (!strcmp (attribute, "col"))
    neut_data_fscanf_col (Sim, NULL, NULL, NULL, NULL, "csys", NULL, 0, 1,
                              datatype, datavalue, pData);

  else if (!strcmp (attribute, "rad"))
    neut_data_fscanf_rad (1, datatype, datavalue, pData);

  else if (!strcmp (attribute, "length"))
    neut_data_fscanf_length (1, datatype, datavalue, pData);

  else if (!strcmp (attribute, "coo"))
  {
    ut_string_string ("none", &(*pData).CooDataType);
    (*pData).CooData = ut_alloc_1d_pdouble (2);
    ut_list_break_double (datavalue, NEUT_SEP_DEP, (*pData).CooData + 1, NULL);
  }

  else if (!strcmp (attribute, "label"))
    neut_data_fscanf_label (datavalue, pData);

  else if (!strcmp (attribute, "fontsize"))
    (*pData).FontSize = atof (datavalue);

  else
    ut_print_exprbug (attribute);

  ut_free_1d_char (&datatype);
  ut_free_1d_char (&datavalue);

  return;
}
