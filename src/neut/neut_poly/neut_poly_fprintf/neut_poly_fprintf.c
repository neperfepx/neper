/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_poly_fprintf_.h"

void
neut_poly_fprintf_gmsh (FILE * file, struct POLY Poly, int beg)
{
  int i, j;

  beg--;

  for (i = 1; i <= Poly.VerQty; i++)
    fprintf (file, "Point (%d) = {%.12f,%.12f,%.12f};\n",
	     beg + i, Poly.VerCoo[i][0], Poly.VerCoo[i][1],
	     Poly.VerCoo[i][2]);

  int lineid = 1;
  for (i = 1; i <= Poly.FaceQty; i++)
  {
    int line1 = lineid;
    for (j = 1; j <= Poly.FaceVerQty[i]; j++)
    {
      fprintf (file, "Line (%d) = {%d,%d};\n",
	       beg + lineid++,
	       beg + Poly.FaceVerNb[i][j],
	       beg +
	       Poly.FaceVerNb[i][ut_num_rotpos
				 (1, Poly.FaceVerQty[i], j, 1)]);
    }

    fprintf (file, "Line Loop (%d) = {", i + beg);
    for (j = 1; j <= Poly.FaceVerQty[i]; j++)
    {
      fprintf (file, "%d%s", beg + line1 + j - 1,
	       (j < Poly.FaceVerQty[i]) ? "," : "");
    }
    fprintf (file, "};\n");
    fprintf (file, "Plane Surface (%d) = {%d}; ", beg + i, beg + i);
    fprintf (file, "Physical Surface (%d) = {%d};\n", beg + i, beg + i);
  }

  fprintf (file, "Surface Loop (%d) = {", beg + 1);
  for (j = 1; j <= Poly.FaceQty; j++)
    fprintf (file, "%d%s", j + beg, (j < Poly.FaceQty) ? "," : "");
  fprintf (file, "};\n");
  fprintf (file, "Volume (%d) = {%d}; ", beg + 1, beg + 1);

  return;
}
