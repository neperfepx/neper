/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_comp_stat_.h"

void
net_tess_opt_comp_objective_fval_comp_stat (struct TOPT *pTOpt, int var)
{
  int i, j, cell;
  double val;

  if ((*pTOpt).iter == 1)
  {
    ut_fct_set_numerical ((*pTOpt).curpdf + var,
			  (*pTOpt).tarpdf[var].min,
			  (*pTOpt).tarpdf[var].max,
			  (*pTOpt).tarpdf[var].size);
    ut_fct_set_numerical ((*pTOpt).curcdf0 + var,
			  (*pTOpt).tarpdf[var].min,
			  (*pTOpt).tarpdf[var].max,
			  (*pTOpt).tarpdf[var].size);
  }

  if ((*pTOpt).dissmoothing > 0)
  {
    if ((*pTOpt).iter == 1)
    {
      ut_array_1d_zero ((*pTOpt).curpdf[var].y,
			(*pTOpt).curpdf[var].size);

      for (j = 1; j <= (*pTOpt).CellQty; j++)
      {
	if ((*pTOpt).curcellpenalty[j] == 0)
	  for (i = 0; i < (*pTOpt).curpdf[var].size; i++)
	    (*pTOpt).curpdf[var].y[i] +=
	      ut_fct_eval ((*pTOpt).cvl[var], (*pTOpt).curpdf[var].x[i] -
			   (*pTOpt).curcellval[var][j][0]) / (*pTOpt).CellQty;
	else
	  (*pTOpt).curpenalty[var] += (*pTOpt).curcellpenalty[j];
      }
    }

    else
      for (j = 0; j < (*pTOpt).cellchangedqty; j++)
      {
	cell = (*pTOpt).cellchanged[j];

	// removing old contribution to curpdf
	if ((*pTOpt).oldcellpenalty[cell] == 0)
	  for (i = 0; i < (*pTOpt).curpdf[var].size; i++)
	  {
	    val = ut_fct_eval ((*pTOpt).cvl[var], (*pTOpt).curpdf[var].x[i] -
			       (*pTOpt).oldcellval[var][cell][0])
			      / (*pTOpt).CellQty;
	    (*pTOpt).curpdf[var].y[i] -= val;

	    // if we have passed the nominal value and the function has
	    // reached 0, stopping.
	    if ((*pTOpt).curpdf[var].x[i] >
		(*pTOpt).oldcellval[var][cell][0] && val == 0)
	      break;
	  }
	(*pTOpt).curpenalty[var] -= (*pTOpt).oldcellpenalty[cell];

	// adding new contribution to curpdf
	if ((*pTOpt).curcellpenalty[cell] == 0)
	  for (i = 0; i < (*pTOpt).curpdf[var].size; i++)
	  {
	    val = ut_fct_eval ((*pTOpt).cvl[var], (*pTOpt).curpdf[var].x[i] -
			       (*pTOpt).curcellval[var][cell][0])
			      / (*pTOpt).CellQty;
	    (*pTOpt).curpdf[var].y[i] += val;

	    // if we have passed the nominal value and the function has
	    // reached 0, stopping.
	    if ((*pTOpt).curpdf[var].x[i] >
		(*pTOpt).curcellval[var][cell][0] && val == 0)
	      break;
	  }
	(*pTOpt).curpenalty[var] += (*pTOpt).curcellpenalty[cell];
      }

    ut_fct_integralfct ((*pTOpt).curpdf[var], (*pTOpt).curcdf + var);
  }

  if (strstr ((*pTOpt).TDyn.logdis, "curcdf0"))
  {
    ut_array_1d_zero ((*pTOpt).curcdf0[var].y,
		      (*pTOpt).curcdf0[var].size);
    for (j = 1; j <= (*pTOpt).CellQty; j++)
      if ((*pTOpt).oldcellpenalty[j] == 0)
	for (i = 0; i < (*pTOpt).curcdf0[var].size; i++)
	  if ((*pTOpt).curcdf0[var].x[i] > (*pTOpt).curcellval[var][j][0])
	    (*pTOpt).curcdf0[var].y[i] += 1. / (*pTOpt).CellQty;
  }

  net_tess_opt_comp_objective_fval_comp_stat_evaluate (pTOpt, var);

  return;
}
