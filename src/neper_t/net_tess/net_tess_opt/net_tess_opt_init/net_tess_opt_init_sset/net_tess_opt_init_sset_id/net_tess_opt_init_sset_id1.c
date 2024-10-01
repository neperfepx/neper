/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_id_.h"

void
net_tess_opt_init_sset_id (int TessId, struct SEEDSET *SSet, char* idexpr, struct TOPT *pTOpt)
{
  (void) TessId;
  (void) SSet;
  FILE *fp = NULL;
  struct TESS Tmp;

  if (ut_string_isfilename (idexpr))
  {
    if (ut_file_testformat (idexpr, "tess"))
    {
      neut_tess_set_zero (&Tmp);
      fp = ut_file_open (idexpr, "R");
      neut_tess_fscanf (fp, &Tmp);
      if (!Tmp.CellId)
      {
        Tmp.CellId = ut_alloc_1d_int (Tmp.CellQty + 1);
        ut_array_1d_int_set_id (Tmp.CellId, Tmp.CellQty + 1);
      }
      (*pTOpt).SSet.SeedId = ut_alloc_1d_int ((*pTOpt).CellQty + 1);
      ut_array_1d_int_memcpy (Tmp.CellId + 1, (*pTOpt).CellQty, (*pTOpt).SSet.SeedId + 1);
      ut_file_close (fp, idexpr, "R");
      neut_tess_free (&Tmp);
    }
    else
    {
      (*pTOpt).SSet.SeedId = ut_alloc_1d_int ((*pTOpt).CellQty);
      ut_array_1d_int_fnscanf (idexpr, (*pTOpt).SSet.SeedId + 1, (*pTOpt).CellQty, "r");
    }
  }

  return;
}
