/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_parse_.h"

void
nes_sim_parse (struct IN_S In, struct SIM *pSim)
{
  ut_print_message (0, 2, "Parsing FEPX results...\n");

  nes_sim_parse_init (In, pSim);

  neut_sim_fscanf (In.fepxdir, pSim);

  nes_sim_parse_fix (In, pSim);

  nes_sim_parse_print (*pSim);

  return;
}
