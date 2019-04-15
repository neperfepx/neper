/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_stat_.h"

void
net_tess_opt_comp_objective_fval_gen_stat_alloc (struct TOPT *pTOpt, int var)
{
  ut_fct_set_numerical ((*pTOpt).curpdf + var,
			(*pTOpt).tarpdf[var].min,
			(*pTOpt).tarpdf[var].max,
			(*pTOpt).tarpdf[var].size, NULL);
  ut_fct_set_numerical ((*pTOpt).curcdf0 + var,
			(*pTOpt).tarpdf[var].min,
			(*pTOpt).tarpdf[var].max,
			(*pTOpt).tarpdf[var].size, NULL);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_stat_smoothed_comp (struct TOPT *pTOpt, int var)
{
  int i, j;
  double val, deltax;

  ut_fct_set_numerical ((*pTOpt).curcdf + var,
			(*pTOpt).tarcdf[var].min,
			(*pTOpt).tarcdf[var].max,
			(*pTOpt).tarcdf[var].size, NULL);

  ut_array_1d_zero ((*pTOpt).curcdf[var].y,
		    (*pTOpt).curcdf[var].size);

  (*pTOpt).curpenalty[var]  = 0;
  for (j = 1; j <= (*pTOpt).CellQty; j++)
  {
    if ((*pTOpt).curcellpenalty[j] == 0)
      for (i = 0; i < (*pTOpt).curcdf[var].size; i++)
      {
	deltax = (*pTOpt).curcdf[var].x[i] - (*pTOpt).curcellval[var][j][0];
	val = 1 + erf (deltax / (OL_S2 * (*pTOpt).cvlsig[var]));
	(*pTOpt).curcdf[var].y[i] += val;
      }
    else
      (*pTOpt).curpenalty[var] += (*pTOpt).curcellpenalty[j];
  }

  ut_array_1d_scale ((*pTOpt).curcdf[var].y, (*pTOpt).curcdf[var].size,
      1. / (2 * (*pTOpt).CellQty));

  return;
}

void
net_tess_opt_comp_objective_fval_gen_stat_smoothed_update (struct TOPT *pTOpt, int var)
{
  int i, cell;

  for (i = 0; i < (*pTOpt).cellchangedqty; i++)
  {
    cell = (*pTOpt).cellchanged[i];

    net_tess_opt_comp_objective_fval_gen_stat_smoothed_update_cell (pTOpt, var, cell);
  }

  return;
}

void
net_tess_opt_comp_objective_fval_gen_stat_smoothed_gen_legacy (struct TOPT *pTOpt, int var)
{
  int i, j, posmin, posmax;
  double xmin, xmax;

  ut_array_1d_zero ((*pTOpt).curpdf[var].y,
		    (*pTOpt).curpdf[var].size);

  for (j = 1; j <= (*pTOpt).CellQty; j++)
  {
    if ((*pTOpt).curcellpenalty[j] == 0)
    {
      xmin = (*pTOpt).curcellval[var][j][0] + (*pTOpt).cvl[var].min;
      xmax = (*pTOpt).curcellval[var][j][0] + (*pTOpt).cvl[var].max;
      posmin = ut_fct_x_pos ((*pTOpt).curpdf[var], xmin);
      posmax = ut_fct_x_pos ((*pTOpt).curpdf[var], xmax);
      if (posmin < 1 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        abort ();

#pragma omp parallel for private(i) schedule(dynamic)
      for (i = posmin - 1; i <= posmax + 1; i++)
	(*pTOpt).curpdf[var].y[i] +=
	  ut_fct_eval ((*pTOpt).cvl[var], (*pTOpt).curpdf[var].x[i] -
		       (*pTOpt).curcellval[var][j][0]) / (*pTOpt).CellQty;
    }
    else
      (*pTOpt).curpenalty[var] += (*pTOpt).curcellpenalty[j];
  }

  ut_fct_integralfct ((*pTOpt).curpdf[var], (*pTOpt).curcdf + var);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_stat_smoothed_update_legacy (struct TOPT *pTOpt, int var)
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
      posmin = ut_fct_x_pos ((*pTOpt).curpdf[var], xmin);
      posmax = ut_fct_x_pos ((*pTOpt).curpdf[var], xmax);
      if (posmin < 1 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        abort ();

#pragma omp parallel for private(i) schedule(dynamic)
      for (i = posmin - 1; i <= posmax + 1; i++)
	(*pTOpt).curpdf[var].y[i] -=
          ut_fct_eval ((*pTOpt).cvl[var], (*pTOpt).curpdf[var].x[i] -
			   (*pTOpt).oldcellval[var][cell][0])
			  / (*pTOpt).CellQty;
    }
    (*pTOpt).curpenalty[var] -= (*pTOpt).oldcellpenalty[cell];

    // adding new contribution to curpdf
    if ((*pTOpt).curcellpenalty[cell] == 0)
    {
      xmin = (*pTOpt).curcellval[var][cell][0] + (*pTOpt).cvl[var].min;
      xmax = (*pTOpt).curcellval[var][cell][0] + (*pTOpt).cvl[var].max;
      posmin = ut_fct_x_pos ((*pTOpt).curpdf[var], xmin);
      posmax = ut_fct_x_pos ((*pTOpt).curpdf[var], xmax);
      if (posmin < 1 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        abort ();

#pragma omp parallel for private(i) schedule(dynamic)
      for (i = posmin - 1; i <= posmax + 1; i++)
	(*pTOpt).curpdf[var].y[i] +=
          ut_fct_eval ((*pTOpt).cvl[var], (*pTOpt).curpdf[var].x[i] -
			   (*pTOpt).curcellval[var][cell][0])
			  / (*pTOpt).CellQty;
    }
    (*pTOpt).curpenalty[var] += (*pTOpt).curcellpenalty[cell];
  }

  ut_fct_integralfct ((*pTOpt).curpdf[var], (*pTOpt).curcdf + var);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_stat_unsmoothed_comp (struct TOPT *pTOpt, int var)
{
  int i, j;

  ut_array_1d_zero ((*pTOpt).curcdf0[var].y,
		    (*pTOpt).curcdf0[var].size);
  for (j = 1; j <= (*pTOpt).CellQty; j++)
    if ((*pTOpt).curcellpenalty[j] == 0)
      for (i = 0; i < (*pTOpt).curcdf0[var].size; i++)
      {
	if ((*pTOpt).curcdf0[var].x[i] > (*pTOpt).curcellval[var][j][0])
	  (*pTOpt).curcdf0[var].y[i] += 1. / (*pTOpt).CellQty;
      }

  if ((*pTOpt).cvlsig[var] == 0)
    ut_fct_memcpy ((*pTOpt).curcdf0[var], (*pTOpt).curcdf + var);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_stat_evaluate (struct TOPT *pTOpt, int var)
{
  if (!strcmp ((*pTOpt).tarobjective[var], "default")
        || !strcmp ((*pTOpt).tarobjective[var], "FL2w"))
    net_tess_opt_comp_objective_fval_gen_stat_evaluate_FL2w (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "FL2wu"))
    net_tess_opt_comp_objective_fval_gen_stat_evaluate_FL2wu (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "ad"))
    net_tess_opt_comp_objective_fval_gen_stat_evaluate_ad (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "cvm"))
    net_tess_opt_comp_objective_fval_gen_stat_evaluate_cvm (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "FL2"))
    net_tess_opt_comp_objective_fval_gen_stat_evaluate_FL2 (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "chi2"))
    net_tess_opt_comp_objective_fval_gen_stat_evaluate_chi2 (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "ks"))
    net_tess_opt_comp_objective_fval_gen_stat_evaluate_ks (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "kuiper"))
    net_tess_opt_comp_objective_fval_gen_stat_evaluate_kuiper (pTOpt, var);

  else
    ut_print_message (2, 3, "Unknown test `%s'.", (*pTOpt).tarobjective[var]);

  if ((*pTOpt).curpenalty[var] > 1e-6)
    (*pTOpt).curval[var] += 1000 * (*pTOpt).curpenalty[var];

  return;
}
