/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_pre_.h"

void
net_tess_opt_comp_pre (struct TOPT *pTOpt)
{
  ut_free_1d (&(*pTOpt).x);

  if (!strcmp ((*pTOpt).optitype, "morpho"))
    net_tess_opt_comp_pre_morpho (pTOpt);
  else if (!strcmp ((*pTOpt).optitype, "ori"))
    net_tess_opt_comp_pre_ori (pTOpt);
  else
    abort ();

  return;
}
