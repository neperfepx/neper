/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_fepxparse_.h"

void
nes_sim_fepxparse (struct IN_S In, struct SIM *pSim)
{
  ut_print_message (0, 2, "Parsing FEPX results...\n");

  neut_sim_set_zero (pSim);

  neut_sim_fscanf (In.fepxdir, pSim, "R");

  ut_string_string (In.simdir, &(*pSim).simdir);

  return;
}
