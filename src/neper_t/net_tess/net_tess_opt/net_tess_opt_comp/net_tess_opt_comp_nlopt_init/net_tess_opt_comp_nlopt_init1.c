/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_nlopt_init_.h"

#ifdef HAVE_NLOPT
void
net_tess_opt_comp_nlopt_init (struct TOPT *pTOpt)
{
  int qty;

  ut_string_separate ((*pTOpt).dof, NEUT_SEP_NODEP, NULL, &qty);

  (*pTOpt).opt = nlopt_create ((*pTOpt).algo, qty * (*pTOpt).seedoptiqty);

  net_tess_opt_comp_nlopt_init_objective (pTOpt);
  net_tess_opt_comp_nlopt_init_bounds (pTOpt);
  net_tess_opt_comp_nlopt_init_stop (pTOpt);

  return;
}

#endif
