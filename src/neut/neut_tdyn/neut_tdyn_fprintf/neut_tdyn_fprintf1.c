/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tdyn_fprintf_.h"

void
neut_tdyn_fprintf_pre (struct TDYN *pTDyn)
{
  int i;
  char *filename = NULL;

  if (strcmp ((*pTDyn).logtime, "none"))
  {
    filename = ut_string_addextension ((*pTDyn).body, ".logtime");
    (*pTDyn).logtime_fp = ut_file_open (filename, "w");
    ut_free_1d_char (filename);
  }

  if (strcmp ((*pTDyn).logvar, "none"))
  {
    filename = ut_string_addextension ((*pTDyn).body, ".logvar");
    (*pTDyn).logvar_fp = ut_file_open (filename, "w");
    ut_free_1d_char (filename);
  }

  if (strcmp ((*pTDyn).logdis, "none"))
  {
    (*pTDyn).logdis_fp = (FILE **) calloc ((*pTDyn).logdis_qty, sizeof (FILE *));
    for (i = 0; i < (*pTDyn).logdis_qty; i++)
    {
      filename = ut_string_addextension ((*pTDyn).body, ".logdis%d", i + 1);
      (*pTDyn).logdis_fp[i] = ut_file_open (filename, "w");
      ut_free_1d_char (filename);
    }
  }

  if (strcmp ((*pTDyn).logval, "none"))
  {
    filename = ut_string_addextension ((*pTDyn).body, ".logval");
    (*pTDyn).logval_fp = ut_file_open (filename, "w");
    ut_free_1d_char (filename);
  }

  return;
}

void
neut_tdyn_fprintf_post (struct TDYN *pTDyn)
{
  int i;
  char *filename = NULL;

  if (strcmp ((*pTDyn).logtime, "none"))
  {
    filename = ut_string_addextension ((*pTDyn).body, ".logtime");
    printf ("\n");
    ut_file_close_nonl ((*pTDyn).logtime_fp, filename, "w");
    ut_free_1d_char (filename);
  }

  if (strcmp ((*pTDyn).logvar, "none"))
  {
    filename = ut_string_addextension ((*pTDyn).body, ".logvar");
    printf ("\n");
    ut_file_close_nonl ((*pTDyn).logvar_fp, filename, "w");
    ut_free_1d_char (filename);
  }

  if (strcmp ((*pTDyn).logdis, "none"))
  {
    for (i = 0; i < (*pTDyn).logdis_qty; i++)
    {
      filename = ut_string_addextension ((*pTDyn).body, ".logdis%d", i + 1);
      printf ("\n");
      ut_file_close_nonl ((*pTDyn).logdis_fp[i], filename, "w");
      ut_free_1d_char (filename);
    }
  }

  if (strcmp ((*pTDyn).logval, "none"))
  {
    filename = ut_string_addextension ((*pTDyn).body, ".logval");
    printf ("\n");
    ut_file_close_nonl ((*pTDyn).logval_fp, filename, "w");
    ut_free_1d_char (filename);
  }

  return;
}

void
neut_tdyn_fprintf_time (struct TDYN TDyn)
{
  int i, varqty;
  char **vars = NULL;

  if (!ut_string_iter_test (TDyn.logtime, NEUT_SEP_NODEP, TDyn.iter))
    return;

  ut_string_separate (TDyn.logtime, NEUT_SEP_NODEP, &vars, &varqty);

  for (i = 0; i < varqty; i++)
  {
    if (!strncmp (vars[i], "iter", 4))
      fprintf (TDyn.logtime_fp, "%d", TDyn.iter);
    else if (!strcmp (vars[i], "varupdateqty"))
      fprintf (TDyn.logtime_fp, "%d", TDyn.varchangedqty);
    else if (!strcmp (vars[i], "seedupdateqty"))
      fprintf (TDyn.logtime_fp, "%d", TDyn.seedchangedqty);
    else if (!strcmp (vars[i], "seedupdatelist"))
      ut_array_1d_int_fprintf_nonl (TDyn.logtime_fp, TDyn.seedchanged,
			       TDyn.seedchangedqty, "%d");
    else if (!strcmp (vars[i], "cellupdateqty"))
      fprintf (TDyn.logtime_fp, "%d", TDyn.cellchangedqty);
    else if (!strcmp (vars[i], "cellupdatelist"))
      ut_array_1d_int_fprintf_nonl (TDyn.logtime_fp, TDyn.cellchanged,
			       TDyn.cellchangedqty, "%d");
    else if (!strcmp (vars[i], "var"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.var_dur);
    else if (!strcmp (vars[i], "seed"))
      fprintf (TDyn.logtime_fp, "%.6f", fabs (TDyn.seed_dur));

    else if (!strcmp (vars[i], "cell_init"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.cell_init_dur);
    else if (!strcmp (vars[i], "cell_kdtree"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.cell_kdtree_dur);
    else if (!strcmp (vars[i], "cell_shift"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.cell_shift_dur);
    else if (!strcmp (vars[i], "cell_neigh"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.cell_neigh_dur);
    else if (!strcmp (vars[i], "cell_cell"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.cell_cell_dur);
    else if (!strcmp (vars[i], "cell_other"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.cell_other_dur);
    else if (!strcmp (vars[i], "cell_total"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.cell_total_dur);

    else if (!strcmp (vars[i], "val"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.val_dur);
    else if (!strcmp (vars[i], "total"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.total_dur);
    else if (!strcmp (vars[i], "cumtotal"))
      fprintf (TDyn.logtime_fp, "%.6f", TDyn.cumtotal_dur);
    else
      fprintf (TDyn.logtime_fp, "%d", -1);

      if (i < varqty - 1)
	fprintf (TDyn.logtime_fp, " ");
    }
    fprintf (TDyn.logtime_fp, "\n");


  ut_free_2d_char (vars,varqty);

  return;
}
