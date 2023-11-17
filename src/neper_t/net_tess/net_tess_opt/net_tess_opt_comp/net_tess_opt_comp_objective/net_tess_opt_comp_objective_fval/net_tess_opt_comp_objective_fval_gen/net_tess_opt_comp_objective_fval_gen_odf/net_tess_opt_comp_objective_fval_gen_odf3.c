/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_odf_.h"

void
net_tess_opt_comp_objective_fval_gen_odf_smoothed_update_cell (struct TOPT
                                                                *pTOpt,
                                                                int var,
                                                                int cell)
{
  int i, minpos, maxpos;
  double val, min, max;

  if ((*pTOpt).oldcellpenalty[cell] == 0
      && (*pTOpt).curcellpenalty[cell] == 0)
  {
    min =
      ut_num_min ((*pTOpt).oldcellval[var][cell][0],
                  (*pTOpt).curcellval[var][cell][0]) -
      5 * (*pTOpt).cvlsig[var];

    minpos = ut_fct_x_bin ((*pTOpt).curcdf[var], min);

    if (minpos < 1)
      abort ();

    minpos--;

    max =
      ut_num_max ((*pTOpt).oldcellval[var][cell][0],
                  (*pTOpt).curcellval[var][cell][0]) +
      5 * (*pTOpt).cvlsig[var];

    maxpos = ut_fct_x_bin ((*pTOpt).curcdf[var], max);

    if (maxpos > (*pTOpt).curcdf[var].size - 1)
      abort ();

    maxpos++;

    for (i = minpos; i < maxpos; i++)
      // for (i = 0; i < (*pTOpt).curcdf[var].size; i++)
    {
      val =
        erf (((*pTOpt).curcdf[var].x[i] -
              (*pTOpt).curcellval[var][cell][0]) / (OL_S2 *
                                                    (*pTOpt).cvlsig[var])) -
        erf (((*pTOpt).curcdf[var].x[i] -
              (*pTOpt).oldcellval[var][cell][0]) / (OL_S2 *
                                                    (*pTOpt).cvlsig[var]));

      (*pTOpt).curcdf[var].y[i] += val / (2 * (*pTOpt).CellQty);
    }
  }

  else
  {
    (*pTOpt).curpenalty[var] -= (*pTOpt).oldcellpenalty[cell];
    (*pTOpt).curpenalty[var] += (*pTOpt).curcellpenalty[cell];
  }

  return;
}

// chi2
void
net_tess_opt_comp_objective_fval_gen_odf_evaluate_chi2 (struct TOPT *pTOpt,
                                                         int var)
{
  int i;
  double val = 0;

  for (i = 0; i < (*pTOpt).tarpdf[var].size; i++)
  {
    if ((*pTOpt).tarpdf[var].y[i] > 0)
      val +=
        pow ((*pTOpt).curpdf[var].y[i] - (*pTOpt).tarpdf[var].y[i],
             2) / (*pTOpt).tarpdf[var].y[i];
    else
      val +=
        pow ((*pTOpt).curpdf[var].y[i] - (*pTOpt).tarpdf[var].y[i],
             2) / (.5 * ((*pTOpt).curpdf[var].y[i] +
                         (*pTOpt).tarpdf[var].y[i]));
  }

  (*pTOpt).curval[var] = val;

  return;
}
