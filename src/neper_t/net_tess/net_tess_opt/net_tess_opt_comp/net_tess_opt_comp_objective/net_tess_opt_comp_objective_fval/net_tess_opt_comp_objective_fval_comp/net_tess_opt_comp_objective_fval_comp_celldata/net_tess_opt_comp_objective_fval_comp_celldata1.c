/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_comp_celldata_.h"

void
net_tess_opt_comp_objective_fval_comp_celldata (struct TOPT *pTOpt, int id)
{
  int i;
  double val, val2, curval, avdiameq;

  if (!strncmp ((*pTOpt).tarvar[id], "size", 4)
      || !strcmp ((*pTOpt).tarvar[id], "sphericity"))
  {
    curval = 0;
    for (i = 1; i <= (*pTOpt).CellQty; ++i)
    {
      if ((*pTOpt).curcellpenalty[i] == 0)
      {
	if ((*pTOpt).tarcellvalqty[id] == 1)
	  val = (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][0];

	else if ((*pTOpt).tarcellvalqty[id] == 2)
	{
	  if ((*pTOpt).curcellval[id][i][0] < (*pTOpt).tarcellval[id][i][0])
	    val = (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][0];
	  else if ((*pTOpt).curcellval[id][i][0] < (*pTOpt).tarcellval[id][i][1])
	    val = 0;
	  else
	    val = (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][1];
	}

	else
	  abort ();
      }
      else
	val = 1000;

      curval += pow (val, 2);
    }
    curval = sqrt (curval / (*pTOpt).CellQty);

    curval /= (*pTOpt).Dim;
  }

  else if (!strcmp ((*pTOpt).tarvar[id], "centroid"))
  {
    curval = 0;
    for (i = 1; i <= (*pTOpt).CellQty; ++i)
    {
      // this happens when a cell is void
      if ((*pTOpt).curcellpenalty[i] == 0)
      {
	if ((*pTOpt).Dim == 2)
	  val = ut_space_dist2d ((*pTOpt).curcellval[id][i],
				 (*pTOpt).tarcellval[id][i]);
	else
	  val = ut_space_dist ((*pTOpt).curcellval[id][i],
			       (*pTOpt).tarcellval[id][i]);
      }
      else
	val = 1000 * (*pTOpt).curcellpenalty[i];

      val /= (*pTOpt).tarrefval[id];

      if (!strcmp ((*pTOpt).objective, "L2"))
	curval += pow (val, 2);
      else if (!strcmp ((*pTOpt).objective, "L1"))
	curval += val;
      else if (!strcmp ((*pTOpt).objective, "Linf"))
	curval = ut_num_max (curval, val);
    }
    if (!strcmp ((*pTOpt).objective, "L2"))
      curval = sqrt (curval) / (*pTOpt).CellQty;
    else if (!strcmp ((*pTOpt).objective, "L1"))
      curval = curval / (*pTOpt).CellQty;
    else if (!strcmp ((*pTOpt).objective, "Linf"))
    {
    }
    else
      abort ();
  }

  else if (!strcmp ((*pTOpt).tarvar[id], "centroidsize")
        || !strcmp ((*pTOpt).tarvar[id], "centroiddiameq"))
  {
    curval = 0;
    for (i = 1; i <= (*pTOpt).CellQty; ++i)
    {
      // this happens when a cell is void
      if ((*pTOpt).curcellpenalty[i] == 0)
      {
	if ((*pTOpt).Dim == 2)
	  val = ut_space_dist2d ((*pTOpt).curcellval[id][i],
				 (*pTOpt).tarcellval[id][i]);
	else
	  val = ut_space_dist ((*pTOpt).curcellval[id][i],
			       (*pTOpt).tarcellval[id][i]);
	val2 =
	  (*pTOpt).tarcellval[id][i][(*pTOpt).Dim] -
	  (*pTOpt).curcellval[id][i][(*pTOpt).Dim];
      }
      else
      {
	val = 1000 * (*pTOpt).curcellpenalty[i];
	val2 = 1000 * (*pTOpt).curcellpenalty[i];
      }

      curval += pow (val, 2) + 0.25 * pow (val2, 2);
    }
    neut_tess_cellavdiameq ((*pTOpt).Dom, (*pTOpt).CellQty, &avdiameq);
    curval = sqrt (curval) / ((*pTOpt).CellQty * avdiameq);
  }

  else
    abort ();

  (*pTOpt).curval[id] = curval;

  return;
}
