/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_tesrobj_.h"

void
net_tess_opt_init_tesrobj (struct TOPT *pTOpt, int var)
{
  int i, j, PartQty, fct_varqty = 0;
  char **parts = NULL;
  char *fct = NULL;
  char **fct_vars = NULL;
  char **fct_vals = NULL;

  net_tess_opt_init_tesrobj_pre (pTOpt);

  ut_list_break ((*pTOpt).tarobjective[var], "+", &parts, &PartQty);

  for (i = 0; i < PartQty; i++)
  {
    ut_string_function (parts[i], &fct, &fct_vars, &fct_vals, &fct_varqty);

    if (!strcmp (fct, "transform"))
    {
      for (j = 0; j < fct_varqty; j++)
      {
        if (!strcmp (fct_vals[j], "scale"))
          net_tess_opt_init_tesrobj_scale (pTOpt);
        else if (!strcmp (fct_vals[j], "rasterscale"))
          net_tess_opt_init_tesrobj_rasterscale (pTOpt);
      }
    }

    else if (!strcmp (fct, "pts"))
    {
      for (j = 0; j < fct_varqty; j++)
      {
        if (!strcmp (fct_vars[j], "region"))
          net_tess_opt_init_tesrobj_region (fct_vals[j], pTOpt);
        else if (!strcmp (fct_vars[j], "res"))
          net_tess_opt_init_tesrobj_res (fct_vals[j], pTOpt);
      }
    }

    else if (!strcmp (fct, "val"))
      ut_string_string (fct_vals[0], &((*pTOpt).objective_tesrval));

    else
      abort ();
  }

  if (!(*pTOpt).objective_tesrval)
    ut_string_string ("bounddist", &(*pTOpt).objective_tesrval);

  net_tess_opt_init_tesrobj_post (pTOpt);

  ut_free_2d_char (&parts, PartQty);
  ut_free_2d_char (&fct_vars, fct_varqty);
  ut_free_2d_char (&fct_vals, fct_varqty);

  return;
}
