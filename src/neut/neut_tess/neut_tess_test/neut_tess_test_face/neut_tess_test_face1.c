/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_face_.h"

int
neut_tess_test_face (struct TESS Tess, int i, int verbosity)
{
  if (verbosity)
    ut_print_message (0, 2, "Checking face %d...\n", i);

  if (Tess.FaceVerQty[i] == 0)
    return 0;

  if (neut_tess_test_faceReciprocityEdge (Tess, i, verbosity) != 0
      || neut_tess_test_faceReciprocityVer (Tess, i, verbosity) != 0
      || neut_tess_test_faceReciprocityPoly (Tess, i, verbosity) != 0
      || neut_tess_test_faceState (Tess, i, verbosity) != 0
      || neut_tess_test_faceBound (Tess, i, verbosity) != 0
      // || neut_tess_test_vervsedge (Tess, i, verbosity) != 0
      || neut_tess_test_faceSelfIntersect (Tess, i, verbosity) != 0
      || neut_tess_test_face_normal (Tess, i, verbosity) != 0)
    return -1;
  else
    return 0;
}
