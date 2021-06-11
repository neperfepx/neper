/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_write_.h"

void
nes_sim_write (struct IN_S In, struct SIM *pSim)
{
  if (strcmp (In.noderes, "none") || strcmp (In.eltres, "none"))
    ut_sys_mkdir (In.simdir);

  ut_print_message (0, 2, "Writing inputs...\n");
  nes_sim_write_inputs (In, pSim);

  if ((*pSim).NodeResQty || (*pSim).EltResQty)
  {
    ut_print_message (0, 2, "Writing results...\n");
    nes_sim_write_results (In, pSim);
  }

  if ((*pSim).RestartFiles)
  {
    ut_print_message (0, 2, "Writing restart...\n");
    nes_sim_write_restart ((*pSim).fepxdir, (*pSim).simdir, *pSim);
  }

  return;
}
