/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_edge_.h"

int
neut_tess_test_edge (struct TESS Tess, int i, int verbosity)
{
  if (verbosity)
    ut_print_message (0, 2, "Checking edge %d...\n", i);

  if (neut_tess_test_edgeLength (Tess, i, verbosity) != 0
      || neut_tess_test_edgeReciprocityVer (Tess, i, verbosity) != 0
      || neut_tess_test_edgeReciprocityFace (Tess, i, verbosity) != 0)
    return -1;
  else
    return 0;
}
