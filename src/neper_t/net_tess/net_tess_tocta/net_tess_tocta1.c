/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_tocta_.h"

int
net_tess_tocta (struct IN_T In, char *morpho, struct MTESS *pMTess,
                struct TESS *Tess, int dtess, int dcell, int TessId,
                struct SEEDSET *SSet)
{
  struct TESS *pTess = Tess + TessId;
  struct SEEDSET *pSSet = SSet + TessId;
  struct TESS Dom;

  neut_tess_set_zero (&Dom);

  neut_tess_poly_tess (Tess[dtess], dcell, &Dom);

  net_tess_tocta_seed (In, morpho, *pMTess, Tess, dtess, dcell, Dom,
                       SSet, pSSet);

  ut_print_message (0, 2, "Running tessellation...\n");
  net_tess3d (Tess[dtess], dcell, *pSSet, TessId, pMTess, pTess);

  net_tess_tocta_cellid (*pSSet, pTess);

  neut_tess_free (&Dom);

  return 0;
}
