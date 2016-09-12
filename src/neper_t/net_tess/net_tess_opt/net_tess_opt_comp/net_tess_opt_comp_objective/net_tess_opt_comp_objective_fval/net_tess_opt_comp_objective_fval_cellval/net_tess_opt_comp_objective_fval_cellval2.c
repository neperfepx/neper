/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_cellval_.h"

void
net_tess_opt_comp_objective_fval_cellval_size (struct TOPT *pTOpt, int var,
					       int cell)
{
  if ((*pTOpt).Dim == 2)
    neut_polys_volume_2d ((*pTOpt).Poly,
			  (*pTOpt).CellSCellList[cell],
			  (*pTOpt).CellSCellQty[cell],
			  ((*pTOpt).Dom).PseudoSize,
			  (*pTOpt).curcellval[var][cell]);
  else
    neut_polys_volume ((*pTOpt).Poly,
		       (*pTOpt).CellSCellList[cell],
		       (*pTOpt).CellSCellQty[cell],
		       (*pTOpt).curcellval[var][cell]);

  (*pTOpt).curcellval[var][cell][0] /= (*pTOpt).tarrefval[var];

  return;
}

void
net_tess_opt_comp_objective_fval_cellval_diameq (struct TOPT *pTOpt, int var,
						int cell)
{
  if ((*pTOpt).Dim == 2)
    neut_polys_diameq_2d ((*pTOpt).Poly,
			  (*pTOpt).CellSCellList[cell],
			  (*pTOpt).CellSCellQty[cell],
			  ((*pTOpt).Dom).PseudoSize,
			  (*pTOpt).curcellval[var][cell]);
  else
    neut_polys_diameq ((*pTOpt).Poly,
		       (*pTOpt).CellSCellList[cell],
		       (*pTOpt).CellSCellQty[cell],
		       (*pTOpt).curcellval[var][cell]);

  (*pTOpt).curcellval[var][cell][0] /= (*pTOpt).tarrefval[var];

  return;
}

void
net_tess_opt_comp_objective_fval_cellval_sphericity (struct TOPT *pTOpt,
						     int var, int cell)
{
  if ((*pTOpt).Dim == 2)
    neut_polys_sphericity_2d ((*pTOpt).Poly,
			      (*pTOpt).CellSCellList[cell],
			      (*pTOpt).CellSCellQty[cell],
			      ((*pTOpt).Dom).PseudoSize,
			      (*pTOpt).curcellval[var][cell]);
  else
    neut_polys_sphericity ((*pTOpt).Poly,
			   (*pTOpt).CellSCellList[cell],
			   (*pTOpt).CellSCellQty[cell],
			   (*pTOpt).curcellval[var][cell]);

  return;
}

void
net_tess_opt_comp_objective_fval_cellval_convexity (struct TOPT *pTOpt,
						    int var, int cell)
{
  neut_polys_convexity ((*pTOpt).Poly,
			(*pTOpt).CellSCellList[cell],
			(*pTOpt).CellSCellQty[cell],
			(*pTOpt).curcellval[var][cell]);

  return;
}

void
net_tess_opt_comp_objective_fval_cellval_centroid (struct TOPT *pTOpt,
						   int var, int cell)
{
  double *coo = ut_alloc_1d (3);

  neut_polys_centroid ((*pTOpt).Poly,
		       (*pTOpt).CellSCellList[cell],
		       (*pTOpt).CellSCellQty[cell], coo);
  ut_array_1d_memcpy ((*pTOpt).curcellval[var][cell], (*pTOpt).Dim, coo);

  ut_free_1d (coo);

  return;
}

void
net_tess_opt_comp_objective_fval_cellval_centroidsize (struct TOPT *pTOpt,
						       int var, int cell)
{
  double *coo = ut_alloc_1d (3);

  neut_polys_centroid ((*pTOpt).Poly,
		       (*pTOpt).CellSCellList[cell],
		       (*pTOpt).CellSCellQty[cell], coo);
  ut_array_1d_memcpy ((*pTOpt).curcellval[var][cell], (*pTOpt).Dim, coo);

  if ((*pTOpt).Dim == 2)
    neut_polys_diameq_2d ((*pTOpt).Poly,
			  (*pTOpt).CellSCellList[cell],
			  (*pTOpt).CellSCellQty[cell],
			  ((*pTOpt).Dom).PseudoSize,
			  (*pTOpt).curcellval[var][cell] + (*pTOpt).Dim);
  else
    neut_polys_diameq ((*pTOpt).Poly,
		       (*pTOpt).CellSCellList[cell],
		       (*pTOpt).CellSCellQty[cell],
		       (*pTOpt).curcellval[var][cell] + (*pTOpt).Dim);

  ut_free_1d (coo);

  return;
}

int
net_tess_opt_comp_objective_fval_cellval_tesr (struct TOPT *pTOpt, int var,
					       int cell)
{
  int i;
  double dist;
  int wrong = 0;

  (*pTOpt).curcellval[var][cell][0] = 0;

  for (i = 0; i < (*pTOpt).tarcellptqty[cell]; i++)
  {
    neut_polys_point_dist ((*pTOpt).Poly,
			   (*pTOpt).tarcellptscells[cell][i],
			   (*pTOpt).CellSCellQty[cell],
			   (*pTOpt).tarcellpts[cell][i], &dist);

    if (!strcmp ((*pTOpt).objective, "vol"))
      (*pTOpt).curcellval[var][cell][0] += dist;
    else if (!strncmp ((*pTOpt).objective, "surf", 4))
      (*pTOpt).curcellval[var][cell][0] += pow (dist, 2.0);
    else
      abort ();

    if (dist > 0)
      wrong++;
  }

  if (!strncmp ((*pTOpt).objective, "surf", 4))
  {
    (*pTOpt).curcellval[var][cell][0] /= (*pTOpt).tarrefval[var];
    wrong = -1;
  }

  (*pTOpt).curcellval[var][cell][0] /= ((*pTOpt).tarrefval[var] *
					(*pTOpt).tarcellptqty[0]);

  return wrong;
}
