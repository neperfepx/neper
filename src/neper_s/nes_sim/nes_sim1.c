/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_.h"

int
nes_sim (struct IN_S In, struct SIM *pSim)
{
  ut_dir_openmessage (In.simdir, "w");

  nes_sim_parse (In, pSim);

  nes_sim_write (In, pSim);

  ut_dir_closemessage (In.simdir, "w");

  return 0;
}
