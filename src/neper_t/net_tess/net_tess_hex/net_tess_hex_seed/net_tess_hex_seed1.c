/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_hex_seed_.h"

void
net_tess_hex_seed (struct IN_T In, char *morpho,
                   struct MTESS MTess, struct TESS *Tess, int dtess,
                   int dcell, struct TESS Dom, struct SEEDSET *SSet,
                   struct SEEDSET *pSSet)
{
  int *n = ut_alloc_1d_int (2);
  char *hex = NULL;

  ut_string_string ("standard", &(*pSSet).Type);

  net_tess_hex_seed_readargs (morpho, &hex, n);

  net_tess_hex_seed_set (In, MTess, Tess, dtess, dcell, Dom, hex, n, SSet, pSSet);

  ut_free_1d_char (&hex);
  ut_free_1d_int (&n);

  return;
}
