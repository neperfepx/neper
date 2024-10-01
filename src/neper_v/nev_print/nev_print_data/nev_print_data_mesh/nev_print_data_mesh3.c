/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_mesh_.h"

void
nev_print_data_mesh_elt_rad (char *datastring, struct DATA *pData)
{
  (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 1);
  ut_array_2d_fnscanf_wcard (datastring, (*pData).RadData + 1, (*pData).Qty, 1,
                             "numeral,size", "r");

  return;
}
