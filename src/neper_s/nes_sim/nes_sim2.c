/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_.h"

void
nes_sim_modify (struct IN_S In, struct SIM *pSim)
{
  if (!ut_list_testelt (In.noderes, NEUT_SEP_NODEP, "inputres"))
    nes_sim_modify_entity (In, pSim, "node");

  if (!ut_list_testelt (In.eltres, NEUT_SEP_NODEP, "inputres"))
    nes_sim_modify_entity (In, pSim, "elt");

  return;
}
