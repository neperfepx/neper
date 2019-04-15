/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_post_.h"

void
net_tess_opt_post (struct MTESS *pMTess, struct TESS *Tess,
		   int dtess, int dcell, int tessid,
		   struct POLY *Poly, struct TOPT TOpt,
		   struct SEEDSET *SSet)
{
  if (!strcmp (TOpt.optitype, "seeds"))
    net_tess_opt_post_tess (pMTess, Tess, dtess, dcell, tessid, Poly, TOpt, SSet);

  return;
}
