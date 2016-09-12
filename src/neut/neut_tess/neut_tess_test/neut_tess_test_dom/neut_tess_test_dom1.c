/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_dom_.h"

int
neut_tess_test_dom (struct TESS Tess, int verbosity)
{
  // disabling test for periodic tessellations
  if (!strncmp (Tess.Type, "periodic", 8))
    return 0;

  if (neut_tess_test_dom_def (Tess, verbosity) != 0
      || neut_tess_test_dom_tessver (Tess, verbosity) != 0
      || neut_tess_test_dom_tessedge (Tess, verbosity) != 0
      || neut_tess_test_dom_tessface (Tess, verbosity) != 0)
    return -1;
  else
    return 0;
}
