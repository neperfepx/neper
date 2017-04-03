/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_comp_.h"

double
net_tess_opt_comp_objective_fval_comp (struct TOPT *pTOpt)
{
  int i;

  // computing individual objective functions
  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    if (!strcmp ((*pTOpt).tartype[i], "stat"))
      net_tess_opt_comp_objective_fval_comp_stat (pTOpt, i);
    else
      net_tess_opt_comp_objective_fval_comp_celldata (pTOpt, i);

    if (isnan ((*pTOpt).curval[i]))
    {
      printf ("(*pTOpt).curval[%d] is not-a-number.\n", i);
      ut_error_reportbug ();
    }
  }

  // computing global objective function

  (*pTOpt).objval = 0;
  for (i = 0; i < (*pTOpt).tarqty; i++)
    (*pTOpt).objval += pow ((*pTOpt).curval[i], 2);
  (*pTOpt).objval = sqrt ((*pTOpt).objval);

  (*pTOpt).objvalmin = ut_realloc_1d ((*pTOpt).objvalmin, (*pTOpt).iter + 1);
  (*pTOpt).objvalmin[(*pTOpt).iter]
    = ((*pTOpt).iter == 1) ? DBL_MAX : (*pTOpt).objvalmin[(*pTOpt).iter - 1];

  (*pTOpt).objvalmin[(*pTOpt).iter]
    = ut_num_min ((*pTOpt).objvalmin[(*pTOpt).iter], (*pTOpt).objval);

  return (*pTOpt).objval;
}
