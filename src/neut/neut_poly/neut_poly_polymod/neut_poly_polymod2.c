/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_poly_polymod_.h"

void
neut_poly_polymodVerQty (struct POLY Poly, struct POLYMOD *pPolymod)
{
  (*pPolymod).VerQty = Poly.VerQty;

  return;
}

void
neut_poly_polymodFaceQty (struct POLY Poly, struct POLYMOD *pPolymod)
{
  (*pPolymod).FaceQty = Poly.FaceQty;

  return;
}

void
neut_poly_polymodVerProp (struct POLY Poly, struct POLYMOD *pPolymod)
{
  neut_poly_polymodVerFace (Poly, pPolymod);
  neut_poly_polymodVerCoo (Poly, pPolymod);
  neut_poly_polymodVerUse (pPolymod);

  return;
}

void
neut_poly_polymodFaceProp (struct POLY Poly, struct POLYMOD *pPolymod)
{
  neut_poly_polymodFacePoly (Poly, pPolymod);
  neut_poly_polymodFaceEq (Poly, pPolymod);
  neut_poly_polymodFaceVerQty (Poly, pPolymod);
  neut_poly_polymodFaceVerNb (Poly, pPolymod);
  neut_poly_polymodFaceUse (pPolymod);

  return;
}
