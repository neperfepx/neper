/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_write_.h"

void
nes_sim_write (struct IN_S In, struct SIM *pSim)
{
  if (strcmp (In.noderes, "none") || strcmp (In.eltres, "none"))
    ut_sys_mkdir (In.simdir);

  /*
  if (!strcmp (In.res, "all")
      || ut_list_testelt (In.res, ",", "avg"))
  {
    ut_print_message (0, 3, "Parsing post.force file(s)...\n");
    nes_sim_write_avg (In, Sim);
  }
  */

  ut_print_message (0, 2, "Writing report file...\n");
  // nes_sim_write_report (In, Sim);

  ut_print_message (0, 2, "Writing inputs...\n");
  nes_sim_write_inputs (In, *pSim);

  if ((*pSim).NodeResQty + (*pSim).EltResQty)
  {
    ut_print_message (0, 2, "Writing results...\n");
    nes_sim_write_results (In, pSim);
  }

  return;
}
