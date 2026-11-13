/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_ff_.h"

// net_reg_merge_del_ff_interpolate builds the interpolation of the face by
// searching the point used (barycenter or a vertex) and calculating the
// corresponding flatness fault.
double
net_reg_merge_del_ff_interpolate (struct TESS *pTess, int face)
{
  int i, domface;
  double minff;
  double *ff = ut_alloc_1d ((*pTess).FaceVerQty[face] + 1);

  // testing bary
  (*pTess).FacePt[face] = 0;
  neut_tess_face_centre (*pTess, face, (*pTess).FacePtCoo[face]);

  ff[0] = neut_tess_face_ff (*pTess, face);

  // testing vers
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    (*pTess).FacePt[face] = i;
    ff[i] = neut_tess_face_ff (*pTess, face);
    if ((*pTess).FaceDom[face][0] == 2)
    {
      domface = (*pTess).FaceDom[face][1];
      if (strcmp ((*pTess).DomFaceType[domface], "plane") && ff[i] < 90)
        ff[i] = 0;
    }
  }

  // the first point of minimal flatness fault is used, to a tolerance
  // (in degrees), so that points of the same flatness fault up to
  // round-off errors (which reach ~1e-6 for a flat face, because of acos)
  // are considered in a deterministic order
  minff = ut_array_1d_min (ff, (*pTess).FaceVerQty[face] + 1);
  for (i = 0; i <= (*pTess).FaceVerQty[face]; i++)
    if (ff[i] <= minff + 1e-5)
    {
      (*pTess).FacePt[face] = i;
      break;
    }

  if ((*pTess).FacePt[face] > 0)
    ut_array_1d_memcpy ((*pTess).
                        VerCoo[(*pTess).
                               FaceVerNb[face][(*pTess).FacePt[face]]], 3,
                        (*pTess).FacePtCoo[face]);
  minff = ff[(*pTess).FacePt[face]];

  ut_free_1d (&ff);

  return minff;
}
