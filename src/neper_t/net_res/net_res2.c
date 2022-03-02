/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_res_.h"

void
net_res_sim (struct IN_T In, struct TESS Tess, struct TESR Tesr)
{
  FILE *file = NULL;
  struct SIM Sim;
  char *simmsh = ut_alloc_1d_char (1000);
  char *simtess = ut_alloc_1d_char (1000);
  char *simtesr = ut_alloc_1d_char (1000);

  neut_sim_set_zero (&Sim);

  ut_dir_openmessage (In.sim, "w");

  if (!neut_tess_isvoid (Tess))
  {
    neut_tess_sim (In.sim, Tess, &Sim);
    sprintf (simtess, "%s/inputs/simulation.tess", In.sim);

    file = ut_file_open (simtess, "W");
    neut_tess_fprintf (file, Tess);
    ut_file_close (file, simtess, "W");
  }

  if (!neut_tesr_isvoid (Tesr))
  {
    neut_tesr_sim (In.sim, Tesr, &Sim);
    sprintf (simtesr, "%s/inputs/simulation.tesr", In.sim);

    file = ut_file_open (simtesr, "W");
    neut_tesr_fprintf (file, NULL, Tesr);
    ut_file_close (file, simtesr, "W");
  }

  neut_sim_fprintf (In.sim, Sim, "W");

  ut_dir_closemessage (In.sim, "w");

  neut_sim_free (&Sim);

  ut_free_1d_char (&simmsh);
  ut_free_1d_char (&simtess);
  ut_free_1d_char (&simtesr);

  return;
}
