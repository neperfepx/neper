/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_weight_.h"

void
net_tess_opt_init_sset_weight (struct MTESS MTess, struct TESS *Tess,
			       int dtess, int dcell, struct TOPT TOpt,
			       char *var, int pos, char *weightexpr,
			       double *rad)
{
  int i;
  char *prev = ut_alloc_1d_char (1000);
  int varqty = 4;
  char **vars = ut_alloc_2d_char (varqty, 10);
  double *vals = ut_alloc_1d (varqty);
  double *radeq = ut_alloc_1d (TOpt.CellQty + 1);
  char *mid = NULL;
  strcpy (vars[0], "avradeq");
  strcpy (vars[1], "avdiameq");
  strcpy (vars[2], "radeq");
  strcpy (vars[3], "diameq");

  if (ut_string_filename (weightexpr))
  {
    neut_mtess_tess_poly_mid (MTess, Tess[dtess], dcell, &mid);
    net_multiscale_arg_1d_fscanf (weightexpr, mid, rad + 1, TOpt.CellQty);
  }

  // special case of tesr: sending to neut_tesr_expr_val
  else if (!strcmp (var, "tesr"))
    for (i = 1; i <= TOpt.CellQty; i++)
    {
      neut_tesr_expr_val (TOpt.tartesr, "cell", i, weightexpr, rad + i, NULL);
      ut_print_progress (stdout, i, TOpt.CellQty, "%.0f%%", prev);
    }

  // other cases: stat, centroid, ...
  else
  {
    // avradeq and avdiameq
    if (!strcmp (var, "size") || !strcmp (var, "diameq")
     || !strncmp (var, "centroid", 8))
    {
      neut_tess_cellavradeq (TOpt.Dom, TOpt.CellQty, vals);
      vals[1] = 2 * vals[0];
    }

    // radeq
    if (!strcmp (var, "none"))
      ut_array_1d_set (radeq + 1, TOpt.CellQty, 0);
    else if (!strcmp (var, "size") || !strcmp (var, "diameq"))
      net_tess_opt_init_sset_weight_stat_radeq (pos, var, TOpt, radeq);
    else if (!strcmp (var, "centroid"))
      ut_array_1d_set (radeq + 1, TOpt.CellQty, vals[0]);
    else if (!strcmp (var, "centroiddiameq"))
      for (i = 1; i <= TOpt.CellQty; i++)
	radeq[i] = 0.5 * TOpt.tarcellval[pos][i][TOpt.SSet.Dim];
    else
      abort ();

    // evaluating expression
    for (i = 1; i <= TOpt.CellQty; i++)
    {
      vals[2] = radeq[i];
      vals[3] = 2 * radeq[i];
      ut_math_eval (weightexpr, varqty, vars, vals, rad + i);
    }
  }

  ut_free_1d_char (prev);
  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);
  ut_free_1d (radeq);
  ut_free_1d_char (mid);

  return;
}
