/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_.h"

void
nes_sim (struct IN_S In, struct SIM *pSim)
{
  ut_dir_openmessage (In.simdir, "w");

  nes_sim_fepxparse (In, pSim);

  neut_sim_verbose (*pSim);

  nes_sim_modify (In, pSim);

  nes_sim_write (In, pSim);

  ut_dir_closemessage (In.simdir, "w");

  return;
}
