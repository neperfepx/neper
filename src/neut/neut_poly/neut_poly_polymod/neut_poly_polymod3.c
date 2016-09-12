/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_poly_polymod_.h"

void
neut_poly_polymodVerFace (struct POLY Poly, struct POLYMOD *pPolymod)
{
  int i, j;

  (*pPolymod).VerFace = ut_alloc_2d_int ((*pPolymod).VerQty + 1, 3);

  for (i = 1; i <= (*pPolymod).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pPolymod).VerFace[i][j] = Poly.VerFace[i][j];

  return;
}

void
neut_poly_polymodVerCoo (struct POLY Poly, struct POLYMOD *pPolymod)
{
  int i, j;

  (*pPolymod).VerCoo = ut_alloc_2d ((*pPolymod).VerQty + 1, 3);

  for (i = 1; i <= (*pPolymod).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pPolymod).VerCoo[i][j] = Poly.VerCoo[i][j];

  return;
}

void
neut_poly_polymodVerUse (struct POLYMOD *pPolymod)
{
  int i;

  (*pPolymod).VerUse = ut_alloc_1d_int ((*pPolymod).VerQty + 1);

  for (i = 1; i <= (*pPolymod).VerQty; i++)
    (*pPolymod).VerUse[i] = 1;

  return;
}

void
neut_poly_polymodFacePoly (struct POLY Poly, struct POLYMOD *pPolymod)
{
  int i;

  (*pPolymod).FacePoly = ut_alloc_1d_int ((*pPolymod).FaceQty + 1);

  for (i = 1; i <= (*pPolymod).FaceQty; i++)
    (*pPolymod).FacePoly[i] = Poly.FacePoly[i];

  return;
}

void
neut_poly_polymodFaceEq (struct POLY Poly, struct POLYMOD *pPolymod)
{
  int i, j;

  (*pPolymod).FaceEq = ut_alloc_2d ((*pPolymod).FaceQty + 1, 4);

  for (i = 1; i <= (*pPolymod).FaceQty; i++)
    for (j = 0; j <= 3; j++)
      (*pPolymod).FaceEq[i][j] = Poly.FaceEq[i][j];

  return;
}

void
neut_poly_polymodFaceVerQty (struct POLY Poly, struct POLYMOD *pPolymod)
{
  int i;

  (*pPolymod).FaceVerQty = ut_alloc_1d_int ((*pPolymod).FaceQty + 1);

  for (i = 1; i <= (*pPolymod).FaceQty; i++)
    (*pPolymod).FaceVerQty[i] = Poly.FaceVerQty[i];

  return;
}

void
neut_poly_polymodFaceVerNb (struct POLY Poly, struct POLYMOD *pPolymod)
{
  int i, j;

  (*pPolymod).FaceVerNb = ut_alloc_1d_pint ((*pPolymod).FaceQty + 1);

  for (i = 1; i <= (*pPolymod).FaceQty; i++)
  {
    (*pPolymod).FaceVerNb[i] =
      ut_alloc_1d_int ((*pPolymod).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pPolymod).FaceVerQty[i]; j++)
      (*pPolymod).FaceVerNb[i][j] = Poly.FaceVerNb[i][j];
  }

  return;
}

void
neut_poly_polymodFaceUse (struct POLYMOD *pPolymod)
{
  int i;

  (*pPolymod).FaceUse = ut_alloc_1d_int ((*pPolymod).FaceQty + 1);

  for (i = 1; i <= (*pPolymod).FaceQty; i++)
    (*pPolymod).FaceUse[i] = 1;

  return;
}
