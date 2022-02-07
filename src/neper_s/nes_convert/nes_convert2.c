/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_convert_.h"

void
nes_convert_parse (struct IN_S In, struct FEPXSIM *pFSim, struct SIM *pSim)
{
  ut_print_message (0, 2, "Parsing FEPX results...\n");

  neut_fepxsim_fscanf (In.fepxdir, pFSim, "R");
  neut_fepxsim_sim (*pFSim, pSim);
  ut_string_string (In.simdir, &(*pSim).simdir);

  neut_sim_verbose (*pSim);

  return;
}
