/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_.h"


double
net_tess_opt_comp_objective_fval (struct TOPT *pTOpt)
{
  int i;
  struct timeval t1, t2, t3, t4, t5;

  gettimeofday (&t1, NULL);

  // Initializing problem & computing penalties ------------------------
  net_tess_opt_comp_objective_fval_init (pTOpt);

  gettimeofday (&t2, NULL);

  // updating penalties for changed cells

  if (!strcmp ((*pTOpt).optitype, "morpho"))
    net_tess_opt_comp_objective_fval_cellpenalty (pTOpt);

  gettimeofday (&t3, NULL);

  (*pTOpt).TDyn.val_val_cellval_dur = 0;
  (*pTOpt).TDyn.val_val_comp_dur = 0;
  // Computing curcellval and curval -----------------------------------
  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    if (!strcmp ((*pTOpt).tarvar[i], "tesr"))
      net_tess_opt_comp_objective_fval_tesr (pTOpt, i);
    else
      net_tess_opt_comp_objective_fval_gen (pTOpt, i);
  }

  gettimeofday (&t4, NULL);

  // Computing objval --------------------------------------------------
  net_tess_opt_comp_objective_fval_comp (pTOpt);

  gettimeofday (&t5, NULL);

  (*pTOpt).TDyn.val_init_dur = ut_time_subtract (&t1, &t2);
  (*pTOpt).TDyn.val_penalty_dur = ut_time_subtract (&t2, &t3);
  (*pTOpt).TDyn.val_val_dur = ut_time_subtract (&t3, &t4);
  (*pTOpt).TDyn.val_comp_dur = ut_time_subtract (&t4, &t5);

  return (*pTOpt).objval;
}
