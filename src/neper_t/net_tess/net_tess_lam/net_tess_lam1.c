/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
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

  ut_print_message (0, 2, "Running tessellation...\n");

  net_tess_lam_seed (In, level, In.morpho[level], *pMTess, Tess, dtess, dcell,
		     Dom, SSet, pSSet);

  net_tess3d (Tess[dtess], dcell, *pSSet, "nanoflann", TessId, pMTess, pTess);

  neut_tess_init_seeds_fromcell (pTess);

  ut_print_message (0, 2, "Generating crystal orientations...\n");
  net_ori (In, level, *pMTess, Tess, SSet, dtess, dcell, pSSet, 3);

  return 0;
}
