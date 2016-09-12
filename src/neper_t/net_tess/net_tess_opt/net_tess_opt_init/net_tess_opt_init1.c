/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init (struct IN_T In, int level, struct MTESS MTess,
		   struct TESS *Tess, int dtess, int dcell,
		   struct SEEDSET *SSet, struct TOPT *pTOpt)
{
  // general
  net_tess_opt_init_general (pTOpt);

  // domain
  net_tess_opt_init_domain (In, Tess[dtess], dcell, pTOpt);

  // target properties
  net_tess_opt_init_target (In, MTess, Tess[dtess], dcell, level,
			    In.morpho[level], pTOpt);

  // sset (must come after target)
  net_tess_opt_init_sset (In, level, MTess, Tess, dtess, dcell, SSet, pTOpt);

  // optimization parameters
  net_tess_opt_init_parms (In, level, Tess[dtess], pTOpt);

  // current properties (allocation)
  net_tess_opt_init_current (pTOpt);

  // bounds (merge into init_parms)
  net_tess_opt_init_bounds (pTOpt);

  return;
}
