/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_png_tess_.h"

// Smallest radius of curvature of the curved domain faces of a tessellation
// (cylinder, elliptic cylinder, sphere and torus, possibly inward: cylinderi,
// etc.), or -1 if there is none
double
nev_print_real_png_tess_curvedmesh_rmin (struct TESS Tess)
{
  int i;
  double r, a, b, rmin = DBL_MAX;

  for (i = 1; i <= Tess.DomFaceQty; i++)
  {
    if (!strncmp (Tess.DomFaceType[i], "cylinder", 8))
      r = Tess.DomFaceParms[i][6];

    else if (!strncmp (Tess.DomFaceType[i], "ecylinder", 9))
    {
      // smallest radius of curvature of an ellipse: b^2 / a, for a >= b
      a = ut_num_max (Tess.DomFaceParms[i][12], Tess.DomFaceParms[i][13]);
      b = ut_num_min (Tess.DomFaceParms[i][12], Tess.DomFaceParms[i][13]);
      r = b * b / a;
    }

    else if (!strncmp (Tess.DomFaceType[i], "sphere", 6))
      r = Tess.DomFaceParms[i][3];

    // torus of major radius R and minor radius r: radii of curvature r (along
    // the meridian) and down to R - r (around the axis, at the inner equator)
    else if (!strncmp (Tess.DomFaceType[i], "torus", 5))
      r = ut_num_min (Tess.DomFaceParms[i][7],
                      fabs (Tess.DomFaceParms[i][6] - Tess.DomFaceParms[i][7]));

    else
      continue;

    rmin = ut_num_min (rmin, fabs (r));
  }

  return rmin < DBL_MAX ? rmin : -1;
}
