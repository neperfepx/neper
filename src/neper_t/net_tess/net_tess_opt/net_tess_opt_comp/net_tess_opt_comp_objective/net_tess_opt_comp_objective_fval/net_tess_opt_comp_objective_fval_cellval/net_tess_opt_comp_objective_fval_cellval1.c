/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_cellval_.h"

void
net_tess_opt_comp_objective_fval_cellval (struct TOPT *pTOpt)
{
  int i, j, cell, scell, wrong, faceqty, contiguous, faceqty_cell = 0;
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

  for (i = 0; i < (*pTOpt).tarqty; i++)
  {
    wrong = 0;

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

	else if (!strcmp ((*pTOpt).tarvar[i], "tesr"))
	  wrong += net_tess_opt_comp_objective_fval_cellval_tesr (pTOpt, i, cell);

	else
	  abort ();
      }
    }

    if (!strcmp ((*pTOpt).tarvar[i], "tesr") && (*pTOpt).iter == 1)
    {
      if (wrong >= 0)
      {
	printf ("\bWrongly assigned voxels: %d\n", wrong);
	ut_print_message (0, 3, (char *) " ");
      }
    }
  }

  return;
}
