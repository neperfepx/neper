/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_x_.h"

void
net_tess_opt_comp_x (struct TOPT *pTOpt, double **px)
{
  if (!strcmp ((*pTOpt).optitype, "seeds"))
    net_tess_opt_comp_x_seeds (pTOpt, px);
  else if (!strcmp ((*pTOpt).optitype, "crystal"))
    net_tess_opt_comp_x_crystal (pTOpt, px);
  else if (!strcmp ((*pTOpt).optitype, "domain"))
    net_tess_opt_comp_x_domain (pTOpt, px);
  else
    abort ();

  return;
}
