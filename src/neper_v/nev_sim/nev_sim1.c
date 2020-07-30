/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_sim_.h"

void
nev_sim (char **argv, int *pi, struct SIM *pSim)
{
  if (!strcmp (argv[*pi], "-simstep"))
    sscanf (argv[++(*pi)], "%d", &(*pSim).step);
  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}
