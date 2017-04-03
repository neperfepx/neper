/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_comp_stat_.h"

void
net_tess_opt_comp_objective_fval_comp_stat_evaluate (struct TOPT *pTOpt, int var)
{
  if (!strcmp ((*pTOpt).objective, "default")
        || !strcmp ((*pTOpt).objective, "FL2w"))
    net_tess_opt_comp_objective_fval_comp_stat_evaluate_FL2w (pTOpt, var);

  else if (!strcmp ((*pTOpt).objective, "ad"))
    net_tess_opt_comp_objective_fval_comp_stat_evaluate_ad (pTOpt, var);

  else if (!strcmp ((*pTOpt).objective, "cvm"))
    net_tess_opt_comp_objective_fval_comp_stat_evaluate_cvm (pTOpt, var);

  else if (!strcmp ((*pTOpt).objective, "FL2"))
    net_tess_opt_comp_objective_fval_comp_stat_evaluate_FL2 (pTOpt, var);

  else if (!strcmp ((*pTOpt).objective, "chi2"))
    net_tess_opt_comp_objective_fval_comp_stat_evaluate_chi2 (pTOpt, var);

  else if (!strcmp ((*pTOpt).objective, "ks"))
    net_tess_opt_comp_objective_fval_comp_stat_evaluate_ks (pTOpt, var);

  else if (!strcmp ((*pTOpt).objective, "kuiper"))
    net_tess_opt_comp_objective_fval_comp_stat_evaluate_kuiper (pTOpt, var);

  else
    ut_print_message (2, 3, "Unknown test `%s'.", (*pTOpt).objective);

  if ((*pTOpt).curpenalty[var] > 1e-6)
    (*pTOpt).curval[var] += 1000 * (*pTOpt).curpenalty[var];

  return;
}
