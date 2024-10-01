/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nes_input_.h"

void
nes_input (struct IN_S *pIn, int fargc, char **fargv, int argc, char **argv)
{
  ut_print_message (0, 2, "Reading arguments...\n");
  nes_input_treatargs (fargc, fargv, argc, argv, pIn);

  return;
}
