/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_tocta_seed_.h"

int
net_tess_tocta_seed_set_finalize (struct SEEDSET *pSSet)
{
  (*pSSet).Nall = (*pSSet).N;
  (*pSSet).SeedWeight = ut_alloc_1d ((*pSSet).N + 1);
  /*
     (*pSSet).SeedCoo = ut_alloc_2d ((*pSSet).N + 1, 3);
     (*pSSet).SeedCoo0 = ut_alloc_2d ((*pSSet).N + 1, 3);
     (*pSSet).Size = ut_alloc_2d (3, 2);
   */

  return 0;
}
