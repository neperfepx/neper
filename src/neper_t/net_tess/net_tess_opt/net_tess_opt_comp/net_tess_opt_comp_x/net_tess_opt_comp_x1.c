/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_x_.h"

void
net_tess_opt_comp_x (struct TOPT *pTOpt, double **px)
{
  if (!strcmp ((*pTOpt).optitype, "morpho"))
    net_tess_opt_comp_x_morpho (pTOpt, px);
  else if (!strcmp ((*pTOpt).optitype, "ori"))
    net_tess_opt_comp_x_ori (pTOpt, px);
  else if (!strcmp ((*pTOpt).optitype, "crystal"))
    net_tess_opt_comp_x_crystal (pTOpt, px);
  else
    abort ();

  return;
}
