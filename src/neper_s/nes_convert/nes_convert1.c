/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_convert_.h"

void
nes_convert (struct IN_S In, struct SIM *pSim)
{
  struct FEPXSIM FSim;

  neut_fepxsim_set_zero (&FSim);

  nes_convert_parse (In, &FSim, pSim);

  nes_convert_write (In, &FSim);

  neut_sim_fprintf (In.simdir, *pSim, "W");

  neut_fepxsim_free (&FSim);

  return;
}
