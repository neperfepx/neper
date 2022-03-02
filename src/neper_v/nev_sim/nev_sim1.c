/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_sim_.h"

void
nev_sim (char **argv, int *pi, struct SIM *pSim)
{
  int status, tmp;

  if (!strcmp (argv[*pi], "-simstep")
   || !strcmp (argv[*pi], "-step"))
  {
    ut_print_message (0, 0, "Setting simulation step...\n");

    if (!strcmp (argv[*pi], "-simstep"))
      ut_print_message (1, 1, "Option `-simstep' will be removed in future versions.  Use `-step` instead.\n");

    sscanf (argv[++(*pi)], "%d", &tmp);
    status = neut_sim_setstep (pSim, tmp);
    if (status)
      ut_print_message (2, 2, "Step %d not available.\n", tmp);
  }
  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}
