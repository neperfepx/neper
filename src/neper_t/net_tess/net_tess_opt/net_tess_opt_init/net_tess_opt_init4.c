/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init_target_cdf (int disbinqty, double binwidth, double *pdf,
			      double *cdf)
{
  int i;

  cdf[0] = 0;
  for (i = 1; i < disbinqty; i++)
    cdf[i] = cdf[i - 1] + pdf[i] * binwidth;

  return;
}

void
net_tess_opt_init_tesrobj_pts (struct TOPT *pTOpt)
{
  int i, j;

  int **pts = NULL;

  char *message = ut_alloc_1d_char (100);

  ut_print_message (0, 4, "Listing cell voxels (%s)... ", (*pTOpt).objective);

  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    ut_print_progress (stdout, i, (*pTOpt).CellQty, "%5.1f%%", message);

    if (!strncmp ((*pTOpt).objective, "surf", 4))
      neut_tesr_cell_boundpoints ((*pTOpt).tartesr, i,
				  &pts, &(*pTOpt).tarcellptqty[i],
				  (*pTOpt).faceconn);

    else if (!strncmp ((*pTOpt).objective, "vol", 3) || (*pTOpt).tarcellptqty[i] == 0)	// take them all if empty bounds
      neut_tesr_cell_points ((*pTOpt).tartesr, i,
			     &pts, &(*pTOpt).tarcellptqty[i]);

    (*pTOpt).tarcellpts[i] = ut_alloc_2d_double ((*pTOpt).tarcellptqty[i], 3);

    for (j = 0; j < (*pTOpt).tarcellptqty[i]; j++)
      neut_tesr_coo ((*pTOpt).tartesr, pts[j], (*pTOpt).tarcellpts[i][j]);

    ut_free_2d_int_ (&pts, (*pTOpt).tarcellptqty[i]);
  }

  ut_free_1d_char (message);

  return;
}
