/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_post_.h"

void
net_tess_opt_init_sset_post (struct IN_T In, int level, struct TESS PTess,
                             int cell, struct TOPT *pTOpt)
{
  /*
  int i, j, scell;

  // orientations
  (*pTOpt).SSet.SeedOri = ut_alloc_2d ((*pTOpt).SSet.N + 1, 4);
  for (i = 1; i <= (*pTOpt).CellQty; i++)
    for (j = 0; j < (*pTOpt).CellSCellQty[i]; j++)
    {
      scell = (*pTOpt).CellSCellList[i][j];
      ut_array_1d_memcpy (SSet.SeedOri[i], 4, (*pTOpt).SSet.SeedOri[scell]);
    }

  // orientation spreads
  (*pTOpt).SSet.SeedOriDistrib = ut_alloc_1d_pchar ((*pTOpt).SSet.N + 1);
  for (i = 1; i <= (*pTOpt).CellQty; i++)
    for (j = 0; j < (*pTOpt).CellSCellQty[i]; j++)
    {
      scell = (*pTOpt).CellSCellList[i][j];
      ut_string_string (SSet.SeedOriDistrib[i], (*pTOpt).SSet.SeedOriDistrib + scell);
    }
    */

  // periodicity
  net_tess_opt_init_sset_post_per (In.periodic, PTess.Level, (*pTOpt).Dom,
                                   cell, &(*pTOpt).SSet);

  // log
  net_tess_opt_init_sset_post_log (In, level, pTOpt);

  return;
}
