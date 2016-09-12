/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_log_.h"

void
net_tess_opt_comp_objective_log_time (struct TOPT TOpt)
{
  if (!ut_string_iter_test (TOpt.TDyn.logtime, NEUT_SEP_NODEP, TOpt.iter))
    return;

  neut_tdyn_fprintf_time (TOpt.TDyn);

  return;
}

void
net_tess_opt_comp_objective_log_dis (struct TOPT TOpt, int id)
{
  int i, j;
  int varqty;
  char **vars = NULL;

  if (!ut_string_iter_test (TOpt.TDyn.logdis, NEUT_SEP_NODEP, TOpt.iter))
    return;

  ut_string_separate (TOpt.TDyn.logdis, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 0; i < TOpt.curpdf[id].size; i++)
  {
    if (TOpt.curpdf[id].size != TOpt.curcdf[id].size)
      abort ();
    for (j = 0; j < varqty; j++)
    {
      if (!strncmp (vars[j], "iter", 4))
	fprintf (TOpt.TDyn.logdis_fp[id], "%d", TOpt.iter);
      else if (!strcmp (vars[j], "x"))
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f", TOpt.curpdf[id].x[i]);
      else if (!strcmp (vars[j], "tarpdf"))
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f", TOpt.tarpdf[id].y[i]);
      /*
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f",
	         ut_fct_eval (TOpt.tarpdf[id],  TOpt.curpdf[id].x[i]));
		 */
      else if (!strcmp (vars[j], "tarcdf"))
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f", TOpt.tarcdf[id].y[i]);
      /*
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f",
		 ut_fct_eval (TOpt.tarcdf[id],  TOpt.curpdf[id].x[i]));
      */
      else if (!strcmp (vars[j], "curpdf"))
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f", TOpt.curpdf[id].y[i]);
      else if (!strcmp (vars[j], "curcdf"))
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f", TOpt.curcdf[id].y[i]);
      else if (!strcmp (vars[j], "tarpdf0"))
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f", TOpt.tarpdf0[id].y[i]);
      /*
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f",
	          ut_fct_eval (TOpt.tarpdf0[id],  TOpt.curpdf[id].x[i]));
      */
      else if (!strcmp (vars[j], "tarcdf0"))
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f", TOpt.tarcdf0[id].y[i]);
      /*
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f",
		 ut_fct_eval (TOpt.tarcdf0[id],  TOpt.curpdf[id].x[i]));
      */
      else if (!strcmp (vars[j], "curcdf0"))
	fprintf (TOpt.TDyn.logdis_fp[id], "%.6f", TOpt.curcdf0[id].y[i]);
      else
	fprintf (TOpt.TDyn.logdis_fp[id], "-1");

      if (j < varqty - 1)
	fprintf (TOpt.TDyn.logdis_fp[id], " ");
    }

    fprintf (TOpt.TDyn.logdis_fp[id], "\n");
  }

  ut_free_2d_char (vars, varqty);

  return;
}

void
net_tess_opt_comp_objective_log_var (struct TOPT TOpt)
{
  int i, j;
  int varqty;
  char **vars = NULL;

  if (!ut_string_iter_test (TOpt.TDyn.logvar, NEUT_SEP_NODEP, TOpt.iter))
    return;

  ut_string_separate (TOpt.TDyn.logvar, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 1; i <= (TOpt.SSet).N; i++)
  {
    for (j = 0; j < varqty; j++)
    {
      if (!strncmp (vars[j], "iter", 4))
	fprintf (TOpt.TDyn.logvar_fp, "%d", TOpt.TDyn.iter);
      else if (!strcmp (vars[j], "id"))
	fprintf (TOpt.TDyn.logvar_fp, "%d", i);
      else if (!strcmp (vars[j], "x"))
	fprintf (TOpt.TDyn.logvar_fp, "%.6f", TOpt.SSet.SeedCoo0[i][0]);
      else if (!strcmp (vars[j], "y"))
	fprintf (TOpt.TDyn.logvar_fp, "%.6f", TOpt.SSet.SeedCoo0[i][1]);
      else if (!strcmp (vars[j], "z"))
	fprintf (TOpt.TDyn.logvar_fp, "%.6f", TOpt.SSet.SeedCoo0[i][2]);
      else if (!strcmp (vars[j], "w"))
	fprintf (TOpt.TDyn.logvar_fp, "%.6f", TOpt.SSet.SeedWeight[i]);
      else
	fprintf (TOpt.TDyn.logvar_fp, "-1");

      if (j < varqty - 1)
	fprintf (TOpt.TDyn.logvar_fp, " ");
    }
    fprintf (TOpt.TDyn.logvar_fp, "\n");
  }

  ut_free_2d_char (vars, varqty);

  return;
}

void
net_tess_opt_comp_objective_log_val (struct TOPT TOpt)
{
  int i, id;
  int varqty;
  char **vars = NULL;

  if (!ut_string_iter_test (TOpt.TDyn.logval, NEUT_SEP_NODEP, TOpt.iter))
    return;

  ut_string_separate (TOpt.TDyn.logval, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 0; i < varqty; i++)
  {
    if (!strncmp (vars[i], "iter", 4))
      fprintf (TOpt.TDyn.logval_fp, "%d", TOpt.TDyn.iter);
    else if (!strcmp (vars[i], "val"))
      fprintf (TOpt.TDyn.logval_fp, "%.6f", TOpt.objval);
    else if (!strcmp (vars[i], "valmin"))
      fprintf (TOpt.TDyn.logval_fp, "%.6f", TOpt.objvalmin[TOpt.iter]);
    else if (!strncmp (vars[i], "val(", 4))
    {
      sscanf (vars[i], "val(%d)", &id);
      if (id >= 1 && id <= TOpt.tarqty)
	fprintf (TOpt.TDyn.logval_fp, "%.6f", TOpt.curval[id - 1]);
      else
	fprintf (TOpt.TDyn.logval_fp, "-1");
    }
    else if (!strncmp (vars[i], "val0(", 5))
    {
      sscanf (vars[i], "val0(%d)", &id);
      if (id >= 1 && id <= TOpt.tarqty)
	fprintf (TOpt.TDyn.logval_fp, "%.6f", TOpt.curval0[id - 1]);
      else
	fprintf (TOpt.TDyn.logval_fp, "-1");
    }
    else
      fprintf (TOpt.TDyn.logval_fp, "-1");

    if (i < varqty - 1)
      fprintf (TOpt.TDyn.logval_fp, " ");
  }
  fprintf (TOpt.TDyn.logval_fp, "\n");

  ut_free_2d_char (vars, varqty);

  return;
}
