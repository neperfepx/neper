/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_poly_.h"

int
neut_tess_test_poly (struct TESS Tess, int i, int verbosity)
{
  int res;

  if (verbosity)
    ut_print_message (0, 2, "Checking poly %d...\n", i);

  if (Tess.PolyFaceQty[i] == 0)
    return 0;

  res = neut_tess_test_polyReciprocityFace (Tess, i, verbosity);
  if (res != 0)
    return res;

  /*
     res = neut_tess_test_polyCoplaneityFace (Tess, i, verbosity);
     if (res != 0)
     return res;
   */

  res = neut_tess_test_polyOrientationFace (Tess, i, verbosity);
  if (res != 0)
    return res;

  res = neut_tess_test_poly_pinching (Tess, i, verbosity);
  if (res != 0)
    return res;

  return 0;
}
