/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_log_.h"

void
net_tess_opt_comp_minimize_objective_log_time (struct TOPT TOpt)
{
  if (!ut_list_iter_test (TOpt.TDyn.logtime, NEUT_SEP_NODEP, TOpt.iter))
    return;

  neut_tdyn_fprintf_time (TOpt.TDyn);

  return;
}

void
net_tess_opt_comp_minimize_objective_log_dis (struct TOPT TOpt, int var)
{
  int i, j;
  int varqty;
  char **vars = NULL;
  int same_x = ut_fct_samex (TOpt.tarcdf[var], TOpt.curcdf[var]);

  if (!ut_list_iter_test (TOpt.TDyn.logdis, NEUT_SEP_NODEP, TOpt.iter))
    return;

  ut_list_break (TOpt.TDyn.logdis, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 0; i < TOpt.curpdf[var].size; i++)
  {
    if (TOpt.curpdf[var].size != TOpt.curcdf[var].size)
      abort ();
    for (j = 0; j < varqty; j++)
    {
      if (!strncmp (vars[j], "iter", 4))
        fprintf (TOpt.TDyn.logdis_fp[var], "%d", TOpt.iter);
      else if (!strcmp (vars[j], "x"))
        fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, TOpt.tarpdf[var].x[i]);
      else if (!strcmp (vars[j], "tarpdf0"))
        fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, TOpt.tarpdf0[var].y[i]);
      else if (!strcmp (vars[j], "tarcdf0"))
        fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, TOpt.tarcdf0[var].y[i]);
      else if (!strcmp (vars[j], "tarpdf"))
        fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, TOpt.tarpdf[var].y[i]);
      else if (!strcmp (vars[j], "tarcdf"))
        fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, TOpt.tarcdf[var].y[i]);
      else if (!strcmp (vars[j], "curpdf"))
      {
        if (same_x)
          fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, TOpt.curpdf[var].y[i]);
        else
          fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, ut_fct_eval (TOpt.curpdf[var], TOpt.tarpdf[var].x[i]));
      }
      else if (!strcmp (vars[j], "curcdf"))
      {
        if (same_x)
          fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, TOpt.curcdf[var].y[i]);
        else
          fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, ut_fct_eval (TOpt.curcdf[var], TOpt.tarpdf[var].x[i]));
      }
      else if (!strcmp (vars[j], "curcdf0"))
      {
        if (same_x)
          fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, TOpt.curcdf0[var].y[i]);
        else
          fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT, ut_fct_eval (TOpt.curcdf0[var], TOpt.tarpdf[var].x[i]));
      }
      else if (!strcmp (vars[j], "curpdf0"))
      {
        if (same_x)
        {
          if (i < TOpt.curcdf0[var].size - 1)
            fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT,
                     (TOpt.curcdf0[var].y[i + 1] - TOpt.curcdf0[var].y[i]) / (TOpt.curcdf0[var].x[i + 1] - TOpt.curcdf0[var].x[i]));
          else
            fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT,
                     (TOpt.curcdf0[var].y[i] - TOpt.curcdf0[var].y[i - 1]) / (TOpt.curcdf0[var].x[i] - TOpt.curcdf0[var].x[i - 1]));
        }
        else
        {
          if (i < TOpt.curcdf0[var].size - 1)
            fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT,
                     (ut_fct_eval (TOpt.curcdf0[var], TOpt.tarpdf[var].x[i + 1])
                    - ut_fct_eval (TOpt.curcdf0[var], TOpt.tarpdf[var].x[i])) / (TOpt.tarpdf[var].x[i + 1] - TOpt.tarpdf[var].x[i]));
          else
            fprintf (TOpt.TDyn.logdis_fp[var], REAL_PRINT_FORMAT,
                     (ut_fct_eval (TOpt.curcdf0[var], TOpt.tarpdf[var].x[i])
                    - ut_fct_eval (TOpt.curcdf0[var], TOpt.tarpdf[var].x[i - 1])) / (TOpt.tarpdf[var].x[i] - TOpt.tarpdf[var].x[i - 1]));
        }
      }
      else
        fprintf (TOpt.TDyn.logdis_fp[var], "-1");

      if (j < varqty - 1)
        fprintf (TOpt.TDyn.logdis_fp[var], " ");
    }

    fprintf (TOpt.TDyn.logdis_fp[var], "\n");
  }

  ut_free_2d_char (&vars, varqty);

  return;
}

