/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_seedset_.h"
#include "neut_structs/neut_nfcloud_struct.hpp"

void
neut_seedset_memcpy_general (struct SEEDSET SSetA, struct SEEDSET *pSSetB)
{
  (*pSSetB).Dim = SSetA.Dim;
  (*pSSetB).N = SSetA.N;
  (*pSSetB).Id = SSetA.Id;
  (*pSSetB).Nall = SSetA.Nall;

  (*pSSetB).Size = ut_alloc_2d (3, 2);
  ut_array_2d_memcpy (SSetA.Size, 3, 2, (*pSSetB).Size);

  (*pSSetB).Periodic = ut_alloc_1d_int (3);
  if (SSetA.Periodic)
    ut_array_1d_int_memcpy (SSetA.Periodic, 3, (*pSSetB).Periodic);

  ut_string_string (SSetA.weight, &((*pSSetB).weight));

  ut_string_string (SSetA.Type, &((*pSSetB).Type));

  (*pSSetB).Random = SSetA.Random;
  (*pSSetB).RandomO = SSetA.RandomO;

  return;
}

void
neut_seedset_memcpy_seeds (struct SEEDSET SSetA, struct SEEDSET *pSSetB)
{
  int i;

  if (SSetA.LamEq)
  {
    (*pSSetB).LamEq = ut_alloc_2d ((*pSSetB).N + 1, 4);
    ut_array_2d_memcpy (SSetA.LamEq + 1, (*pSSetB).N, 4, (*pSSetB).LamEq + 1);
  }

  if (SSetA.LamWidth)
  {
    (*pSSetB).LamWidth = ut_alloc_1d ((*pSSetB).N + 1);
    ut_array_1d_memcpy (SSetA.LamWidth + 1, (*pSSetB).N,
                        (*pSSetB).LamWidth + 1);
  }

  if (SSetA.LamWidthId)
  {
    (*pSSetB).LamWidthId = ut_alloc_1d_int ((*pSSetB).N + 1);
    ut_array_1d_int_memcpy (SSetA.LamWidthId + 1, (*pSSetB).N,
                            (*pSSetB).LamWidthId + 1);
  }

  (*pSSetB).SeedCoo0 = ut_alloc_2d ((*pSSetB).N + 1, 3);
  ut_array_2d_memcpy (SSetA.SeedCoo0 + 1, (*pSSetB).N, 3,
                      (*pSSetB).SeedCoo0 + 1);

  (*pSSetB).SeedCoo = ut_alloc_2d ((*pSSetB).Nall + 1, 3);
  ut_array_2d_memcpy (SSetA.SeedCoo + 1, (*pSSetB).Nall, 3,
                      (*pSSetB).SeedCoo + 1);

  (*pSSetB).SeedWeight = ut_alloc_1d ((*pSSetB).Nall + 1);
  ut_array_1d_memcpy (SSetA.SeedWeight + 1, (*pSSetB).Nall,
                      (*pSSetB).SeedWeight + 1);

  if (SSetA.SeedOri)
  {
    (*pSSetB).SeedOri = ut_alloc_2d ((*pSSetB).N + 1, 4);
    ut_array_2d_memcpy (SSetA.SeedOri + 1, (*pSSetB).N, 4, (*pSSetB).SeedOri + 1);
  }

  if (SSetA.SeedOriR)
  {
    (*pSSetB).SeedOriR = ut_alloc_2d ((*pSSetB).N + 1, 3);
    ut_array_2d_memcpy (SSetA.SeedOriR + 1, (*pSSetB).N, 3, (*pSSetB).SeedOriR + 1);
  }

  if (SSetA.SeedOriDistrib)
  {
    (*pSSetB).SeedOriDistrib = ut_alloc_2d_char ((*pSSetB).N + 1, 1);
    for (i = 1; i <= (*pSSetB).N; i++)
      ut_string_string (SSetA.SeedOriDistrib[i], (*pSSetB).SeedOriDistrib + i);
  }

  if (SSetA.SeedOriTheta)
  {
    (*pSSetB).SeedOriTheta = ut_alloc_1d ((*pSSetB).N + 1);
    ut_array_1d_memcpy (SSetA.SeedOriTheta + 1, (*pSSetB).N, (*pSSetB).SeedOriTheta + 1);
  }

  if (SSetA.SeedOriWeight)
  {
    (*pSSetB).SeedOriWeight = ut_alloc_1d ((*pSSetB).N + 1);
    ut_array_1d_memcpy (SSetA.SeedOriWeight + 1, (*pSSetB).N, (*pSSetB).SeedOriWeight + 1);
  }

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
    ut_array_1d_int_memcpy (SSetA.Periodic, 3, (*pSSetB).Periodic);
  }

  if (SSetA.PeriodicDist)
  {
    (*pSSetB).PeriodicDist = ut_alloc_1d (3);
    ut_array_1d_memcpy (SSetA.PeriodicDist, 3, (*pSSetB).PeriodicDist);
  }

  if (SSetA.PerSeedMaster)
  {
    (*pSSetB).PerSeedMaster = ut_alloc_1d_int ((*pSSetB).Nall + 1);
    ut_array_1d_int_memcpy (SSetA.PerSeedMaster + 1, (*pSSetB).Nall,
                            (*pSSetB).PerSeedMaster + 1);
  }

  if (SSetA.PerSeedShift)
  {
    (*pSSetB).PerSeedShift = ut_alloc_2d_int ((*pSSetB).Nall + 1, 3);
    ut_array_2d_int_memcpy (SSetA.PerSeedShift + 1, (*pSSetB).Nall, 3,
                            (*pSSetB).PerSeedShift + 1);
  }

  if (SSetA.PerSeedSlaveQty)
  {
    (*pSSetB).PerSeedSlaveQty = ut_alloc_1d_int ((*pSSetB).N + 1);
    ut_array_1d_int_memcpy (SSetA.PerSeedSlaveQty + 1, (*pSSetB).N,
                            (*pSSetB).PerSeedSlaveQty + 1);
  }

  if (SSetA.PerSeedSlaveNb)
  {
    (*pSSetB).PerSeedSlaveNb = ut_alloc_1d_pint ((*pSSetB).N + 1);
    for (i = 1; i <= (*pSSetB).N; i++)
    {
      (*pSSetB).PerSeedSlaveNb[i] =
        ut_alloc_1d_int ((*pSSetB).PerSeedSlaveQty[i] + 1);
      ut_array_1d_int_memcpy (SSetA.PerSeedSlaveNb[i] + 1,
                              (*pSSetB).PerSeedSlaveQty[i],
                              (*pSSetB).PerSeedSlaveNb[i] + 1);
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
    ut_array_1d_memcpy (SSet.SeedCoo[i + 1], 3, (*pnf_cloud).pts[i].p);
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
                                 NFCLOUD * pnf_cloud, NFTREE ** pnf_tree,
                                 int **pptid_seedid, int **pseedid_ptid)
{
  int newpt, oldpt;

  oldpt = (*pseedid_ptid)[seed];
  (*pnf_tree)->removePoint (oldpt);

  (*pnf_cloud).pts.resize ((*pnf_cloud).pts.size () + 1);
  // to my understanding, this should not be necessary, but it turns out that
  // with the current implementation, a removed point can be returned as
  // neighbors by findNeighbors
  ut_array_1d_set_3 ((*pnf_cloud).pts[oldpt].p, DBL_MAX, DBL_MAX, DBL_MAX);

  newpt = (*pnf_cloud).pts.size () - 1;
  ut_array_1d_memcpy (SSet.SeedCoo[seed], 3, (*pnf_cloud).pts[newpt].p);
  (*pnf_tree)->addPoints (newpt, newpt);

  (*pptid_seedid) =
    ut_realloc_1d_int (*pptid_seedid, (*pnf_cloud).pts.size ());
  (*pptid_seedid)[newpt] = seed;
  (*pseedid_ptid)[seed] = newpt;

  return;
}
