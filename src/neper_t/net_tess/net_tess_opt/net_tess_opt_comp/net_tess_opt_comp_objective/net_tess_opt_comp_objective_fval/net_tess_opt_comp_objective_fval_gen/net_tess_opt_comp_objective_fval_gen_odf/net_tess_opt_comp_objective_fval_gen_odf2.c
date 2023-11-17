/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_odf_.h"

void
net_tess_opt_comp_objective_fval_gen_odf_smoothed (struct TOPT
                                                               *pTOpt,
                                                               int var)
{
  int i, j, posmin, posmax;
  double xmin, xmax;

  ut_array_1d_zero ((*pTOpt).curpdf[var].y, (*pTOpt).curpdf[var].size);

  (*pTOpt).curpenalty[var] = 0;
  for (j = 1; j <= (*pTOpt).CellQty; j++)
  {
    if ((*pTOpt).curcellpenalty[j] == 0)
    {
      xmin = (*pTOpt).curcellval[var][j][0] + (*pTOpt).cvl[var].min;
      xmax = (*pTOpt).curcellval[var][j][0] + (*pTOpt).cvl[var].max;
      posmin = ut_fct_x_bin ((*pTOpt).curpdf[var], xmin);
      posmax = ut_fct_x_bin ((*pTOpt).curpdf[var], xmax);
      if (posmin < 1 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        abort ();

#pragma omp parallel for private(i) schedule(dynamic)
      for (i = posmin - 1; i <= posmax + 1; i++)
        (*pTOpt).curpdf[var].y[i] +=
          ut_fct_eval ((*pTOpt).cvl[var],
                       (*pTOpt).curpdf[var].x[i] -
                       (*pTOpt).curcellval[var][j][0]) / (*pTOpt).CellQty;
    }
    else
      (*pTOpt).curpenalty[var] += (*pTOpt).curcellpenalty[j];
  }

  ut_fct_integralfct ((*pTOpt).curpdf[var], (*pTOpt).curcdf + var);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_odf_smoothed_update (struct TOPT *pTOpt, int var)
{
  int i, j, cell, posmin, posmax;
  double xmin, xmax;

  for (j = 0; j < (*pTOpt).cellchangedqty; j++)
  {
    cell = (*pTOpt).cellchanged[j];

    // removing old contribution to curpdf
    if ((*pTOpt).oldcellpenalty[cell] == 0)
    {
      xmin = (*pTOpt).oldcellval[var][cell][0] + (*pTOpt).cvl[var].min;
      xmax = (*pTOpt).oldcellval[var][cell][0] + (*pTOpt).cvl[var].max;
      posmin = ut_fct_x_bin ((*pTOpt).curpdf[var], xmin);
      posmax = ut_fct_x_bin ((*pTOpt).curpdf[var], xmax);
      if (posmin < 1 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        abort ();

#pragma omp parallel for private(i) schedule(dynamic)
      for (i = posmin - 1; i <= posmax + 1; i++)
        (*pTOpt).curpdf[var].y[i] -=
          ut_fct_eval ((*pTOpt).cvl[var],
                       (*pTOpt).curpdf[var].x[i] -
                       (*pTOpt).oldcellval[var][cell][0]) / (*pTOpt).CellQty;
    }
    (*pTOpt).curpenalty[var] -= (*pTOpt).oldcellpenalty[cell];

    // adding new contribution to curpdf
    if ((*pTOpt).curcellpenalty[cell] == 0)
    {
      xmin = (*pTOpt).curcellval[var][cell][0] + (*pTOpt).cvl[var].min;
      xmax = (*pTOpt).curcellval[var][cell][0] + (*pTOpt).cvl[var].max;
      posmin = ut_fct_x_bin ((*pTOpt).curpdf[var], xmin);
      posmax = ut_fct_x_bin ((*pTOpt).curpdf[var], xmax);
      if (posmin < 1 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        abort ();

#pragma omp parallel for private(i) schedule(dynamic)
      for (i = posmin - 1; i <= posmax + 1; i++)
        (*pTOpt).curpdf[var].y[i] +=
          ut_fct_eval ((*pTOpt).cvl[var],
                       (*pTOpt).curpdf[var].x[i] -
                       (*pTOpt).curcellval[var][cell][0]) / (*pTOpt).CellQty;
    }
    (*pTOpt).curpenalty[var] += (*pTOpt).curcellpenalty[cell];
  }

  ut_fct_integralfct ((*pTOpt).curpdf[var], (*pTOpt).curcdf + var);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_odf_evaluate (struct TOPT *pTOpt, struct ODF Odf,
                                                    int var)
{
  int i;

  if (!strcmp ((*pTOpt).tarobjective[var], "default"))
  {
    // net_tess_opt_comp_objective_fval_gen_odf_evaluate_chi2 (pTOpt, var);

    (*pTOpt).curval[var] = 0;
    for (i = 0; i < (*pTOpt).Odf.odfqty; i++)
      (*pTOpt).curval[var] += Odf.EltWeight[i] * pow ((*pTOpt).Odf.odf[i] - Odf.odf[i], 2);
    (*pTOpt).curval[var] /= ut_array_1d_sum (Odf.EltWeight, (*pTOpt).Odf.odfqty);
    if (isnan ((*pTOpt).curval[var]))
      (*pTOpt).curval[var] = 1000;
    (*pTOpt).curval[var] = sqrt ((*pTOpt).curval[var]);
  }

  else
    ut_print_message (2, 3, "Unknown test `%s'.", (*pTOpt).tarobjective[var]);

  if ((*pTOpt).curpenalty[var] > 1e-6)
    (*pTOpt).curval[var] += 1000 * (*pTOpt).curpenalty[var];

  return;
}
