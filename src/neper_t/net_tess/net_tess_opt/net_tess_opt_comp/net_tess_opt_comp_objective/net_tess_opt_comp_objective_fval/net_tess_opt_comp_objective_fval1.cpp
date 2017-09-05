/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_.h"
#include<ANN/ANN.h>

double
net_tess_opt_comp_objective_fval (struct TOPT *pTOpt)
{
  // initializing problem

  net_tess_opt_comp_objective_fval_init (pTOpt);

  // updating penalties for changed cells

  net_tess_opt_comp_objective_fval_cellpenalty (pTOpt);

  // case of a tesr
  if ((*pTOpt).tarqty == 1 && !strcmp ((*pTOpt).tarvar[0], "tesr"))
    net_tess_opt_comp_objective_fval_tesr (pTOpt);

  // other cases
  else
  {
    // updating values for changed cells

    net_tess_opt_comp_objective_fval_cellval (pTOpt);

    // computing objective function

    net_tess_opt_comp_objective_fval_comp (pTOpt);
  }

  return (*pTOpt).objval;
}
