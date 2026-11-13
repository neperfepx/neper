/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_fval_comp_.h"

// factor applied to the penalty (which measures how far the cells are from
// being valid) when combined with the objective function, so that making the
// cells valid prevails over decreasing the objective function
static const double penalty_fact = 1000;

// The objective function combines, in quadrature, its error part (the
// quadratic mean of the target values) and the total penalty:
//   O = sqrt (R^2 + (k p)^2),
// and the total error of cell i is
//   Delta_i = sqrt ((1/T) sum_t delta_{t,i}^2 + N k^2 p_i^2),
// so that O is the quadratic mean of the total cell errors (see the
// developer documentation).

double
net_tess_opt_comp_minimize_objective_fval_comp (struct TOPT *pTOpt)
{
  int i;

  // computing global objective function
  if ((*pTOpt).tarqty > 0)
    (*pTOpt).objval =
      ut_array_1d_lmean_expr ((*pTOpt).curval, (*pTOpt).tarqty,
                              (*pTOpt).objective);
  else
    (*pTOpt).objval = 0;

  // combining the total penalty in quadrature (not done if zero, so that
  // the value without penalty is exactly the error part)
  if ((*pTOpt).curpenalty > 0)
    (*pTOpt).objval =
      sqrt (pow ((*pTOpt).objval, 2) +
            pow (penalty_fact * (*pTOpt).curpenalty, 2));

  // total cell errors
  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    int t;
    double sum = 0;

    for (t = 0; t < (*pTOpt).tarqty; t++)
      sum += pow ((*pTOpt).curcellerror[t][i], 2);

    (*pTOpt).curcelltotalerror[i] =
      sqrt (((*pTOpt).tarqty > 0 ? sum / (*pTOpt).tarqty : 0)
            + (*pTOpt).CellQty * pow (penalty_fact * (*pTOpt).curcellpenalty[i], 2));
  }

  (*pTOpt).objvalmin = ut_realloc_1d ((*pTOpt).objvalmin, (*pTOpt).iter + 1);
  (*pTOpt).objvalmin[(*pTOpt).iter] =
    ((*pTOpt).iter == 0) ? DBL_MAX : (*pTOpt).objvalmin[(*pTOpt).iter];

  (*pTOpt).objvalmin[(*pTOpt).iter] =
    (*pTOpt).iter == 0 ? (*pTOpt).objval : ut_num_min ((*pTOpt).objvalmin[(*pTOpt).iter - 1], (*pTOpt).objval);

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
      ((*pTOpt).iter == 0) ? DBL_MAX : (*pTOpt).objvalmin0[(*pTOpt).iter];

    (*pTOpt).objvalmin0[(*pTOpt).iter] =
      ut_num_min ((*pTOpt).objvalmin0[(*pTOpt).iter], (*pTOpt).objval0);
  }

  return (*pTOpt).objval;
}
