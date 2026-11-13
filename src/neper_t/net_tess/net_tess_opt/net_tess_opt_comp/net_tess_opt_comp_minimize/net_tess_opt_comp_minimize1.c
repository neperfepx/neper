/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_.h"

// Minimizing the objective function from (*pTOpt).x, with the subplex
// algorithm (net_tess_opt_comp_minimize_subplex)
void
net_tess_opt_comp_minimize (struct TOPT *pTOpt)
{
  (*pTOpt).message[0] = '\0';
  ut_print_message (0, 3, "Running optimization...\n");
  ut_print_lineheader (0);
  printf ("      . ");

  (*pTOpt).iter = -1;
  net_tess_opt_comp_minimize_subplex (pTOpt);

  // evaluating the final solution again, as the last iteration (if the
  // objective function was evaluated several times)
  if ((*pTOpt).iter > 0)
  {
    (*pTOpt).iter--;
    net_tess_opt_comp_minimize_objective ((*pTOpt).x, pTOpt);
    net_tess_opt_comp_minimize_objective_stop (pTOpt);
    net_tess_opt_comp_minimize_objective_report (pTOpt);
  }

  net_tess_opt_comp_minimize_objective_stop_end ();

  printf ("\n");
  if ((*pTOpt).iter >= 0)
    ut_print_message (0, 4, "Final solution  :   f   =%.9f (%d iterations)",
                      (*pTOpt).objvalmin[(*pTOpt).iter], (*pTOpt).iter);

  return;
}
