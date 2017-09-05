/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_ver_.h"

int
neut_tess_test_ver (struct TESS Tess, int i, int verbosity)
{
  if (verbosity)
    ut_print_message (0, 2, "Checking vertex %d...\n", i);

  if (neut_tess_test_verEdgeQtyNNb (Tess, i, verbosity) != 0
      || neut_tess_test_verEdgeReciprocity (Tess, i, verbosity) != 0
      || neut_tess_test_verBoundNCoo (Tess, i, verbosity) != 0
      || (strcmp (Tess.DomType, "cylinder") != 0 &&
	  neut_tess_test_verFaceCoplaneity (Tess, i, verbosity) != 0))
    return -1;
  else
    return 0;
}
