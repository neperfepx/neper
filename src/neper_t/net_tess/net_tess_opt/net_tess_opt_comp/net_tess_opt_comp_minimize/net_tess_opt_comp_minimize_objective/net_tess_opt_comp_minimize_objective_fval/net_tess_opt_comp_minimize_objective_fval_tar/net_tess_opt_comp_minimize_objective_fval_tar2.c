/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_fval_tar_.h"

void
net_tess_opt_comp_minimize_objective_fval_tar_size (struct TOPT *pTOpt, int var,
                                                    int cell)
{
  if ((*pTOpt).Dim == 2)
    neut_polys_volume_2d ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                          (*pTOpt).CellSCellQty[cell],
                          ((*pTOpt).Dom).PseudoSize,
                          (*pTOpt).curcellval[var][cell]);
  else
    neut_polys_volume ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                       (*pTOpt).CellSCellQty[cell],
                       (*pTOpt).curcellval[var][cell]);

  if ((*pTOpt).CellSize)
    (*pTOpt).CellSize[cell] = (*pTOpt).curcellval[var][cell][0];

  (*pTOpt).curcellval[var][cell][0] /= (*pTOpt).tarrefval[var];

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_diameq (struct TOPT *pTOpt, int var,
                                                      int cell)
{
  if ((*pTOpt).Dim == 2)
  {
    neut_polys_diameq_2d ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                          (*pTOpt).CellSCellQty[cell],
                          ((*pTOpt).Dom).PseudoSize,
                          (*pTOpt).curcellval[var][cell]);

    if ((*pTOpt).CellSize)
      (*pTOpt).CellSize[cell] =
        M_PI / 4 * pow ((*pTOpt).curcellval[var][cell][0], 2);
  }
  else
  {
    neut_polys_diameq ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                       (*pTOpt).CellSCellQty[cell],
                       (*pTOpt).curcellval[var][cell]);

    if ((*pTOpt).CellSize)
      (*pTOpt).CellSize[cell] =
        M_PI / 6 * pow ((*pTOpt).curcellval[var][cell][0], 3);
  }

  (*pTOpt).curcellval[var][cell][0] /= (*pTOpt).tarrefval[var];

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_sphericity (struct TOPT *pTOpt, int var,
                                                          int cell)
{
  if ((*pTOpt).Dim == 2)
  {
    if ((*pTOpt).CellSize)
      neut_polys_size_sphericity_2d ((*pTOpt).Poly,
                                     (*pTOpt).CellSCellList[cell],
                                     (*pTOpt).CellSCellQty[cell],
                                     (*pTOpt).CellSize[cell],
                                     (*pTOpt).curcellval[var][cell]);
    else
      neut_polys_sphericity_2d ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                                (*pTOpt).CellSCellQty[cell],
                                ((*pTOpt).Dom).PseudoSize,
                                (*pTOpt).curcellval[var][cell]);
  }

  else
  {
    if ((*pTOpt).CellSize)
      neut_polys_size_sphericity ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                                  (*pTOpt).CellSCellQty[cell],
                                  (*pTOpt).CellSize[cell],
                                  (*pTOpt).curcellval[var][cell]);
    else
      neut_polys_sphericity ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                             (*pTOpt).CellSCellQty[cell],
                             (*pTOpt).curcellval[var][cell]);
  }

  if (!strcmp ((*pTOpt).tarvar[var], "1-sphericity"))
    (*pTOpt).curcellval[var][cell][0] = 1 - (*pTOpt).curcellval[var][cell][0];

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_anisofact (struct TOPT *pTOpt, int var,
                                                         int cell)
{
  if ((*pTOpt).Dim == 2)
    neut_polys_anisofact_2d ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                              (*pTOpt).CellSCellQty[cell],
                              (*pTOpt).curcellval[var][cell]);

  else
    neut_polys_anisofact ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                           (*pTOpt).CellSCellQty[cell],
                           (*pTOpt).curcellval[var][cell]);

  if (!strcmp ((*pTOpt).tarvar[var], "anisofact-1"))
    (*pTOpt).curcellval[var][cell][0] = (*pTOpt).curcellval[var][cell][0] - 1;

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_convexity (struct TOPT *pTOpt, int var,
                                                         int cell)
{
  neut_polys_convexity ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                        (*pTOpt).CellSCellQty[cell],
                        (*pTOpt).curcellval[var][cell]);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_centroid (struct TOPT *pTOpt, int var,
                                                        int cell)
{
  double *coo = ut_alloc_1d (3);

  neut_polys_centroid ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                       (*pTOpt).CellSCellQty[cell], coo);
  ut_array_1d_memcpy (coo, (*pTOpt).Dim, (*pTOpt).curcellval[var][cell]);

  ut_free_1d (&coo);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_centroidsize (struct TOPT *pTOpt,
                                                            int var, int cell)
{
  double *coo = ut_alloc_1d (3);

  neut_polys_centroid ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                       (*pTOpt).CellSCellQty[cell], coo);
  ut_array_1d_memcpy (coo, (*pTOpt).Dim, (*pTOpt).curcellval[var][cell]);

  if ((*pTOpt).Dim == 2)
    neut_polys_diameq_2d ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                          (*pTOpt).CellSCellQty[cell],
                          ((*pTOpt).Dom).PseudoSize,
                          (*pTOpt).curcellval[var][cell] + (*pTOpt).Dim);
  else
    neut_polys_diameq ((*pTOpt).Poly, (*pTOpt).CellSCellList[cell],
                       (*pTOpt).CellSCellQty[cell],
                       (*pTOpt).curcellval[var][cell] + (*pTOpt).Dim);

  ut_free_1d (&coo);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_ori (struct TOPT *pTOpt, int var,
                                                   int cell)
{
  ut_array_1d_memcpy ((*pTOpt).SSet.SeedOriR[cell], 3, (*pTOpt).curcellval[var][cell]);

  return;
}

