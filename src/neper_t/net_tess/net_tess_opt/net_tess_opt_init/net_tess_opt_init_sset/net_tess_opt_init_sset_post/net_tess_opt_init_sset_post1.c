/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_post_.h"

void
net_tess_opt_init_sset_post (struct IN_T In, int level, struct TESS PTess,
			     int cell, struct SEEDSET SSet,
			     struct TOPT *pTOpt)
{
  int i, j, scell;

  // orientations
  (*pTOpt).SSet.q = ut_alloc_2d ((*pTOpt).SSet.N + 1, 4);
  for (i = 1; i <= (*pTOpt).CellQty; i++)
    for (j = 0; j < (*pTOpt).CellSCellQty[i]; j++)
    {
      scell = (*pTOpt).CellSCellList[i][j];
      ut_array_1d_memcpy ((*pTOpt).SSet.q[scell], 4, SSet.q[i]);
    }

  // periodicity
  net_tess_opt_init_sset_post_per (In.periodic, PTess, cell, &(*pTOpt).SSet);

  // log
  net_tess_opt_init_sset_post_log (In, level, pTOpt);

  return;
}
