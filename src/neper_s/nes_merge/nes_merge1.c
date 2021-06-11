/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_merge_.h"

void
nes_merge (struct IN_S In, struct SIM *pFullSim)
{
  int i, j, stepqty, simqty;
  char **sims = NULL;
  struct SIM Sim;

  neut_sim_set_zero (&Sim);

  ut_sys_mkdir (In.simdir);

  ut_list_break (In.mergedir, NEUT_SEP_NODEP, &sims, &simqty);

  ut_print_message (0, 2, "Merging inputs...\n");

  neut_sim_fscanf (sims[0], &Sim, "R");

  nes_merge_input (Sim, In.simdir);

  ut_print_message (0, 2, "Merging results...\n");

  stepqty = -1;
  for (i = 0; i < simqty; i++)
  {
    neut_sim_fscanf (sims[i], &Sim, "R");

    for (j = !(i == 0); j <= Sim.StepQty; j++)
    {
      neut_sim_setstep (&Sim, j);

      stepqty++;

      nes_merge_entity_res (Sim, "node", stepqty, In.simdir);
      nes_merge_entity_res (Sim, "element", stepqty, In.simdir);
      nes_merge_entity_res (Sim, "elset", stepqty, In.simdir);
    }
  }

  if (Sim.RestartFiles)
    nes_merge_restart (Sim, sims[simqty - 1], In.simdir);

  ut_print_message (0, 2, "Merging results...\n");

  nes_merge_post (In, sims[0], stepqty, Sim, pFullSim);

  ut_free_2d_char (&sims, simqty);
  neut_sim_free (&Sim);

  return;
}
