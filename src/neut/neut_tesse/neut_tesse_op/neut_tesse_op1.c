/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesse_op_.h"

void
neut_tesse_set_zero (struct TESSE *pTessE)
{
  (*pTessE).VerQty = 0;
  (*pTessE).EdgeQty = 0;
  (*pTessE).FaceQty = 0;
  (*pTessE).PolyQty = 0;

  (*pTessE).VerFVer = NULL;
  (*pTessE).EdgeFEdgeQty = NULL;
  (*pTessE).EdgeFEdgeNb = NULL;
  (*pTessE).EdgeFEdgeOri = NULL;
  (*pTessE).FaceFFaceQty = NULL;
  (*pTessE).FaceFFaceNb = NULL;
  (*pTessE).FaceFFaceOri = NULL;

  (*pTessE).PolyFPolyQty = NULL;
  (*pTessE).PolyFPolyNb = NULL;

  return;
}

void
neut_tesse_face_addface (struct TESSE *pTessE, int face, int fface, int fori)
{
  (*pTessE).FaceFFaceQty[face]++;
  (*pTessE).FaceFFaceNb[face] =
    ut_realloc_1d_int ((*pTessE).FaceFFaceNb[face],
                       (*pTessE).FaceFFaceQty[face]);
  (*pTessE).FaceFFaceOri[face] =
    ut_realloc_1d_int ((*pTessE).FaceFFaceOri[face],
                       (*pTessE).FaceFFaceQty[face]);
  (*pTessE).FaceFFaceNb[face][(*pTessE).FaceFFaceQty[face] - 1] = fface;
  (*pTessE).FaceFFaceOri[face][(*pTessE).FaceFFaceQty[face] - 1] = fori;

  return;
}

void
neut_tesse_poly_addpoly (struct TESSE *pTessE, int poly, int fpoly)
{
  (*pTessE).PolyFPolyQty[poly]++;
  (*pTessE).PolyFPolyNb[poly] =
    ut_realloc_1d_int ((*pTessE).PolyFPolyNb[poly],
                       (*pTessE).PolyFPolyQty[poly]);
  (*pTessE).PolyFPolyNb[poly][(*pTessE).PolyFPolyQty[poly] - 1] = fpoly;

  return;
}

void
neut_tesse_free (struct TESSE *pTessE)
{
  ut_free_1d_int (&(*pTessE).VerFVer);
  (*pTessE).VerQty = 0;

  ut_free_1d_int (&(*pTessE).EdgeFEdgeQty);
  ut_free_2d_int (&(*pTessE).EdgeFEdgeNb, (*pTessE).EdgeQty + 1);
  ut_free_2d_int (&(*pTessE).EdgeFEdgeOri, (*pTessE).EdgeQty + 1);
  (*pTessE).EdgeQty = 0;

  ut_free_1d_int (&(*pTessE).FaceFFaceQty);
  ut_free_2d_int (&(*pTessE).FaceFFaceNb, (*pTessE).FaceQty + 1);
  ut_free_2d_int (&(*pTessE).FaceFFaceOri, (*pTessE).FaceQty + 1);
  (*pTessE).FaceQty = 0;

  ut_free_1d_int (&(*pTessE).PolyFPolyQty);
  ut_free_2d_int (&(*pTessE).PolyFPolyNb, (*pTessE).PolyQty + 1);
  (*pTessE).PolyQty = 0;

  return;
}
