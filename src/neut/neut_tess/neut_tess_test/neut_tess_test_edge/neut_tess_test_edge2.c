/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_edge_.h"

int
neut_tess_test_edgeLength (struct TESS Tess, int i, int verbosity)
{
  double eps = 1e-15;

  if (Tess.EdgeLength[i] < eps)
  {
    if (verbosity)
      ut_print_message (0, 2, "length is zero.\n");

    return 2;
  }

  return 0;
}

int
neut_tess_test_edgeReciprocityVer (struct TESS Tess, int i, int verbosity)
{
  int j;
  int ver;

  for (j = 0; j <= 1; j++)
  {
    ver = Tess.EdgeVerNb[i][j];
    if (ut_array_1d_int_eltpos (Tess.VerEdgeNb[ver], Tess.VerEdgeQty[ver], i)
	== -1)
    {
      if (verbosity)
	ut_print_message (2, 3,
			  "based on ver %d, but edge not in ver edge list.\n",
			  ver);

      return 2;
    }
  }

  return 0;
}

int
neut_tess_test_edgeReciprocityFace (struct TESS Tess, int i, int verbosity)
{
  int j;
  int face;

  /* this test could be more accurate (considering edge ver bounds) */
  if (Tess.EdgeFaceQty[i] < Tess.Dim - 1)
  {
    if (verbosity)
      ut_print_message (2, 3, "number of faces = %d < 2.\n",
			Tess.EdgeFaceQty[i]);

    return 2;
  }

  for (j = 0; j < Tess.EdgeFaceQty[i]; j++)
  {
    face = Tess.EdgeFaceNb[i][j];

    if (ut_array_1d_int_eltpos
	(Tess.FaceEdgeNb[face] + 1, Tess.FaceVerQty[face], i) == -1)
    {
      if (verbosity)
	ut_print_message (2, 3,
			  "face %d is in face list, but face not based on edge.\n",
			  face);

      return 2;
    }
  }

  return 0;
}
