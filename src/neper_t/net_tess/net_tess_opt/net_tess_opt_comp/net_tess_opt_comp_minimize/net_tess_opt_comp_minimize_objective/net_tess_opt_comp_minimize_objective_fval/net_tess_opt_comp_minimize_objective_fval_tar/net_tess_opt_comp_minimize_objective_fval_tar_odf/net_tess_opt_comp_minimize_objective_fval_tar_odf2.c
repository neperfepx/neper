/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_fval_tar_odf_.h"

void
net_tess_opt_comp_minimize_objective_fval_tar_odf_evaluate (struct TOPT *pTOpt, struct ODF Odf,
                                                    int var)
{
  int i;

  if (!strcmp ((*pTOpt).tarobjective[var], "default"))
  {
    (*pTOpt).curval[var] = 0;
    for (i = 0; i < (*pTOpt).tarOdf.odfqty; i++)
      (*pTOpt).curval[var] += (*pTOpt).tarOdf.Sp.Mesh[3].EltWeight[i + 1] * pow ((*pTOpt).tarOdf.odf[i] - Odf.odf[i], 2);
    (*pTOpt).curval[var] /= ut_array_1d_sum ((*pTOpt).tarOdf.Sp.Mesh[3].EltWeight + 1, (*pTOpt).tarOdf.odfqty);
    if (isnan ((*pTOpt).curval[var]))
      (*pTOpt).curval[var] = 1000;
    (*pTOpt).curval[var] = sqrt ((*pTOpt).curval[var]);
  }

  else
    ut_print_message (2, 3, "Unknown test `%s'.", (*pTOpt).tarobjective[var]);

  return;
}
