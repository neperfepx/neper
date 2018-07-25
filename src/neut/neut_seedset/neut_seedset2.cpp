/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_seedset_.h"
#include "neut_structs/neut_nanoflann_struct.hpp"

void
neut_seedset_memcpy_general (struct SEEDSET SSetA, struct SEEDSET *pSSetB)
{
  (*pSSetB).Dim = SSetA.Dim;
  (*pSSetB).N = SSetA.N;
  (*pSSetB).Id = SSetA.Id;
  (*pSSetB).Nall = SSetA.Nall;

  (*pSSetB).Size = ut_alloc_2d (3, 2);
  ut_array_2d_memcpy ((*pSSetB).Size, 3, 2, SSetA.Size);

  (*pSSetB).Periodic = ut_alloc_1d_int (3);
  if (SSetA.Periodic)
    ut_array_1d_int_memcpy ((*pSSetB).Periodic, 3, SSetA.Periodic);

  ut_string_string (SSetA.weight, &((*pSSetB).weight));

  ut_string_string (SSetA.Type, &((*pSSetB).Type));

  (*pSSetB).Random = SSetA.Random;
  (*pSSetB).RandomO = SSetA.RandomO;

  return;
}

void
neut_seedset_memcpy_seeds (struct SEEDSET SSetA, struct SEEDSET *pSSetB)
{
  if (SSetA.LamEq)
  {
    (*pSSetB).LamEq = ut_alloc_2d ((*pSSetB).N + 1, 4);
    ut_array_2d_memcpy ((*pSSetB).LamEq + 1, (*pSSetB).N, 4, SSetA.LamEq + 1);
  }

  if (SSetA.LamWidth)
  {
    (*pSSetB).LamWidth = ut_alloc_1d ((*pSSetB).N + 1);
    ut_array_1d_memcpy ((*pSSetB).LamWidth + 1, (*pSSetB).N,
                        SSetA.LamWidth + 1);
  }

  if (SSetA.LamWidthId)
  {
    (*pSSetB).LamWidthId = ut_alloc_1d_int ((*pSSetB).N + 1);
    ut_array_1d_int_memcpy ((*pSSetB).LamWidthId + 1, (*pSSetB).N,
                            SSetA.LamWidthId + 1);
  }

  (*pSSetB).SeedCoo0 = ut_alloc_2d ((*pSSetB).N + 1, 3);
  ut_array_2d_memcpy ((*pSSetB).SeedCoo0 + 1, (*pSSetB).N, 3,
                      SSetA.SeedCoo0 + 1);

  (*pSSetB).SeedCoo = ut_alloc_2d ((*pSSetB).Nall + 1, 3);
  ut_array_2d_memcpy ((*pSSetB).SeedCoo + 1, (*pSSetB).Nall, 3,
                      SSetA.SeedCoo + 1);

  (*pSSetB).SeedWeight = ut_alloc_1d ((*pSSetB).Nall + 1);
  ut_array_1d_memcpy ((*pSSetB).SeedWeight + 1, (*pSSetB).Nall,
                      SSetA.SeedWeight + 1);

  (*pSSetB).q = ut_alloc_2d ((*pSSetB).N + 1, 4);
  ut_array_2d_memcpy ((*pSSetB).q + 1, (*pSSetB).N, 4, SSetA.q + 1);

  ut_string_string (SSetA.crysym, &(*pSSetB).crysym);

  return;
}

