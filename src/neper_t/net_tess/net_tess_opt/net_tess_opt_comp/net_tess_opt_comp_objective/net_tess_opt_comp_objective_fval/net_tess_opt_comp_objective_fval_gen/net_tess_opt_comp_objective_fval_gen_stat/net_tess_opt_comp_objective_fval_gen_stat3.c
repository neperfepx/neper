/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_stat_.h"

void
net_tess_opt_comp_objective_fval_gen_stat_smoothed_update_cell (struct TOPT *pTOpt, int var, int cell)
{
  int i, minpos, maxpos;
  double val, min, max;

  if ((*pTOpt).oldcellpenalty[cell] == 0 && (*pTOpt).curcellpenalty[cell] == 0)
  {
    min = ut_num_min ((*pTOpt).oldcellval[var][cell][0],
                      (*pTOpt).curcellval[var][cell][0])
        - 5 * (*pTOpt).cvlsig[var];

    minpos = ut_fct_x_pos ((*pTOpt).curcdf[var], min);

    if (minpos < 1)
      abort ();

    minpos--;

    max = ut_num_max ((*pTOpt).oldcellval[var][cell][0],
                      (*pTOpt).curcellval[var][cell][0])
        + 5 * (*pTOpt).cvlsig[var];

    maxpos = ut_fct_x_pos ((*pTOpt).curcdf[var], max);

    if (maxpos > (*pTOpt).curcdf[var].size - 1)
      abort ();

    maxpos++;

    for (i = minpos; i < maxpos; i++)
    // for (i = 0; i < (*pTOpt).curcdf[var].size; i++)
    {
      val = erf (((*pTOpt).curcdf[var].x[i] - (*pTOpt).curcellval[var][cell][0])
	         / (OL_S2 * (*pTOpt).cvlsig[var]))
          - erf (((*pTOpt).curcdf[var].x[i] - (*pTOpt).oldcellval[var][cell][0])
	         / (OL_S2 * (*pTOpt).cvlsig[var]));

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
net_tess_opt_comp_objective_fval_gen_stat_evaluate_chi2 (struct TOPT *pTOpt,
							  int var)
{
  int i;
  double val = 0;

  for (i = 0; i < (*pTOpt).tarpdf[var].size; i++)
  {
    if ((*pTOpt).tarpdf[var].y[i] > 0)
      val += pow ((*pTOpt).curpdf[var].y[i] -
	  (*pTOpt).tarpdf[var].y[i], 2)
	/ (*pTOpt).tarpdf[var].y[i];
    else
      val += pow ((*pTOpt).curpdf[var].y[i] -
	  (*pTOpt).tarpdf[var].y[i], 2)
	/ (.5 * ((*pTOpt).curpdf[var].y[i] + (*pTOpt).tarpdf[var].y[i]));
  }

  (*pTOpt).curval[var] = val;

  return;
}

// Kolmogorov Sminrnov
void
net_tess_opt_comp_objective_fval_gen_stat_evaluate_ks (struct TOPT *pTOpt,
					 int var)
{
  ut_stat_test_ks ((*pTOpt).tarcdf[var].y, (*pTOpt).curcdf[var].y,
		   (*pTOpt).tarcdf[var].size,
		   (*pTOpt).curval + var);

  return;
}

// Kuiper
void
net_tess_opt_comp_objective_fval_gen_stat_evaluate_kuiper (struct TOPT *pTOpt,
					     int var)
{
  ut_stat_test_kuiper ((*pTOpt).tarcdf[var].y,
		       (*pTOpt).curcdf[var].y,
		       (*pTOpt).tarcdf[var].size,
		       (*pTOpt).curval + var);

  return;
}

// Cramer Von-Mises
void
net_tess_opt_comp_objective_fval_gen_stat_evaluate_cvm (struct TOPT *pTOpt,
					  int var)
{
  int i, N = (*pTOpt).CellQty;;
  double x, w, F, val;
  double *array = ut_alloc_1d (N + 1);

  val = 0;

  if ((*pTOpt).cvlsig[var] == 0 || strstr ((*pTOpt).TDyn.logval, "val0"))
  {
    for (i = 1; i <= N; i++)
      array[i] = (*pTOpt).curcellval[var][i][0];

    ut_array_1d_sort (array + 1, N);

    val = 1. / (12 * N);
    for (i = 1; i <= N; i++)
    {
      F = ut_fct_eval ((*pTOpt).tarcdf0[var], array[i]);
      val += pow ((2. * i - 1) / (2. * N) - F, 2);
    }

    val /= N;

    (*pTOpt).curval0[var] = val;
    if ((*pTOpt).cvlsig[var] == 0)
      (*pTOpt).curval[var] = val;
  }

  if ((*pTOpt).cvlsig[var] > 0)
  {
    for (i = 0; i < (*pTOpt).curcdf[var].size; i++)
    {
      x = (*pTOpt).curcdf[var].x[i];
      w = ut_fct_binwidth ((*pTOpt).curcdf[var], i);
      val += w * pow ((*pTOpt).curcdf[var].y[i]
		    - ut_fct_eval ((*pTOpt).tarcdf[var], x), 2)
	       * ut_fct_eval ((*pTOpt).tarpdf[var], x);
    }

    (*pTOpt).curval[var] = val;
  }

  ut_free_1d (array);

  return;
}

// Anderson–Darling
void
net_tess_opt_comp_objective_fval_gen_stat_evaluate_ad (struct TOPT
    *pTOpt, int var)
{
  int i, N = (*pTOpt).CellQty;
  double w, weight, val1, val2, S, val;
  double *array = ut_alloc_1d (N + 1);

  if ((*pTOpt).cvlsig[var] == 0 || strstr ((*pTOpt).TDyn.logval, "val0"))
  {
    for (i = 1; i <= N; i++)
      array[i] = (*pTOpt).curcellval[var][i][0];

    ut_array_1d_sort (array + 1, N);

    S = 0;
    for (i = 1; i <= N; i++)
    {
      val1 = ut_fct_eval ((*pTOpt).tarcdf0[var], array[i]);
      val2 = ut_fct_eval ((*pTOpt).tarcdf0[var], array[N + 1 - i]);
      val1 = ut_num_max (val1, 1e-9);
      val2 = ut_num_max (val2, 1e-9);

      S += ((2. * i - 1) / N) * (ut_num_log (val1) + ut_num_log (1 - val2));
    }

    val = -N - S;

    if (isnan (val))
    {
      printf ("val is not-a-number.\n");
      abort ();
    }

    (*pTOpt).curval0[var] = val;
    if ((*pTOpt).cvlsig[var] == 0)
      (*pTOpt).curval[var] = val;
  }

  if ((*pTOpt).cvlsig[var] > 0)
  {
    val = 0;
    for (i = 0; i < (*pTOpt).tarcdf[var].size; i++)
    {
      w = ut_fct_binwidth ((*pTOpt).tarcdf[var], i);

      weight = ut_num_min (1 / ((*pTOpt).tarcdf[var].y[i]
			        * (1 - (*pTOpt).tarcdf[var].y[i])),
			    DBL_MAX);

      val += w * weight * pow ((*pTOpt).curcdf[var].y[i] -
			       (*pTOpt).tarcdf[var].y[i], 2)
			* (*pTOpt).tarpdf[var].y[i];
    }

    (*pTOpt).curval[var] = val;
  }

  ut_free_1d (array);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_stat_evaluate_FL2w (struct TOPT
    *pTOpt, int var)
{
  int i, j;
  double w, weight, val;

  val = 0;
  for (i = 0; i < (*pTOpt).tarcdf[var].size; i++)
  {
    w = ut_fct_binwidth ((*pTOpt).tarcdf[var], i);

    weight = 1 / (*pTOpt).tarcdf[var].y[i];
    for (j = 0; j < (*pTOpt).tarexprdisqty[var] - 1; j++)
      weight /= fabs ((*pTOpt).tarcdf[var].y[i] - (*pTOpt).tarexprdisprop[var][j]);
    weight /= fabs ((*pTOpt).tarcdf[var].y[i] - 1);
    weight = ut_num_min (weight, 1e3);

    val += w * weight * pow ((*pTOpt).curcdf[var].y[i] -
			     (*pTOpt).tarcdf[var].y[i], 2);
  }

  (*pTOpt).curval[var] = val;

  if (strstr ((*pTOpt).TDyn.logval, "val0"))
  {
    val = 0;
    for (i = 0; i < (*pTOpt).tarcdf0[var].size; i++)
    {
      w = ut_fct_binwidth ((*pTOpt).tarcdf0[var], i);

      weight = 1 / (*pTOpt).tarcdf0[var].y[i];
      for (j = 0; j < (*pTOpt).tarexprdisqty[var] - 1; j++)
	weight /= fabs ((*pTOpt).tarcdf0[var].y[i] - (*pTOpt).tarexprdisprop[var][j]);
      weight /= fabs ((*pTOpt).tarcdf0[var].y[i] - 1);
      weight = ut_num_min (weight, 1e3);

      val += w * weight * pow ((*pTOpt).curcdf0[var].y[i] -
			       (*pTOpt).tarcdf0[var].y[i], 2);
    }

    (*pTOpt).curval0[var] = val;
  }

  return;
}

void
net_tess_opt_comp_objective_fval_gen_stat_evaluate_FL2 (struct TOPT
    *pTOpt, int var)
{
  int i;
  double w, val;

  val = 0;
  for (i = 0; i < (*pTOpt).tarcdf[var].size; i++)
  {
    w = ut_fct_binwidth ((*pTOpt).tarcdf[var], i);

    val += w * pow ((*pTOpt).curcdf[var].y[i] -
		    (*pTOpt).tarcdf[var].y[i], 2);
  }

  (*pTOpt).curval[var] = val;

  return;
}
