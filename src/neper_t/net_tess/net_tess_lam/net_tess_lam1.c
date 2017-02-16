/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_lam_.h"

int
net_tess_lam (struct IN_T In, int level,
              struct MTESS *pMTess, struct TESS *Tess,
	      int dtess, int dcell,
	      struct TESS Dom,
	      int TessId,
	      struct SEEDSET *SSet)
{
  struct TESS *pTess = Tess + TessId;
  struct SEEDSET *pSSet = SSet + TessId;

  net_tess_lam_seed (In, level, In.morpho[level], *pMTess, Tess, dtess, dcell,
		     Dom, SSet, pSSet);

  net_tess3d (Tess[dtess], dcell, *pSSet, "ann", TessId, pMTess, pTess);

  neut_tess_init_seeds_fromcell (pTess);

  return 0;
}
