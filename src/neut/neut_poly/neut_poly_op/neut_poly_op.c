/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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
neut_poly_poly (struct POLY PolyA, struct POLY *pPolyB)
{
  int i;

  neut_poly_free (pPolyB);

  (*pPolyB).PseudoDim = PolyA.PseudoDim;
  (*pPolyB).PseudoSize = PolyA.PseudoSize;

  (*pPolyB).VerQty = PolyA.VerQty;
  (*pPolyB).FaceQty = PolyA.FaceQty;

  (*pPolyB).VerFace = ut_alloc_2d_int (PolyA.VerQty + 1, 3);
  (*pPolyB).VerCoo = ut_alloc_2d (PolyA.VerQty + 1, 3);
  ut_array_2d_int_memcpy (PolyA.VerFace + 1, PolyA.VerQty, 3, (*pPolyB).VerFace + 1);
  ut_array_2d_memcpy (PolyA.VerCoo + 1, PolyA.VerQty, 3, (*pPolyB).VerCoo + 1);

  (*pPolyB).FacePoly = ut_alloc_1d_int (PolyA.FaceQty + 1);
  (*pPolyB).FaceEq = ut_alloc_2d (PolyA.FaceQty + 1, 4);
  (*pPolyB).FaceVerQty = ut_alloc_1d_int (PolyA.FaceQty + 1);
  (*pPolyB).FaceVerNb = ut_alloc_1d_pint (PolyA.FaceQty + 1);
  ut_array_1d_int_memcpy (PolyA.FacePoly + 1, PolyA.FaceQty, (*pPolyB).FacePoly + 1);
  ut_array_2d_memcpy (PolyA.FaceEq + 1, PolyA.FaceQty, 4, (*pPolyB).FaceEq + 1);
  ut_array_1d_int_memcpy (PolyA.FaceVerQty + 1, PolyA.FaceQty, (*pPolyB).FaceVerQty + 1);
  for (i = 1; i <= PolyA.FaceQty; i++)
  {
    (*pPolyB).FaceVerNb[i] = ut_alloc_1d_int (PolyA.FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (PolyA.FaceVerNb[i] + 1, PolyA.FaceVerQty[i],
                            (*pPolyB).FaceVerNb[i] + 1);
  }

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

void
neut_poly_fixverorder (struct POLY *pPoly)
{
  int i, face;
  double *bary = ut_alloc_1d (3);
  double *n = ut_alloc_1d (3);
  double *n0 = ut_alloc_1d (3);

  for (face = 1; face <= (*pPoly).FaceQty; face++)
  {
    // skip non-planar faces, for which FaceEq = -1
    if (ut_vector_norm ((*pPoly).FaceEq[face] + 1) - 1 > 1e-6)
      continue;

    int cooqty = (*pPoly).FaceVerQty[face];
    double **coo = ut_alloc_2d (cooqty, 3);
    ut_array_1d_zero (bary, 3);
    for (i = 0; i < (*pPoly).FaceVerQty[face]; i++)
    {
      ut_array_1d_memcpy ((*pPoly).VerCoo[(*pPoly).FaceVerNb[face][i + 1]], 3, coo[i]);
      ut_array_1d_add (bary, coo[i], 3, bary);
    }
    ut_array_1d_scale (bary, 3, 1. / cooqty);

    ut_array_1d_zero (n, 3);
    for (i = 0; i < (*pPoly).FaceVerQty[face]; i++)
    {
      ut_space_triangle_normal (bary, coo[i],
                                coo[ut_array_rotpos
                                    (0, (*pPoly).FaceVerQty[face] - 1, i, 1)], n0);
      ut_array_1d_add (n, n0, 3, n);
    }
    ut_array_1d_scale (n, 3, 1. / ut_array_1d_norm (n, 3));

    if (ut_vector_scalprod (n, (*pPoly).FaceEq[face] + 1) < 0)
      ut_array_1d_int_reverseelts ((*pPoly).FaceVerNb[face] + 1, (*pPoly).FaceVerQty[face]);

    ut_free_2d (&coo, cooqty);

    /*
    // make sure smallest vertex appears first (for unicity)
    int index = ut_array_1d_int_min_index ((*pPoly).FaceVerNb[face] + 1, (*pPoly).FaceVerQty[face]);
    ut_array_1d_int_rotate ((*pPoly).FaceVerNb[face] + 1, (*pPoly).FaceVerQty[face], index);
    */
  }

  ut_free_1d (&bary);
  ut_free_1d (&n);
  ut_free_1d (&n0);

  return;
}
