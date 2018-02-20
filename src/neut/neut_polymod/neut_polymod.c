/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_polymod_.h"

void
neut_polymod_set_zero (struct POLYMOD *pPolymod)
{
  (*pPolymod).VerQty = 0;
  (*pPolymod).FaceQty = 0;

  (*pPolymod).VerFace = NULL;
  (*pPolymod).VerCoo = NULL;
  (*pPolymod).VerUse = NULL;

  (*pPolymod).FacePoly = NULL;
  (*pPolymod).FaceEq = NULL;
  (*pPolymod).FaceVerQty = NULL;
  (*pPolymod).FaceVerNb = NULL;
  (*pPolymod).FaceUse = NULL;

  return;
}

void
neut_polymod_free (struct POLYMOD *pPolymod)
{
  ut_free_2d_int ((*pPolymod).VerFace, (*pPolymod).VerQty + 1);
  ut_free_2d ((*pPolymod).VerCoo, (*pPolymod).VerQty + 1);
  ut_free_1d_int ((*pPolymod).VerUse);

  ut_free_1d_int ((*pPolymod).FacePoly);
  ut_free_2d ((*pPolymod).FaceEq, (*pPolymod).FaceQty + 1);
  ut_free_1d_int ((*pPolymod).FaceVerQty);
  ut_free_2d_int ((*pPolymod).FaceVerNb, (*pPolymod).FaceQty + 1);
  ut_free_1d_int ((*pPolymod).FaceUse);

  neut_polymod_set_zero (pPolymod);

  return;
}
