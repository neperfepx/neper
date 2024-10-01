/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_stat_.h"

void
net_tess_opt_comp_objective_fval_gen_stat (struct TOPT *pTOpt, int var)
{
  if ((*pTOpt).iter == 1)
    net_tess_opt_comp_objective_fval_gen_stat_alloc (pTOpt, var);

  if ((*pTOpt).cvlsig[var] > 0)
  {
    if ((*pTOpt).iter == (*pTOpt).loop_start[(*pTOpt).loop - 1]
        || (*pTOpt).iter % 1000 == 1)
    {
      if (!strcmp ((*pTOpt).cvlmethod, "numerical"))
        net_tess_opt_comp_objective_fval_gen_stat_smoothed_gen_legacy (pTOpt,
                                                                       var);
      else if (!strcmp ((*pTOpt).cvlmethod, "analytical"))
        net_tess_opt_comp_objective_fval_gen_stat_smoothed_comp (pTOpt, var);
      else
        ut_print_neperbug ();
    }

    else
    {
      if (!strcmp ((*pTOpt).cvlmethod, "numerical"))
        net_tess_opt_comp_objective_fval_gen_stat_smoothed_update_legacy
          (pTOpt, var);
      else if (!strcmp ((*pTOpt).cvlmethod, "analytical"))
        net_tess_opt_comp_objective_fval_gen_stat_smoothed_update (pTOpt,
                                                                   var);
      else
        ut_print_neperbug ();
    }
  }

  if ((*pTOpt).cvlsig[var] == 0 || strstr ((*pTOpt).TDyn.logdis, "curcdf0")
      || strstr ((*pTOpt).TDyn.logval, "val0"))
    net_tess_opt_comp_objective_fval_gen_stat_unsmoothed_comp (pTOpt, var);

  net_tess_opt_comp_objective_fval_gen_stat_evaluate (pTOpt, var);

  return;
}
