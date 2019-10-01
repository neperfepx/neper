/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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

void
neut_polymod_faces_inter (struct POLYMOD Polymod, int p1, int p2, int p3, double *inter)
{
  int i;
  double **A = ut_alloc_2d (3, 3);
  double *B = ut_alloc_1d (3);
  double det;

  for (i = 0; i < 3; i++)
  {
    A[0][i] = Polymod.FaceEq[p1][i + 1];
    A[1][i] = Polymod.FaceEq[p2][i + 1];
    A[2][i] = Polymod.FaceEq[p3][i + 1];
  }
  B[0] = Polymod.FaceEq[p1][0];
  B[1] = Polymod.FaceEq[p2][0];
  B[2] = Polymod.FaceEq[p3][0];

  det = ut_mat_det_33 (A);

  if (ut_num_equal (det, 0, 1e-15)) // the 2 first faces must be coplanar
  {
    if (!ut_space_planes_equal (Polymod.FaceEq[p1], Polymod.FaceEq[p2]))
      ut_error_reportbug ();

    int verqty, *vers = ut_alloc_1d_int (2);

    ut_array_1d_commonelts (Polymod.FaceVerNb[p1] + 1,
                            Polymod.FaceVerQty[p1],
                            Polymod.FaceVerNb[p2] + 1,
                            Polymod.FaceVerQty[p2],
                            &vers, &verqty);

    // seems that verqty != 2 should not happen...
    if (verqty == 2)
    {
      double *v = ut_alloc_1d (3);
      ut_array_1d_sub (Polymod.VerCoo[vers[0]], Polymod.VerCoo[vers[1]], 3, v);
      ut_array_1d_normalize (v, 3);

      ut_vector_vectprod (A[0], v, A[1]);

      B[1] = ut_array_1d_scalprod (A[1], Polymod.VerCoo[vers[0]], 3);

      if (ut_num_equal (ut_mat_det_33 (A), 0, 1e-12)) // Fix did not work
        ut_error_reportbug ();

      ut_free_1d (v);
      ut_free_1d_int (vers);
    }
  }

  ut_linalg_solve_LU (A, B, 3, inter);

  ut_free_2d (A, 3);
  ut_free_1d (B);

  return;
}
