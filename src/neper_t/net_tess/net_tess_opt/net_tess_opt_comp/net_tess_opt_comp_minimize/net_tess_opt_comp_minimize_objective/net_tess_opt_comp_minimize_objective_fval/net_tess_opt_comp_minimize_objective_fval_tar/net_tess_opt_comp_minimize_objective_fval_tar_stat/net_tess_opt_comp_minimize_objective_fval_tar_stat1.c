/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_fval_tar_stat_.h"

void
net_tess_opt_comp_minimize_objective_fval_tar_stat (struct TOPT *pTOpt, int var)
{
  int i, cell;

  if ((*pTOpt).iter == 0)
    net_tess_opt_comp_minimize_objective_fval_tar_stat_alloc (pTOpt, var);

  if ((*pTOpt).cvlsig[var] > 0)
  {
    // full computation at the first iteration and every 1000 iterations
    if ((*pTOpt).iter % 1000 == 0)
    {
      if (!strcmp ((*pTOpt).cvlmethod, "numerical"))
        net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_gen_legacy (pTOpt,
                                                                                var);
      else if (!strcmp ((*pTOpt).cvlmethod, "analytical"))
        net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_comp (pTOpt, var);
      else
        ut_print_neperbug ();
    }

    else
    {
      if (!strcmp ((*pTOpt).cvlmethod, "numerical"))
        net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_update_legacy
          (pTOpt, var);
      else if (!strcmp ((*pTOpt).cvlmethod, "analytical"))
        net_tess_opt_comp_minimize_objective_fval_tar_stat_smoothed_update (pTOpt,
                                                                            var);
      else
        ut_print_neperbug ();
    }
  }

  if ((*pTOpt).cvlsig[var] == 0 || strstr ((*pTOpt).TDyn.logdis, "curcdf0")
      || strstr ((*pTOpt).TDyn.logval, "val0"))
  {
    if ((*pTOpt).iter % 1000 == 0)
      net_tess_opt_comp_minimize_objective_fval_tar_stat_unsmoothed_comp (pTOpt, var);

    else
      net_tess_opt_comp_minimize_objective_fval_tar_stat_unsmoothed_update (pTOpt, var);
  }

  net_tess_opt_comp_minimize_objective_fval_tar_stat_evaluate (pTOpt, var);

  // copying changed weights to oldcellweight
  if ((*pTOpt).curcellweight[var])
    for (i = 0; i < (*pTOpt).cellchangedqty; i++)
    {
      cell = (*pTOpt).cellchanged[i];
      (*pTOpt).oldcellweight[var][cell] = (*pTOpt).curcellweight[var][cell];
    }

  return;
}
