/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_.h"

void
net_tess_opt_comp (struct TOPT *pTOpt)
{
  neut_tdyn_fprintf_pre (&((*pTOpt).TDyn));

  net_tess_opt_comp_pre (pTOpt);

  if ((*pTOpt).tarqty == 0)
    net_tess_opt_comp_once (pTOpt);
  else if (!strncmp ((*pTOpt).algoname, "random", 5))
    net_tess_opt_comp_rand (pTOpt);
  else if (!strncmp ((*pTOpt).algoname, "lloyd", 5))
    net_tess_opt_comp_lloyd (pTOpt);
  else
    net_tess_opt_comp_minimize (pTOpt);

  net_tess_opt_comp_post (*pTOpt);

  neut_tdyn_fprintf_post (&((*pTOpt).TDyn));

  return;
}
