/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_input_.h"

void
nev_input_treatargs (int argc, char **argv, struct IN_V *pIn, int *ppos)
{
  if (*ppos <= 1)
    nev_input_options_default (pIn);

  nev_input_options_set (argc, argv, pIn, ppos);

  if (!strcmp ((*pIn).space, "ori") || !strcmp ((*pIn).space, "rodrigues"))
    ut_string_string ("rodrigues(fr20)", &((*pIn).space));

  return;
}
