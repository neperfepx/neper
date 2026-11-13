/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_.h"

// set on Ctrl+C, by the signal handler
static int ctrlc = 0;

static void
net_tess_opt_comp_minimize_objective_stop_ctrlc_handler (int sig)
{
  if (sig != SIGINT)
    abort ();

  ctrlc = 1;

  return;
}

// Returns 1 if Ctrl+C was pressed (since the previous call), 0 otherwise.
// A first Ctrl+C stops the optimization, and a second one stops Neper.
int
net_tess_opt_comp_minimize_objective_stop_ctrlc (void)
{
  signal (SIGINT, net_tess_opt_comp_minimize_objective_stop_ctrlc_handler);

  if (ctrlc)
  {
    signal (SIGINT, SIG_DFL);
    ctrlc = 0;
    return 1;
  }

  return 0;
}

// Restores the default Ctrl+C behavior, at the end of the optimization
// (otherwise, a Ctrl+C during the subsequent steps would be ignored).
void
net_tess_opt_comp_minimize_objective_stop_ctrlc_end (void)
{
  signal (SIGINT, SIG_DFL);
  ctrlc = 0;

  return;
}
