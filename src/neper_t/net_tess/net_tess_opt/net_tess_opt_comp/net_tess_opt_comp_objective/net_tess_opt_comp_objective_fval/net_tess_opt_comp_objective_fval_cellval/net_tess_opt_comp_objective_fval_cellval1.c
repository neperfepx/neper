/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_cellval_.h"

void
net_tess_opt_comp_objective_fval_cellval (struct TOPT *pTOpt)
{
  int i, j, cell;

  for (i = 0; i < (*pTOpt).tarqty; i++)
    for (j = 0; j < (*pTOpt).cellchangedqty; j++)
    {
      cell = (*pTOpt).cellchanged[j];

      if ((*pTOpt).curcellpenalty[cell] == 0)
      {
	if (!strcmp ((*pTOpt).tarvar[i], "size"))
	  net_tess_opt_comp_objective_fval_cellval_size (pTOpt, i, cell);

	else if (!strcmp ((*pTOpt).tarvar[i], "diameq"))
	  net_tess_opt_comp_objective_fval_cellval_diameq (pTOpt, i, cell);

	else if (!strcmp ((*pTOpt).tarvar[i], "sphericity"))
	  net_tess_opt_comp_objective_fval_cellval_sphericity (pTOpt, i, cell);

	else if (!strcmp ((*pTOpt).tarvar[i], "convexity"))
	  net_tess_opt_comp_objective_fval_cellval_convexity (pTOpt, i, cell);

	else if (!strcmp ((*pTOpt).tarvar[i], "centroid"))
	  net_tess_opt_comp_objective_fval_cellval_centroid (pTOpt, i, cell);

	else if (!strcmp ((*pTOpt).tarvar[i], "centroidsize")
	      || !strcmp ((*pTOpt).tarvar[i], "centroiddiameq"))
	  net_tess_opt_comp_objective_fval_cellval_centroidsize (pTOpt, i, cell);

	else
	  abort ();
      }
    }

  return;
}
