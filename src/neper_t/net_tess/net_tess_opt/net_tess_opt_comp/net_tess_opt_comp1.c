/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_.h"

void
net_tess_opt_comp (struct TOPT *pTOpt)
{
  double *x = NULL;
  int res = -1;

  neut_tdyn_fprintf_pre (&((*pTOpt).TDyn));

  net_tess_opt_comp_x (pTOpt, &x);

  if ((*pTOpt).tarqty > 0)
    ut_print_message (0, 3, " ");

  if ((*pTOpt).tarqty == 0)
    res = net_tess_opt_comp_once (x, pTOpt);
  else if (!strncmp ((*pTOpt).algoname[0], "random", 5))
    res = net_tess_opt_comp_rand (x, pTOpt);
  else if (!strncmp ((*pTOpt).algoname[0], "lloyd", 5))
    res = net_tess_opt_comp_lloyd (x, pTOpt);
  else
#ifdef HAVE_NLOPT
    res = net_tess_opt_comp_nlopt (x, pTOpt);
#else
    ut_print_message (2, 0,
                      "net_tess_opt_comp_nlopt not available since Neper was compiled without NLopt.\n");
#endif

#ifdef HAVE_NLOPT
  net_tess_opt_comp_post (*pTOpt, res);
#else
  (void) res;
#endif

  neut_tdyn_fprintf_post (&((*pTOpt).TDyn));

  ut_free_1d (&x);

  return;
}
