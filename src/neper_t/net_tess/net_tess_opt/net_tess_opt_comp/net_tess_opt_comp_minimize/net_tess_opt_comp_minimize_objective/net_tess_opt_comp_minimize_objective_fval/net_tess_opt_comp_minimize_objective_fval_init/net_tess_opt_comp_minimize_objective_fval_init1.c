/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_fval_init_.h"

void
net_tess_opt_comp_minimize_objective_fval_init (struct TOPT *pTOpt)
{
  int i, j, cell;

  if ((*pTOpt).iter == 0)
    return;

  for (i = 0; i < (*pTOpt).tarqty; i++)
    for (j = 0; j < (*pTOpt).cellchangedqty; j++)
    {
      cell = (*pTOpt).cellchanged[j];

      (*pTOpt).oldcellpenalty[cell] = (*pTOpt).curcellpenalty[cell];
      (*pTOpt).oldcellweight[i][cell] = (*pTOpt).curcellweight[i][cell];

      ut_array_1d_memcpy ((*pTOpt).curcellval[i][cell],
                          (*pTOpt).tarcellvalqty[i],
                          (*pTOpt).oldcellval[i][cell]);

      if ((*pTOpt).curcellcdf[i])
        ut_fct_memcpy ((*pTOpt).curcellcdf[i][cell], (*pTOpt).oldcellcdf[i] + cell);
    }

  return;
}
