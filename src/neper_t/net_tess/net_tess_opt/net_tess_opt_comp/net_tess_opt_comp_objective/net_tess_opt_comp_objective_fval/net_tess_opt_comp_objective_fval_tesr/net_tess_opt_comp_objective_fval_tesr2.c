/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_tesr_.h"

void
net_tess_opt_comp_objective_fval_tesr_pre (struct TOPT TOpt,
					   int **pcellcomped,
					   int *pcellcompedqty)
{
  int i, cell;

  for (i = 0; i < TOpt.cellchangedqty; i++)
  {
    cell = TOpt.cellchanged[i];
    if (TOpt.curcellpenalty[cell] == 0)
      ut_array_1d_int_list_addelt_nocheck (pcellcomped, pcellcompedqty, cell);
  }

  return;
}

void
net_tess_opt_comp_objective_fval_tesr_dist (int *cells, int cellqty, struct TOPT *pTOpt)
{
  int i, j, cell, pt, distqty = 0;
  for (i = 0; i < cellqty; i++)
    distqty += (*pTOpt).tarcellptqty[cells[i]];

  // getting ready for openmp
  int id = 0;
  int *dist_cell = ut_alloc_1d_int (distqty);
  int *dist_pt = ut_alloc_1d_int (distqty);
  for (i = 0; i < cellqty; i++)
    for (j = 0; j < (*pTOpt).tarcellptqty[cells[i]]; j++)
    {
      dist_cell[id] = cells[i];
      dist_pt[id] = j;
      id++;
    }

#pragma omp parallel for private(cell,pt)
  for (i = 0; i < distqty; i++)
  {
    cell = dist_cell[i];
    pt = dist_pt[i];
    neut_polys_point_dist ((*pTOpt).Poly,
			   (*pTOpt).tarcellptscells[cell][pt],
			   (*pTOpt).CellSCellQty[cell],
			   (*pTOpt).tarcellpts[cell][pt],
			   (*pTOpt).tarcellptsdist[cell] + pt);
  }

  ut_free_1d_int (dist_cell);
  ut_free_1d_int (dist_pt);

  return;
}

void
net_tess_opt_comp_objective_fval_tesr_cellval (int *cellcomped,
					       int cellcompedqty,
					       struct TOPT *pTOpt)
{
  int i, j, cell;

  for (i = 0; i < cellcompedqty; i++)
  {
    cell = cellcomped[i];

    (*pTOpt).curcellval[0][cell][0] = 0;

    if (!strcmp ((*pTOpt).objective_tesrval, "bounddist"))
      for (j = 0; j < (*pTOpt).tarcellptqty[cell]; j++)
	(*pTOpt).curcellval[0][cell][0] += (*pTOpt).tarcellptweights[cell][j]
	                                 * pow ((*pTOpt).tarcellptsdist[cell][j], 2);

    else if (!strcmp ((*pTOpt).objective_tesrval, "intervol"))
      for (j = 0; j < (*pTOpt).tarcellptqty[cell]; j++)
	(*pTOpt).curcellval[0][cell][0] += (*pTOpt).tarcellptweights[cell][j]
	                                 * (*pTOpt).tarcellptsdist[cell][j];

    else
      abort ();

    (*pTOpt).curcellval[0][cell][0] *= (*pTOpt).tarcellfact[cell];
  }

  return;
}

void
net_tess_opt_comp_objective_fval_tesr_obj (struct TOPT *pTOpt)
{
  int i;
  double val, lv, size, avdiameq;

  neut_tess_cellavdiameq ((*pTOpt).Dom, (*pTOpt).CellQty, &avdiameq);

  if (!strcmp ((*pTOpt).objective_tesrval, "bounddist"))
  {
    val = 0;
    for (i = 1; i <= (*pTOpt).CellQty; ++i)
      if ((*pTOpt).curcellpenalty[i] == 0)
	val += (*pTOpt).curcellval[0][i][0];
      else
	val += 1000 * (*pTOpt).curcellpenalty[i];
    val = 2 / ((*pTOpt).tarptqtyini * avdiameq) * sqrt (val);
  }

  else if (!strcmp ((*pTOpt).objective_tesrval, "intervol"))
  {
    neut_tess_size ((*pTOpt).Dom, &size);
    neut_tesr_voxlengtheq ((*pTOpt).tartesr, &lv);

    val = 0;
    for (i = 1; i <= (*pTOpt).CellQty; ++i)
      if ((*pTOpt).curcellpenalty[i] == 0)
	val += (*pTOpt).curcellval[0][i][0];
      else
	val += 1000 * (*pTOpt).curcellpenalty[i];
    val = pow (lv, (*pTOpt).Dim - 1) / size * val;
  }

  else
    abort ();

  (*pTOpt).curval[0] = val;

  return;
}

void
net_tess_opt_comp_objective_fval_tesr_post (struct TOPT *pTOpt)
{
  // comp / global computation

  (*pTOpt).objval = (*pTOpt).curval[0];

  (*pTOpt).objvalmin = ut_realloc_1d ((*pTOpt).objvalmin, (*pTOpt).iter + 1);
  (*pTOpt).objvalmin[(*pTOpt).iter]
    = ((*pTOpt).iter == 1) ? DBL_MAX : (*pTOpt).objvalmin[(*pTOpt).iter - 1];

  (*pTOpt).objvalmin[(*pTOpt).iter]
    = ut_num_min ((*pTOpt).objvalmin[(*pTOpt).iter], (*pTOpt).objval);

  return;
}
