/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_lam_seed_.h"

void
net_tess_lam_seed (struct IN_T In, int level, char *morpho,
                   struct MTESS MTess, struct TESS *Tess, int dtess,
                   int dcell, struct TESS Dom, struct SEEDSET *SSet,
                   struct SEEDSET *pSSet)
{
  int wqty;
  int *n = NULL;
  double reps, *w = NULL, *v = NULL;
  char *pos = NULL;
  char *ntype = NULL, *wtype = NULL, *vtype = NULL, *postype = NULL;

  ut_string_string ("standard", &(*pSSet).Type);

  net_tess_lam_seed_readargs (In, level, morpho, MTess, Tess, dtess, dcell, &ntype, &n,
                              &wtype, &w, &wqty, &vtype, &v, &postype, &pos, &reps);

  net_tess_lam_seed_set (In, level, MTess, Tess, dtess, dcell, Dom, ntype, n, &wtype, &w,
                         &wqty, vtype, v, postype, pos, reps, SSet, pSSet);

  ut_free_1d_int (&n);
  ut_free_1d (&v);
  ut_free_1d (&w);
  ut_free_1d_char (&pos);
  ut_free_1d_char (&ntype);
  ut_free_1d_char (&vtype);
  ut_free_1d_char (&wtype);
  ut_free_1d_char (&postype);

  return;
}