// Computing the value of a cell, from the distances of its points of the
// raster tessellation to its boundaries
void
net_tess_opt_comp_minimize_objective_fval_tar_tesr (struct TOPT *pTOpt, int var,
                                                    int cell)
{
  int i;

  // the points are distributed as tasks, so that the threads which have
  // finished their cells (of the enclosing parallel loop on the cells)
  // contribute (there are few changed cells, but many points)
#ifdef HAVE_OPENMP
#pragma omp taskloop
#endif // HAVE_OPENMP
  for (i = 0; i < (*pTOpt).tarcellptqty[cell]; i++)
    neut_polys_point_dist ((*pTOpt).Poly, (*pTOpt).tarcellptscells[cell][i],
                           (*pTOpt).CellSCellQty[cell],
                           (*pTOpt).tarcellpts[cell][i],
                           (*pTOpt).tarcellptsdist[cell] + i);

  (*pTOpt).curcellval[var][cell][0] = 0;

  if (!strcmp ((*pTOpt).objective_tesrval, "bounddist"))
    for (i = 0; i < (*pTOpt).tarcellptqty[cell]; i++)
      (*pTOpt).curcellval[var][cell][0] +=
        (*pTOpt).tarcellptweights[cell][i] *
        pow ((*pTOpt).tarcellptsdist[cell][i], 2);

  else if (!strcmp ((*pTOpt).objective_tesrval, "intervol"))
    for (i = 0; i < (*pTOpt).tarcellptqty[cell]; i++)
      (*pTOpt).curcellval[var][cell][0] +=
        (*pTOpt).tarcellptweights[cell][i] *
        (*pTOpt).tarcellptsdist[cell][i];

  else
    abort ();

  (*pTOpt).curcellval[var][cell][0] *= (*pTOpt).tarcellfact[cell];

  return;
}
