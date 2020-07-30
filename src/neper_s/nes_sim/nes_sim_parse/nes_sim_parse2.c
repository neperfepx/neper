/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_parse_.h"

void
nes_sim_parse_init (struct IN_S In, struct SIM *pSim)
{
  neut_sim_set_zero (pSim);

  ut_string_string (In.fepxdir, &(*pSim).fepxdir);
  ut_string_string (In.simdir, &(*pSim).simdir);
  ut_string_string ("simulation", &(*pSim).body);
  ut_string_string ("simulation.msh", &(*pSim).msh);
  ut_string_string ("simulation.tess", &(*pSim).tess);

  return;
}

void
nes_sim_parse_fix (struct IN_S In, struct SIM *pSim)
{
  if (!ut_list_testelt (In.noderes, NEUT_SEP_NODEP, "inputres"))
    nes_sim_parse_fix_entity (In, pSim, "node");

  if (!ut_list_testelt (In.eltres, NEUT_SEP_NODEP, "inputres"))
    nes_sim_parse_fix_entity (In, pSim, "elt");

  return;
}

void
nes_sim_parse_print (struct SIM Sim)
{
  ut_print_message (0, 3, "Partition number: %d.\n", Sim.PartQty);
  ut_print_message (0, 3, "Step      number: %d.\n", Sim.StepQty);
  ut_print_message (0, 3, "Node      number: %d.\n", Sim.NodeQty);
  ut_print_message (0, 3, "Element   number: %d.\n", Sim.EltQty);

  return;
}
