/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_.h"
#include"neut/neut_structs/neut_nanoflann_struct.hpp"
extern void neut_seedset_kdtree (struct SEEDSET SSet, NFCLOUD * pnf_cloud,
                                 NFTREE ** pnf_tree, int **, int **);
extern void neut_seedset_kdtree_update (struct SEEDSET SSet, int *seedchanged,
                                        int seedchangedqty,
                                        NFCLOUD * pnf_cloud,
                                        NFTREE ** pnf_tree, int **, int **);

extern void net_polycomp_cells_updatecell (struct POLY Domain,
                                           struct SEEDSET SSet,
                                           NFTREE ** pnf_tree,
                                           int *ptid_seedid, int cell,
                                           struct POLY **pPoly,
                                           struct TDYN *pTD);

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
    (*pTD).cell_shift_dur = 0;
    (*pTD).cell_neigh_dur = 0;
    (*pTD).cell_cell_dur = 0;
    (*pTD).cell_other_dur = 0;
    (*pTD).cell_total_dur = 0;
  }

  return;
}

void
net_polycomp_kdtree (struct SEEDSET SSet, NFCLOUD * pnf_cloud,
                     NFTREE ** pnf_tree, int **pptid_seedid,
                     int **pseedid_ptid, struct TDYN *pTD)
{
  struct timeval time;

  gettimeofday (&time, NULL);

  if ((*pTD).iter == 1)
    neut_seedset_kdtree (SSet, pnf_cloud, pnf_tree, pptid_seedid,
                         pseedid_ptid);
  else if ((*pTD).seedmovedqty > 0)
    neut_seedset_kdtree_update (SSet, (*pTD).seedmoved, (*pTD).seedmovedqty,
                                pnf_cloud, pnf_tree, pptid_seedid,
                                pseedid_ptid);

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
    (*pTD).shift[i] = ut_space_dist ((*pTD).neighrefcoo[i], SSet.SeedCoo[i]);
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
        ut_array_1d_int_list_addval_nocheck (pupdatedseeds, pupdatedseedqty,
                                             i);
  }
  else
  {
    (*pupdatedseedqty) = updatedseedqty_in;
    (*pupdatedseeds) = ut_alloc_1d_int (*pupdatedseedqty);
    ut_array_1d_int_memcpy (updatedseeds_in, *pupdatedseedqty,
                            *pupdatedseeds);
  }

  return;
}

void
net_polycomp_cells (struct POLY Domain, struct SEEDSET SSet,
                    NFTREE ** pnf_tree, int *ptid_seedid, int *updatedseeds,
                    int updatedseedqty, struct TDYN *pTD, struct POLY **pPoly)
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
#pragma omp parallel for schedule(dynamic)
    for (i = 1; i <= SSet.N; i++)
      net_polycomp_cells_updatecell (Domain, SSet, pnf_tree, ptid_seedid, i,
                                     pPoly, pTD);

  // Recording old neighs of updatedseeds
  neut_polys_neighpolys (*pPoly, SSet, updatedseeds, updatedseedqty,
                         &oldneighs, &oldneighqty);

  // Updating cells of updatedseeds
#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < updatedseedqty; i++)
    net_polycomp_cells_updatecell (Domain, SSet, pnf_tree, ptid_seedid,
                                   updatedseeds[i], pPoly, pTD);

  // Recording new neighs of updatedseeds
  neut_polys_neighpolys (*pPoly, SSet, updatedseeds, updatedseedqty,
                         &newneighs, &newneighqty);

  // Updating old first-neighbors of updatedseeds
#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < oldneighqty; i++)
    net_polycomp_cells_updatecell (Domain, SSet, pnf_tree, ptid_seedid,
                                   oldneighs[i], pPoly, pTD);

  // Updating new first-neighbors of updatedseeds
#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < newneighqty; i++)
    net_polycomp_cells_updatecell (Domain, SSet, pnf_tree, ptid_seedid,
                                   newneighs[i], pPoly, pTD);

  // Updating second-and-more-neighbors of updateseeds (changedneighs)
  // We start from the second-neighbors, but third-and-more-neighbors
  // may be added to changedneighs along the way.
#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < (*pTD).changedneighqty; i++)
    net_polycomp_cells_updatecell (Domain, SSet, pnf_tree, ptid_seedid,
                                   (*pTD).changedneighs[i], pPoly, pTD);

  // If a cell is the full domain, updating all cells
  if ((*pTD).domcellqty == 1)
#pragma omp parallel for schedule(dynamic)
    for (i = 1; i <= SSet.N; i++)
      net_polycomp_cells_updatecell (Domain, SSet, pnf_tree, ptid_seedid, i,
                                     pPoly, pTD);

  // Free'ing memory
  ut_free_1d_int (&oldneighs);
  ut_free_1d_int (&newneighs);

  return;
}