void
net_tess_opt_comp_minimize_objective_log_var (struct TOPT TOpt)
{
  int i, j;
  int varqty;
  char **vars = NULL;

  if (!ut_list_iter_test (TOpt.TDyn.logvar, NEUT_SEP_NODEP, TOpt.iter))
    return;

  ut_list_break (TOpt.TDyn.logvar, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 1; i <= (TOpt.SSet).N; i++)
  {
    for (j = 0; j < varqty; j++)
    {
      if (!strncmp (vars[j], "iter", 4))
        fprintf (TOpt.TDyn.logvar_fp, "%d", TOpt.iter);
      else if (!strcmp (vars[j], "id"))
        fprintf (TOpt.TDyn.logvar_fp, "%d", i);
      else if (!strcmp (vars[j], "x"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedCoo0[i][0]);
      else if (!strcmp (vars[j], "y"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedCoo0[i][1]);
      else if (!strcmp (vars[j], "z"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedCoo0[i][2]);
      else if (!strcmp (vars[j], "w"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedWeight[i]);
      else if (!strcmp (vars[j], "error"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.curcelltotalerror[i]);
      else if (!strcmp (vars[j], "r1"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedOriR[i][0]);
      else if (!strcmp (vars[j], "r2"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedOriR[i][1]);
      else if (!strcmp (vars[j], "r3"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedOriR[i][2]);
      else if (!strcmp (vars[j], "rw") && TOpt.SSet.SeedOriWeight)
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedOriWeight[i]);
      else if (!strcmp (vars[j], "rt"))
        fprintf (TOpt.TDyn.logvar_fp, REAL_PRINT_FORMAT, TOpt.SSet.SeedOriTheta[i]);
      else
        fprintf (TOpt.TDyn.logvar_fp, "-1");

      if (j < varqty - 1)
        fprintf (TOpt.TDyn.logvar_fp, " ");
    }
    fprintf (TOpt.TDyn.logvar_fp, "\n");
  }

  ut_free_2d_char (&vars, varqty);

  return;
}

void
net_tess_opt_comp_minimize_objective_log_val (struct TOPT TOpt)
{
  int i, id;
  int varqty;
  double val;
  char **vars = NULL;

  if (!ut_list_iter_test (TOpt.TDyn.logval, NEUT_SEP_NODEP, TOpt.iter))
    return;

  ut_list_break (TOpt.TDyn.logval, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 0; i < varqty; i++)
  {
    if (!strncmp (vars[i], "iter", 4))
      fprintf (TOpt.TDyn.logval_fp, "%d", TOpt.iter);
    else if (!strcmp (vars[i], "val"))
      fprintf (TOpt.TDyn.logval_fp, REAL_PRINT_FORMAT, TOpt.objval);
    else if (!strcmp (vars[i], "valmin"))
      fprintf (TOpt.TDyn.logval_fp, REAL_PRINT_FORMAT, TOpt.objvalmin[TOpt.iter]);
    else if (!strcmp (vars[i], "val0"))
      fprintf (TOpt.TDyn.logval_fp, REAL_PRINT_FORMAT, TOpt.objval0);
    else if (!strcmp (vars[i], "valmin0"))
      fprintf (TOpt.TDyn.logval_fp, REAL_PRINT_FORMAT, TOpt.objvalmin0[TOpt.iter]);
    else if (!strncmp (vars[i], "val(", 4))
    {
      sscanf (vars[i], "val(%d)", &id);
      if (id >= 1 && id <= TOpt.tarqty)
        fprintf (TOpt.TDyn.logval_fp, REAL_PRINT_FORMAT, TOpt.curval[id - 1]);
      else
        fprintf (TOpt.TDyn.logval_fp, "-1");
    }
    else if (!strncmp (vars[i], "val0(", 5))
    {
      sscanf (vars[i], "val0(%d)", &id);
      if (id >= 1 && id <= TOpt.tarqty)
        fprintf (TOpt.TDyn.logval_fp, REAL_PRINT_FORMAT, TOpt.curval0[id - 1]);
      else
        fprintf (TOpt.TDyn.logval_fp, "-1");
    }
    else if (!strcmp (vars[i], "eps"))
    {
      neut_topt_eps (TOpt, &val);
      fprintf (TOpt.TDyn.logval_fp, REAL_PRINT_FORMAT, val);
    }
    else if (!strcmp (vars[i], "reps"))
    {
      neut_topt_reps (TOpt, &val);
      fprintf (TOpt.TDyn.logval_fp, REAL_PRINT_FORMAT, val);
    }
    else
      fprintf (TOpt.TDyn.logval_fp, "-1");

    if (i < varqty - 1)
      fprintf (TOpt.TDyn.logval_fp, " ");
  }
  fprintf (TOpt.TDyn.logval_fp, "\n");
  fflush (TOpt.TDyn.logval_fp);

  ut_free_2d_char (&vars, varqty);

  return;
}

void
net_tess_opt_comp_minimize_objective_log_tesr (struct TOPT TOpt)
{
  int i, j, k;
  char **vars = NULL;
  int varqty;

  if (!ut_list_iter_test (TOpt.TDyn.logtesr, NEUT_SEP_NODEP, TOpt.iter))
    return;

  ut_list_break (TOpt.TDyn.logtesr, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 1; i <= TOpt.CellQty; i++)
    for (j = 0; j < TOpt.tarcellptqty[i]; j++)
    {
      for (k = 0; k < varqty; k++)
      {
        if (!strncmp (vars[k], "iter", 4))
          fprintf (TOpt.TDyn.logtesr_fp, "%d", TOpt.TDyn.iter);
        else if (!strcmp (vars[k], "id"))
          fprintf (TOpt.TDyn.logtesr_fp, "%d", i);
        else if (!strcmp (vars[k], "x"))
          fprintf (TOpt.TDyn.logtesr_fp, REAL_PRINT_FORMAT, TOpt.tarcellpts[i][j][0]);
        else if (!strcmp (vars[k], "y"))
          fprintf (TOpt.TDyn.logtesr_fp, REAL_PRINT_FORMAT, TOpt.tarcellpts[i][j][1]);
        else if (!strcmp (vars[k], "z"))
          fprintf (TOpt.TDyn.logtesr_fp, REAL_PRINT_FORMAT, TOpt.tarcellpts[i][j][2]);
        else if (!strcmp (vars[k], "w"))
          fprintf (TOpt.TDyn.logtesr_fp, REAL_PRINT_FORMAT, TOpt.tarcellptweights[i][j]);
        else if (!strcmp (vars[k], "dist"))
          fprintf (TOpt.TDyn.logtesr_fp, REAL_PRINT_FORMAT, TOpt.tarcellptsdist[i][j]);
        else
          fprintf (TOpt.TDyn.logtesr_fp, "-1");

        if (k < varqty - 1)
          fprintf (TOpt.TDyn.logtesr_fp, " ");
      }
      fprintf (TOpt.TDyn.logtesr_fp, "\n");
    }

  ut_free_2d_char (&vars, varqty);

  return;
}
