/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_cellpenalty_.h"

void
net_tess_opt_comp_objective_fval_cellpenalty (struct TOPT *pTOpt)
{
  int i, j, cell, scell, faceqty, contiguous, faceqty_cell = 0;
  double dist;
  struct POLY DomPoly;

  for (i = 0; i < (*pTOpt).cellchangedqty; i++)
  {
    cell = (*pTOpt).cellchanged[i];
    faceqty = INT_MAX;
    for (j = 0; j < (*pTOpt).CellSCellQty[cell]; j++)
    {
      scell = (*pTOpt).CellSCellList[cell][j];
      if ((*pTOpt).Poly[scell].FaceQty < faceqty)
      {
	faceqty = ((*pTOpt).Poly[scell]).FaceQty;
	faceqty_cell = scell;
      }
    }

    if (faceqty > 0)
    {
      int **setpoly = NULL;
      int *setpolyqty = NULL;
      int setqty;

      contiguous
	= neut_polys_contiguous ((*pTOpt).Poly,
	                         (*pTOpt).SSet.PerSeedSlaveQty,
	                         (*pTOpt).SSet.PerSeedSlaveNb,
				 (*pTOpt).CellSCellList[cell],
				 (*pTOpt).CellSCellQty[cell],
				 &setpoly, &setpolyqty, &setqty);

      ut_free_1d_int (setpolyqty);
      ut_free_2d_int (setpoly, setqty);

      if (contiguous)
	(*pTOpt).curcellpenalty[cell] = 0;
      else
	(*pTOpt).curcellpenalty[cell] = 1;
    }
    else
    {
      (*pTOpt).curcellpenalty[cell] = 0;
      for (j = 0; j < (*pTOpt).CellSCellQty[cell]; j++)
      {
	scell = (*pTOpt).CellSCellList[cell][j];
	(*pTOpt).curcellpenalty[cell] +=
	  neut_seedset_voidcell_distance ((*pTOpt).SSet, scell);
      }

      // In the case of a standard tessellation, the above distance may be
      // zero for a void cell, in the case where the seed is far from the
      // domain.  So, adding the distance to the domain as a penalty.
      if (strncmp (((*pTOpt).SSet).Type, "periodic", 8) != 0)
      {
	neut_poly_set_zero (&DomPoly);
	net_tess_poly ((*pTOpt).Dom, 1, &DomPoly);

	for (j = 0; j < (*pTOpt).CellSCellQty[cell]; j++)
	{
	  scell = (*pTOpt).CellSCellList[cell][j];
	  neut_poly_point_dist (DomPoly, ((*pTOpt).SSet).SeedCoo[scell],
				&dist);
	  (*pTOpt).curcellpenalty[cell] += dist;
	}

	neut_poly_free (&DomPoly);
      }
    }

    if (faceqty == 0 && (*pTOpt).curcellpenalty[cell] == 0)
    {
      neut_poly_set_zero (&DomPoly);
      net_tess_poly ((*pTOpt).Dom, 1, &DomPoly);

      ut_space_polypts_point_dist_verbosity (DomPoly.VerCoo + 1, DomPoly.VerQty,
					     (*pTOpt).SSet.SeedCoo[faceqty_cell],
					     &dist, 0);

      neut_poly_free (&DomPoly);

      ut_error_reportbug ();
    }
  }

  return;
}
