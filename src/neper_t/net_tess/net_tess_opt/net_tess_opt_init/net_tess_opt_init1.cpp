/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init (struct IN_T In, int level, char *morpho,
                   struct MTESS MTess, struct TESS *Tess, int dtess,
                   int dcell, struct SEEDSET *SSet, struct TOPT *pTOpt)
{
  // general, dim, domain0
  net_tess_opt_init_general (In, level, MTess, Tess, dtess, dcell, pTOpt);

  // target properties
  net_tess_opt_init_target (In, MTess, Tess, dtess, dcell, level, morpho,
                            pTOpt);

  // domain to tessellate
  net_tess_opt_init_domain (In, Tess[dtess], dcell, pTOpt);

  // sset (must come after target)
  if (!strcmp ((*pTOpt).optitype, "seeds"))
    net_tess_opt_init_sset (In, level, MTess, Tess, dtess, dcell, SSet,
                            pTOpt);
  else if (!strcmp ((*pTOpt).optitype, "crystal"))
    net_tess_opt_init_crystal (In, level, pTOpt);
  else if (!strcmp ((*pTOpt).optitype, "domain"))
  {
  }
  else
    abort ();

  // optimization parameters
  net_tess_opt_init_parms (In, level, MTess, Tess, dtess, dcell, pTOpt);

  // current properties (allocation)
  net_tess_opt_init_current (pTOpt);

  // bounds (merge into init_parms)
  net_tess_opt_init_bounds (pTOpt);

  // finalizing
  net_tess_opt_init_post (In, pTOpt);

  return;
}
