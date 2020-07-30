/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_s_.h"

int
neper_s (int fargc, char **fargv, int argc, char **argv)
{
  // ###################################################################
  // ### INITIALIZING ##################################################

  // Variable declaration ###
  struct IN_S In;
  struct SIM Sim;

  nes_in_set_zero (&In);
  neut_sim_set_zero (&Sim);

  // Printing program header ###
  ut_print_moduleheader ("-S", fargc, fargv, argc, argv);

  // Reading input data ###
  ut_print_message (0, 1, "Reading input data...\n");
  nes_input (&In, fargc, fargv, argc, argv);

// #####################################################################
// ### READING RESULTS #################################################

  if (In.fepxdir)
  {
    ut_print_message (0, 1, "Writing simulation directory from FEPX raw result directory...\n");
    nes_sim (In, &Sim);
  }

// #####################################################################
// ### RUNNING POST-PROCESSING #########################################

  if (In.simdir && strcmp (In.eltres, "inputres"))
  {
    ut_print_message (0, 1, "Running post-processing...\n");
    nes_pproc (In, &Sim);
  }

// #####################################################################
// ### FREEING MEMORY ##################################################

  neut_sim_free (&Sim);
  nes_in_free (In);

  return EXIT_SUCCESS;
}
