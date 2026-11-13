/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_polycomp_.h"
#include"neut_struct_nfcloud.hpp"
#include<float.h>

extern void net_tess3d_polycomp_poly (struct POLY Domain, struct SEEDSET SSet, NFTREE ** pnf_tree,
                                      int *ptid_seedid, int PolyId, double wcut, int *heavyseeds,
                                      int heavyseedqty, struct POLY *pPoly, struct TDYN *pTD);

void
net_tess3d_polycomp_cells_inittdyn (struct TDYN *pTD)
{
  int i;

  for (i = 0; i < (*pTD).cellchangedqty; i++)
    (*pTD).cellchangedflag[(*pTD).cellchanged[i]] = 0;
  for (i = 0; i < (*pTD).changedneighqty; i++)
    (*pTD).changedneighflag[(*pTD).changedneighs[i]] = 0;

  ut_free_1d_int (&(*pTD).cellchanged);
  (*pTD).cellchangedqty = 0;
  ut_free_1d_int (&(*pTD).changedneighs);
  (*pTD).changedneighqty = 0;

  return;
}

void
net_tess3d_polycomp_cells_updatecell (struct POLY Domain, struct SEEDSET SSet, NFTREE ** pnf_tree,
                                      int *ptid_seedid, int cell, double wcut, int *heavyseeds,
                                      int heavyseedqty, struct POLY **pPoly, struct TDYN *pTD)
{
  int changed;

#pragma omp atomic read
  changed = (*pTD).cellchangedflag[cell];

  if (!changed)
    net_tess3d_polycomp_poly (Domain, SSet, pnf_tree, ptid_seedid, cell, wcut, heavyseeds,
                              heavyseedqty, (*pPoly) + cell,
                              pTD);

  return;
}

// Recording the seeds of outlier (heavy) weights, heavyseeds (including the
// periodic slaves), and the maximum weight of the other seeds, wcut.  A
// weight is an outlier if it is higher than twice the weight of rank
// N / 1000 (from the top, at least the second highest weight), so that
// there are at most N / 1000 heavy seeds.  In net_tess3d_polycomp_poly, the
// heavy seeds are tested explicitly, and the neighbor search is stopped
// from wcut, so that a few seeds of high weights (typically, seeds that
// moved out of the domain during an optimization) do not slow down the
// computation of all cells.
void
net_tess3d_polycomp_cells_heavyseeds (struct SEEDSET SSet, double *pwcut, int **pheavyseeds,
                                      int *pheavyseedqty)
{
  int i, j, rank;
  double wq, wthres, *wtop = NULL;

  (*pheavyseedqty) = 0;
  (*pwcut) = ut_array_1d_max (SSet.SeedWeight + 1, SSet.N);

  rank = ut_num_min_int (SSet.N - 1, ut_num_max_int (1, SSet.N / 1000));
  if (rank < 1)
    return;

  // wtop: rank + 1 highest weights, by decreasing order (wq = wtop[rank])
  wtop = ut_alloc_1d (rank + 1);
  ut_array_1d_set (wtop, rank + 1, -DBL_MAX);
  for (i = 1; i <= SSet.N; i++)
    if (SSet.SeedWeight[i] > wtop[rank])
    {
      for (j = rank; j > 0 && SSet.SeedWeight[i] > wtop[j - 1]; j--)
        wtop[j] = wtop[j - 1];
      wtop[j] = SSet.SeedWeight[i];
    }
  wq = wtop[rank];
  ut_free_1d (&wtop);

  wthres = wq + ut_num_max (0, wq);

  if ((*pwcut) <= wthres)
    return;

  (*pwcut) = -DBL_MAX;
  for (i = 1; i <= SSet.N; i++)
    if (SSet.SeedWeight[i] <= wthres)
      (*pwcut) = ut_num_max (*pwcut, SSet.SeedWeight[i]);

  for (i = 1; i <= SSet.Nall; i++)
    if (SSet.SeedWeight[i] > wthres)
      ut_array_1d_int_list_addval_nocheck (pheavyseeds, pheavyseedqty, i);

  return;
}
