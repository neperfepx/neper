/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_.h"
#include<ANN/ANN.h>

void net_polycomp_cells_updatecell (struct POLY Domain, struct SEEDSET SSet,
				    ANNkd_tree *pkdTree, int cell,
				    struct POLY **pPoly, struct TDYN *pTD);

void
net_polycomp_inittdyn (struct POLY Domain, struct SEEDSET SSet,
                       struct TDYN *pTD, struct POLY **pPoly)
{
  if ((*pTD).iter == 1)
  {
    neut_tdyn_alloc (pTD, SSet.N, SSet.Nall);

    (*pPoly) = (POLY *) calloc (SSet.N + 1, sizeof (POLY));

    if ((*pTD).avdiameq == 0)
      neut_tdyn_init_avdiameq (pTD, SSet, Domain);
  }

  else
  {
    (*pTD).cell_kdtree_dur = 0;
    (*pTD).cell_shift_dur  = 0;
    (*pTD).cell_neigh_dur  = 0;
    (*pTD).cell_cell_dur   = 0;
    (*pTD).cell_other_dur  = 0;
    (*pTD).cell_total_dur  = 0;
  }

  return;
}

void
net_polycomp_kdtree (struct SEEDSET SSet, ANNkd_tree ** pkdTree,
		     struct TDYN *pTD)
{
  struct timeval time;

  gettimeofday (&time, NULL);

  if ((*pTD).iter > 1 && *pkdTree)
    delete *pkdTree;
  (*pkdTree) = new ANNkd_tree (SSet.SeedCoo + 1, SSet.Nall, SSet.Dim);

  (*pTD).cell_kdtree_dur = ut_time_subtract (&time, NULL);

  return;
}

void
net_polycomp_shift (struct SEEDSET SSet, struct TDYN *pTD)
{
  int i;
  struct timeval time;

  if ((*pTD).iter == 1)
    return;

  gettimeofday (&time, NULL);

  // computing shifts and maxshift, on condition that the refcoo are defined
  // (i.e. this is not the first increment)
  for (i = 1; i <= (*pTD).N; i++)
    (*pTD).shift[i] =
      ut_space_dist ((*pTD).neighrefcoo[i], SSet.SeedCoo[i]);
  (*pTD).shiftmax = ut_array_1d_max ((*pTD).shift + 1, (*pTD).N);
  (*pTD).shiftmean = ut_array_1d_mean ((*pTD).shift + 1, (*pTD).N);

  for (i = 1; i <= (*pTD).N; i++)
    (*pTD).shiftw[i] = fabs ((*pTD).neighrefw[i] - SSet.SeedWeight[i]);
  (*pTD).shiftwmax = ut_array_1d_max ((*pTD).shiftw + 1, (*pTD).N);
  (*pTD).shiftwmean = ut_array_1d_mean ((*pTD).shiftw + 1, (*pTD).N);

  (*pTD).cell_shift_dur = ut_time_subtract (&time, NULL);

  return;
}

void
net_polycomp_updatedseeds (struct SEEDSET SSet, struct TDYN *pTD,
			   int *updatedseeds_in, int updatedseedqty_in,
			   int **pupdatedseeds, int *pupdatedseedqty)
{
  int i;

  if (updatedseedqty_in == -1)
  {
    (*pupdatedseedqty) = 0;
    for (i = 1; i <= SSet.N; i++)
      if ((*pTD).iter == 1 || (*pTD).shift[i] > 1e-20
	  || (*pTD).shiftw[i] > 1e-20)
	ut_array_1d_int_list_addelt_nocheck (pupdatedseeds, pupdatedseedqty, i);
  }
  else
  {
    (*pupdatedseedqty) = updatedseedqty_in;
    (*pupdatedseeds) = ut_alloc_1d_int (*pupdatedseedqty);
    ut_array_1d_int_memcpy (*pupdatedseeds, *pupdatedseedqty,
			    updatedseeds_in);
  }

  return;
}

void
net_polycomp_cells (struct POLY Domain, struct SEEDSET SSet,
                    ANNkd_tree *pkdTree, int *updatedseeds, int updatedseedqty,
		    struct TDYN *pTD, struct POLY **pPoly)
{
  int i;
  int oldneighqty, *oldneighs = NULL;
  int newneighqty, *newneighs = NULL;

  // Initializing some members of TDyn
  net_polycomp_cells_inittdyn (pTD);

  // If a cell was the full domain, updating all cells The domain-cell
  // configuration could be handled more efficiently (by skipping the
  // below steps or setting other cells to void instead of computing
  // them, but the current implementation is robust and kept simple.
  if ((*pTD).domcellqty == 1)
    for (i = 1; i <= SSet.N; i++)
      net_polycomp_cells_updatecell (Domain, SSet, pkdTree, i, pPoly, pTD);

  // Recording old neighs of updatedseeds
  neut_polys_neighpolys (*pPoly, SSet, updatedseeds, updatedseedqty,
			 &oldneighs, &oldneighqty);

  // Updating cells of updatedseeds
  for (i = 0; i < updatedseedqty; i++)
    net_polycomp_cells_updatecell (Domain, SSet, pkdTree,
				   updatedseeds[i], pPoly, pTD);

  // Recording new neighs of updatedseeds
  neut_polys_neighpolys (*pPoly, SSet, updatedseeds, updatedseedqty,
			 &newneighs, &newneighqty);

  // Updating old first-neighbours of updatedseeds
  for (i = 0; i < oldneighqty; i++)
    net_polycomp_cells_updatecell (Domain, SSet, pkdTree, oldneighs[i],
				   pPoly, pTD);

  // Updating new first-neighbours of updatedseeds
  for (i = 0; i < newneighqty; i++)
    net_polycomp_cells_updatecell (Domain, SSet, pkdTree, newneighs[i],
				   pPoly, pTD);

  // Updating second-and-more-neighbours of updateseeds (changedneighs)
  // We start from the second-neighbours, but third-and-more-neighbours
  // may be added to changedneighs along the way.
  for (i = 0; i < (*pTD).changedneighqty; i++)
    net_polycomp_cells_updatecell (Domain, SSet, pkdTree,
				   (*pTD).changedneighs[i], pPoly, pTD);

  // If a cell is the full domain, updating all cells
  if ((*pTD).domcellqty == 1)
    for (i = 1; i <= SSet.N; i++)
      net_polycomp_cells_updatecell (Domain, SSet, pkdTree, i, pPoly, pTD);

  // Free'ing memory
  ut_free_1d_int (oldneighs);
  ut_free_1d_int (newneighs);

  return;
}