void
neut_seedset_memcpy_periodic (struct SEEDSET SSetA, struct SEEDSET *pSSetB)
{
  int i;

  if (SSetA.Periodic)
  {
    (*pSSetB).Periodic = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy ((*pSSetB).Periodic, 3, SSetA.Periodic);
  }

  if (SSetA.PeriodicDist)
  {
    (*pSSetB).PeriodicDist = ut_alloc_1d (3);
    ut_array_1d_memcpy ((*pSSetB).PeriodicDist, 3, SSetA.PeriodicDist);
  }

  if (SSetA.PerSeedMaster)
  {
    (*pSSetB).PerSeedMaster = ut_alloc_1d_int ((*pSSetB).Nall + 1);
    ut_array_1d_int_memcpy ((*pSSetB).PerSeedMaster + 1, (*pSSetB).Nall,
                            SSetA.PerSeedMaster + 1);
  }

  if (SSetA.PerSeedShift)
  {
    (*pSSetB).PerSeedShift = ut_alloc_2d_int ((*pSSetB).Nall + 1, 3);
    ut_array_2d_int_memcpy ((*pSSetB).PerSeedShift + 1, (*pSSetB).Nall, 3,
                            SSetA.PerSeedShift + 1);
  }

  if (SSetA.PerSeedSlaveQty)
  {
    (*pSSetB).PerSeedSlaveQty = ut_alloc_1d_int ((*pSSetB).N + 1);
    ut_array_1d_int_memcpy ((*pSSetB).PerSeedSlaveQty + 1, (*pSSetB).N,
                            SSetA.PerSeedSlaveQty + 1);
  }

  if (SSetA.PerSeedSlaveNb)
  {
    (*pSSetB).PerSeedSlaveNb = ut_alloc_1d_pint ((*pSSetB).N + 1);
    for (i = 1; i <= (*pSSetB).N; i++)
    {
      (*pSSetB).PerSeedSlaveNb[i]
        = ut_alloc_1d_int ((*pSSetB).PerSeedSlaveQty[i] + 1);
      ut_array_1d_int_memcpy ((*pSSetB).PerSeedSlaveNb[i] + 1,
                              (*pSSetB).PerSeedSlaveQty[i],
                              SSetA.PerSeedSlaveNb[i] + 1);
    }
  }

  return;
}

void
neut_seedset_kdtree_cloud (struct SEEDSET SSet, NFCLOUD * pnf_cloud,
                           int **pptid_seedid, int **pseedid_ptid)
{
  int i;

  (*pnf_cloud).pts.resize (SSet.Nall);
  (*pptid_seedid) = ut_alloc_1d_int (SSet.Nall);
  (*pseedid_ptid) = ut_alloc_1d_int (SSet.Nall + 1);

  for (i = 0; i < SSet.Nall; i++)
  {
    ut_array_1d_memcpy ((*pnf_cloud).pts[i].p, 3, SSet.SeedCoo[i + 1]);
    (*pptid_seedid)[i] = i + 1;
    (*pseedid_ptid)[i + 1] = i;
  }

  return;
}

void
neut_seedset_kdtree_tree (NFCLOUD * pnf_cloud, NFTREE ** pnf_tree)
{
  if (*pnf_tree)
    delete *pnf_tree;
  (*pnf_tree) = new NFTREE (3, *pnf_cloud);

  return;
}

void
neut_seedset_kdtree_update_seed (struct SEEDSET SSet, int seed,
                                 NFCLOUD * pnf_cloud,
                                 NFTREE ** pnf_tree,
                                 int **pptid_seedid, int **pseedid_ptid)
{
  int newpt, oldpt;

  oldpt = (*pseedid_ptid)[seed];
  (*pnf_tree)->removePoint (oldpt);

  (*pnf_cloud).pts.resize ((*pnf_cloud).pts.size () + 1);
  // to my understanding, this should not be necessary, but it turns out that
  // with the current implementation, a removed point can be returned as
  // neighbours by findNeighbors
  ut_array_1d_set_3 ((*pnf_cloud).pts[oldpt].p, DBL_MAX, DBL_MAX, DBL_MAX);

  newpt = (*pnf_cloud).pts.size () - 1;
  ut_array_1d_memcpy ((*pnf_cloud).pts[newpt].p, 3, SSet.SeedCoo[seed]);
  (*pnf_tree)->addPoints (newpt, newpt);

  (*pptid_seedid) =
    ut_realloc_1d_int (*pptid_seedid, (*pnf_cloud).pts.size ());
  (*pptid_seedid)[newpt] = seed;
  (*pseedid_ptid)[seed] = newpt;

  return;
}
