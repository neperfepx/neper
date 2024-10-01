/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_comp_.h"

double
net_tess_opt_comp_objective_fval_comp (struct TOPT *pTOpt)
{
  int i;

  // computing global objective function
  if ((*pTOpt).tarqty > 0)
    (*pTOpt).objval =
      ut_array_1d_lmean_expr ((*pTOpt).curval, (*pTOpt).tarqty,
                              (*pTOpt).objective);
  else
    (*pTOpt).objval = 0;
  // (*pTOpt).objval = ut_array_1d_norm ((*pTOpt).curval, (*pTOpt).tarqty);

  (*pTOpt).objvalmin = ut_realloc_1d ((*pTOpt).objvalmin, (*pTOpt).iter + 1);
  (*pTOpt).objvalmin[(*pTOpt).iter] =
    ((*pTOpt).iter == 1) ? DBL_MAX : (*pTOpt).objvalmin[(*pTOpt).iter - 1];

  (*pTOpt).objvalmin[(*pTOpt).iter] =
    ut_num_min ((*pTOpt).objvalmin[(*pTOpt).iter], (*pTOpt).objval);

  if (strstr ((*pTOpt).TDyn.logval, "val0")
      || strstr ((*pTOpt).TDyn.logval, "valmin0"))
  {
    (*pTOpt).objval0 = 0;
    for (i = 0; i < (*pTOpt).tarqty; i++)
      (*pTOpt).objval0 += pow ((*pTOpt).curval0[i], 2);
    (*pTOpt).objval0 = sqrt ((*pTOpt).objval0);

    (*pTOpt).objvalmin0 =
      ut_realloc_1d ((*pTOpt).objvalmin0, (*pTOpt).iter + 1);
    (*pTOpt).objvalmin0[(*pTOpt).iter] =
      ((*pTOpt).iter == 1) ? DBL_MAX : (*pTOpt).objvalmin0[(*pTOpt).iter - 1];

    (*pTOpt).objvalmin0[(*pTOpt).iter] =
      ut_num_min ((*pTOpt).objvalmin0[(*pTOpt).iter], (*pTOpt).objval0);
  }

  return (*pTOpt).objval;
}
