/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_fval_tar_stat_.h"

void
net_tess_opt_comp_minimize_objective_fval_tar_stat_alloc (struct TOPT *pTOpt, int var)
{
  ut_fct_set_numerical ((*pTOpt).curpdf + var, (*pTOpt).tarpdf[var].min,
                        (*pTOpt).tarpdf[var].max, (*pTOpt).tarpdf[var].size,
                        NULL);
  ut_fct_set_numerical ((*pTOpt).curcdf0 + var, (*pTOpt).tarpdf[var].min,
                        (*pTOpt).tarpdf[var].max, (*pTOpt).tarpdf[var].size,
                        NULL);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_comp (struct TOPT *pTOpt,
                                                                  int var)
{
  int i, j;
  double val, deltax;

  ut_fct_set_numerical ((*pTOpt).curcdf + var, (*pTOpt).tarcdf[var].min,
                        (*pTOpt).tarcdf[var].max, (*pTOpt).tarcdf[var].size,
                        NULL);

  ut_array_1d_zero ((*pTOpt).curcdf[var].y, (*pTOpt).curcdf[var].size);

  for (j = 1; j <= (*pTOpt).CellQty; j++)
    if ((*pTOpt).curcellpenalty[j] == 0)
      for (i = 0; i < (*pTOpt).curcdf[var].size; i++)
      {
        deltax = (*pTOpt).curcdf[var].x[i] - (*pTOpt).curcellval[var][j][0];
        val = 1 + erf (deltax / (OL_S2 * (*pTOpt).cvlsig[var]));
        (*pTOpt).curcdf[var].y[i] += val;
      }

  ut_array_1d_scale ((*pTOpt).curcdf[var].y, (*pTOpt).curcdf[var].size,
                     1. / (2 * (*pTOpt).CellQty));

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_update (struct TOPT *pTOpt,
                                                                    int var)
{
  int i, cell;

  for (i = 0; i < (*pTOpt).cellchangedqty; i++)
  {
    cell = (*pTOpt).cellchanged[i];

    net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_update_cell (pTOpt,
                                                                             var,
                                                                             cell);
  }

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_gen_legacy (struct TOPT
                                                                        *pTOpt,
                                                                        int var)
{
  int i, j, posmin, posmax;
  double xmin, xmax, weight_sum;

  ut_array_1d_zero ((*pTOpt).curpdf[var].y, (*pTOpt).curpdf[var].size);

  weight_sum = 0;
  for (j = 1; j <= (*pTOpt).CellQty; j++)
  {
    if ((*pTOpt).curcellpenalty[j] == 0)
    {
      xmin = (*pTOpt).curcellval[var][j][0] + (*pTOpt).cvl[var].min;
      xmax = (*pTOpt).curcellval[var][j][0] + (*pTOpt).cvl[var].max;
      posmin = ut_fct_x_bin ((*pTOpt).curpdf[var], xmin);
      posmax = ut_fct_x_bin ((*pTOpt).curpdf[var], xmax);
      if (posmin < 1 || posmax < 0 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        ut_print_message (2, 2, "Out-of-range value of `%s' generated for cell %d (%f vs [%f, %f]).  Increase range in -morphooptigrid.\n", \
                          (*pTOpt).tarvar[var], j, (*pTOpt).curcellval[var][j][0], (*pTOpt).curpdf[var].min, (*pTOpt).curpdf[var].max);

      for (i = posmin - 1; i <= posmax + 1; i++)
        (*pTOpt).curpdf[var].y[i] +=
          (*pTOpt).curcellweight[var][j] * ut_fct_eval ((*pTOpt).cvl[var],
                       (*pTOpt).curpdf[var].x[i] -
                       (*pTOpt).curcellval[var][j][0]);
    }

    weight_sum += (*pTOpt).curcellweight[var][j];
  }
  ut_array_1d_scale ((*pTOpt).curpdf[var].y, (*pTOpt).curpdf[var].size, 1. / weight_sum);

  ut_fct_integralfct ((*pTOpt).curpdf[var], (*pTOpt).curcdf + var);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_update_legacy (struct TOPT
                                                                           *pTOpt,
                                                                           int var)
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
      if (posmin < 1 || posmax < 0 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        abort ();

      for (i = posmin - 1; i <= posmax + 1; i++)
        (*pTOpt).curpdf[var].y[i] -=
          ut_fct_eval ((*pTOpt).cvl[var],
                       (*pTOpt).curpdf[var].x[i] -
                       (*pTOpt).oldcellval[var][cell][0]) / (*pTOpt).CellQty;
    }

    // adding new contribution to curpdf
    if ((*pTOpt).curcellpenalty[cell] == 0)
    {
      xmin = (*pTOpt).curcellval[var][cell][0] + (*pTOpt).cvl[var].min;
      xmax = (*pTOpt).curcellval[var][cell][0] + (*pTOpt).cvl[var].max;
      posmin = ut_fct_x_bin ((*pTOpt).curpdf[var], xmin);
      posmax = ut_fct_x_bin ((*pTOpt).curpdf[var], xmax);
      if (posmin < 1 || posmax < 0 || posmax + 1 >= (*pTOpt).curpdf[var].size)
        abort ();

      for (i = posmin - 1; i <= posmax + 1; i++)
        (*pTOpt).curpdf[var].y[i] +=
          ut_fct_eval ((*pTOpt).cvl[var],
                       (*pTOpt).curpdf[var].x[i] -
                       (*pTOpt).curcellval[var][cell][0]) / (*pTOpt).CellQty;
    }
  }

  ut_fct_integralfct ((*pTOpt).curpdf[var], (*pTOpt).curcdf + var);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_stat_unsmoothed_comp (struct TOPT *pTOpt,
                                                                    int var)
{
  int i, j;
  double L_cur;

  ut_array_1d_zero ((*pTOpt).curcdf0[var].y, (*pTOpt).curcdf0[var].size);
  L_cur = 0;
  for (j = 1; j <= (*pTOpt).CellQty; j++)
  {
    L_cur += (*pTOpt).curcellweight[var][j];

    if ((*pTOpt).curcellpenalty[j] == 0)
    {
      if (!(*pTOpt).curcellcdf[var])
      {
        for (i = 0; i < (*pTOpt).curcdf0[var].size; i++)
          if ((*pTOpt).curcdf0[var].x[i] > (*pTOpt).curcellval[var][j][0])
            (*pTOpt).curcdf0[var].y[i] += (*pTOpt).curcellweight[var][j];
      }
      else
      {
        for (i = 0; i < (*pTOpt).curcdf0[var].size; i++)
          if ((*pTOpt).curcdf0[var].x[i] > (*pTOpt).curcellcdf[var][j].from)
            (*pTOpt).curcdf0[var].y[i] += (*pTOpt).curcellweight[var][j] * ut_fct_eval ((*pTOpt).curcellcdf[var][j], (*pTOpt).curcdf0[var].x[i]);
      }
    }
  }
  if (L_cur > 0)
    ut_array_1d_scale ((*pTOpt).curcdf0[var].y, (*pTOpt).curcdf0[var].size, 1. / L_cur);

  ut_fct_init_interp ((*pTOpt).curcdf0 + var);

  /*
  fp = ut_file_open ("curcdf0", "w");
  for (k = 0; k < (*pTOpt).curcdf0[var].size; k++)
    fprintf (fp, "%f %f\n", (*pTOpt).curcdf0[var].x[k], (*pTOpt).curcdf0[var].y[k]);
  ut_file_close (fp, "curcdf0", "w");
  */

  if ((*pTOpt).cvlsig[var] == 0)
    ut_fct_memcpy ((*pTOpt).curcdf0[var], (*pTOpt).curcdf + var);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_stat_unsmoothed_update (struct TOPT *pTOpt, int var)
{
  int i, j, cell;
  double old_L, cur_L;

  old_L = ut_array_1d_sum ((*pTOpt).oldcellweight[var] + 1, (*pTOpt).CellQty);
  cur_L = ut_array_1d_sum ((*pTOpt).curcellweight[var] + 1, (*pTOpt).CellQty);

  // all cells penalized (penalized cells have zero weight), before or now
  if (old_L == 0 || cur_L == 0)
  {
    net_tess_opt_comp_minimize_objective_fval_tar_stat_unsmoothed_comp (pTOpt, var);
    return;
  }

  ut_array_1d_scale ((*pTOpt).curcdf0[var].y, (*pTOpt).curcdf0[var].size, old_L / cur_L);

  /*
  for (j = 1; j <= (*pTOpt).CellQty; j++)
  {
    sprintf (filename, "cell%d.curcdf", j);
    fp = ut_file_open (filename, "W");
    for (i = 0; i < (*pTOpt).curcellcdf[var][j].size; i++)
      fprintf (fp, "%.12f %.12f\n", (*pTOpt).curcellcdf[var][j].x[i], (*pTOpt).curcellcdf[var][j].y[i]);
    ut_file_close (fp, filename, "W");
  }
  */

  for (j = 0; j < (*pTOpt).cellchangedqty; j++)
  {
    cell = (*pTOpt).cellchanged[j];

    if ((*pTOpt).oldcellpenalty[cell] == 0)
    {
      for (i = 0; i < (*pTOpt).curcdf0[var].size; i++)
        if ((*pTOpt).curcdf0[var].x[i] > (*pTOpt).oldcellcdf[var][cell].from)
          (*pTOpt).curcdf0[var].y[i]
            -= (*pTOpt).oldcellweight[var][cell] * ut_fct_eval ((*pTOpt).oldcellcdf[var][cell], (*pTOpt).curcdf0[var].x[i]) / cur_L;
    }

    if ((*pTOpt).curcellpenalty[cell] == 0)
    {
      for (i = 0; i < (*pTOpt).curcdf0[var].size; i++)
        if ((*pTOpt).curcdf0[var].x[i] > (*pTOpt).curcellcdf[var][cell].from)
          (*pTOpt).curcdf0[var].y[i]
            += (*pTOpt).curcellweight[var][cell] * ut_fct_eval ((*pTOpt).curcellcdf[var][cell], (*pTOpt).curcdf0[var].x[i]) / cur_L;
    }
  }

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate (struct TOPT *pTOpt,
                                                             int var)
{
  if (!strcmp ((*pTOpt).tarobjective[var], "default")
      || !strcmp ((*pTOpt).tarobjective[var], "FL2w"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_FL2w (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "FL2wu"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_FL2wu (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "ad"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_ad (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "cvm"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_cvm (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "FL2"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_FL2 (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "chi2"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_chi2 (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "ks"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_ks (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "kuiper"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_kuiper (pTOpt, var);

  else if (!strcmp ((*pTOpt).tarobjective[var], "FiL2"))
    net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate_FiL2 (pTOpt, var);

  else
    ut_print_message (2, 3, "Unknown test `%s'.", (*pTOpt).tarobjective[var]);

  return;
}
