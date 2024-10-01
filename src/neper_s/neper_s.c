/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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

  // reading simulation and printing simulation content to terminal
  if (In.simdir)
  {
    neut_sim_fscanf (In.simdir, &Sim, "R");
    neut_sim_verbose (Sim);
  }

  // merging simulations and loading resulting simulation
  else if (In.mergedir)
  {
    ut_print_message (0, 1, "Merging simulation directories...\n");
    nes_merge (In, &Sim);
    neut_sim_verbose (Sim);
  }

// #####################################################################
// ### DEFINING NEW ENTITIES ###########################################

  // defining new entities
  if (In.entity)
  {
    ut_print_message (0, 1, "Adding entity...\n");
    nes_entity (In, &Sim);
  }

// #####################################################################
// ### RUNNING POST-PROCESSING #########################################

  // loading orientation space
  if (In.simdir && strcmp (In.orispace, "none"))
  {
    ut_print_message (0, 1, "Loading orientation space...\n");
    nes_orispace (In, &Sim);
  }

  // loading number of steps
  if (In.stepqty != -1)
  {
    Sim.StepQty = In.stepqty;
    Sim.StepState = ut_alloc_1d_int (Sim.StepQty + 1);
    ut_array_1d_int_set (Sim.StepState, Sim.StepQty + 1, 1);
    neut_sim_fprintf (In.simdir, Sim, "W");
  }

  // going into post-processing
  if (In.simdir && In.entityqty)
  {
    ut_print_message (0, 1, "Running post-processing...\n");
    nes_pproc (In, &Sim);
  }

// #####################################################################
// ### FREEING MEMORY ##################################################

  neut_sim_free (&Sim);
  nes_in_free (&In);

  return EXIT_SUCCESS;
}
