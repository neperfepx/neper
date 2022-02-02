/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_convert_.h"

void
nes_convert (struct IN_S In, struct SIM *pSim)
{
  ut_dir_openmessage (In.simdir, "w");

  nes_convert_fepxparse (In, pSim);

  neut_sim_verbose (*pSim);

  nes_convert_write (In, pSim);

  ut_dir_closemessage (In.simdir, "w");

  return;
}
