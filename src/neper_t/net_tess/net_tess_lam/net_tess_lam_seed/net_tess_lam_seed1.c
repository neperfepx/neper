/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_lam_seed_.h"

void
net_tess_lam_seed (struct IN_T In, int level, char *morpho, struct MTESS MTess, struct TESS *Tess,
	      int dtess, int dcell, struct TESS Dom,
	      struct SEEDSET *SSet, struct SEEDSET *pSSet)

{
  int wqty, vqty;
  double *w = NULL, *v = NULL;
  char *wtype = NULL, *vtype = NULL;

  ut_string_string ("standard", &(*pSSet).Type);

  net_tess_lam_seed_readargs (morpho, MTess, Tess, dtess, dcell,
			      &wtype, &w, &wqty, &vtype, &v, &vqty);

  net_tess_lam_seed_set (In, level, MTess, Tess, dtess, dcell, Dom, wtype, w, wqty,
      vtype, v, vqty, SSet, pSSet);

  ut_free_1d (v);
  ut_free_1d (w);
  ut_free_1d_char (vtype);
  ut_free_1d_char (wtype);

  return;
}
