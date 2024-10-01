/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_poly_op_.h"

void
neut_poly_set_zero (struct POLY *pPoly)
{
  (*pPoly).PseudoDim = -1;
  (*pPoly).PseudoSize = -DBL_MAX;

  (*pPoly).VerQty = 0;
  (*pPoly).FaceQty = 0;

  (*pPoly).VerFace = NULL;
  (*pPoly).VerCoo = NULL;

  (*pPoly).FacePoly = NULL;
  (*pPoly).FaceEq = NULL;
  (*pPoly).FaceVerQty = NULL;
  (*pPoly).FaceVerNb = NULL;

  return;
}

void
neut_poly_free (struct POLY *pPoly)
{
  ut_free_2d_int (&(*pPoly).VerFace, (*pPoly).VerQty + 1);
  ut_free_2d (&(*pPoly).VerCoo, (*pPoly).VerQty + 1);

  ut_free_1d_int (&(*pPoly).FacePoly);
  ut_free_2d (&(*pPoly).FaceEq, (*pPoly).FaceQty + 1);
  ut_free_1d_int (&(*pPoly).FaceVerQty);
  ut_free_2d_int (&(*pPoly).FaceVerNb, (*pPoly).FaceQty + 1);

  neut_poly_set_zero (pPoly);

  return;
}

void
neut_poly_array_free (struct POLY **pPoly, int size)
{
  int i;

  if ((*pPoly) == NULL)
    return;

  for (i = 1; i <= size; i++)
    neut_poly_free (&((*pPoly)[i]));

  free (*pPoly);
  (*pPoly) = NULL;

  return;
}
