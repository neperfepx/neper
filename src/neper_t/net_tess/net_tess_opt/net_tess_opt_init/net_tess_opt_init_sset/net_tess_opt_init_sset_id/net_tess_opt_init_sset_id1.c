/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_id_.h"

void
net_tess_opt_init_sset_id (int TessId, struct SEEDSET *SSet, char* idexpr, struct TOPT *pTOpt)
{
  (void) TessId;
  (void) SSet;

  if (ut_string_isfilename (idexpr))
  {
    (*pTOpt).SSet.SeedId = ut_alloc_1d_int ((*pTOpt).CellQty);
    ut_array_1d_int_fnscanf (idexpr, (*pTOpt).SSet.SeedId + 1, (*pTOpt).CellQty, "r");
  }

  return;
}
