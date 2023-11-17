/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_tocta_seed_.h"

void
net_tess_tocta_seed (struct IN_T In, char *morpho,
                     struct MTESS MTess, struct TESS *Tess, int dtess,
                     int dcell, struct TESS Dom, struct SEEDSET *SSet,
                     struct SEEDSET *pSSet)
{
  int n;

  ut_string_string ("standard", &(*pSSet).Type);

  net_tess_tocta_seed_readargs (morpho, &n);

  net_tess_tocta_seed_set (In, MTess, Tess, dtess, dcell, Dom, n, SSet, pSSet);

  return;
}
