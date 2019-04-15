/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_init_.h"

void
net_tess_opt_comp_objective_fval_init (struct TOPT *pTOpt)
{
  int i, j, cell;

  if ((*pTOpt).iter <= 1)
    return;

  for (i = 0; i < (*pTOpt).tarqty; i++)
    for (j = 0; j < (*pTOpt).cellchangedqty; j++)
    {
      cell = (*pTOpt).cellchanged[j];
      (*pTOpt).oldcellpenalty[cell] = (*pTOpt).curcellpenalty[cell];
      ut_array_1d_memcpy ((*pTOpt).oldcellval[i][cell],
			  (*pTOpt).tarcellvalqty[i],
			  (*pTOpt).curcellval[i][cell]);
    }

  return;
}
