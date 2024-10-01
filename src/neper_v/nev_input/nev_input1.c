/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_input_.h"

int
nev_input (int argc, char **argv, struct IN_V *pIn, int *ppos)
{
  if (*ppos == 1)
    ut_print_message (0, 2, "Reading arguments...\n");

  nev_input_treatargs (argc, argv, pIn, ppos);

  return (*ppos == -1) ? -1 : 0;
}
