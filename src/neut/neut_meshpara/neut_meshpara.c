/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_meshpara_.h"

void
neut_meshpara_set_zero (struct MESHPARA *pMeshPara)
{
  (*pMeshPara).VerQty = 0;
  (*pMeshPara).EdgeQty = 0;
  (*pMeshPara).FaceQty = 0;
  (*pMeshPara).PolyQty = 0;

  (*pMeshPara).elttype = NULL;

  (*pMeshPara).cltype = NULL;
  (*pMeshPara).elttype = NULL;
  (*pMeshPara).clratio = NULL;

  (*pMeshPara).poly_cl = NULL;
  (*pMeshPara).face_cl = NULL;
  (*pMeshPara).edge_cl = NULL;
  (*pMeshPara).ver_cl = NULL;

  (*pMeshPara).mesh3dclreps = NULL;

  (*pMeshPara).face_op = NULL;
  (*pMeshPara).edge_op = NULL;
  (*pMeshPara).face_eq = NULL;

  return;
}

void
neut_meshpara_free (struct MESHPARA *pMeshPara)
{
  ut_free_1d_char (&(*pMeshPara).elttype);

  ut_free_1d_char (&(*pMeshPara).cltype);
  ut_free_1d (&(*pMeshPara).clratio);

  ut_free_1d (&(*pMeshPara).poly_cl);
  ut_free_1d (&(*pMeshPara).face_cl);
  ut_free_1d (&(*pMeshPara).edge_cl);
  ut_free_1d (&(*pMeshPara).ver_cl);

  ut_free_1d (&(*pMeshPara).mesh3dclreps);

  ut_free_1d_char (&(*pMeshPara).elttype);

  ut_free_2d_char (&(*pMeshPara).face_op, (*pMeshPara).FaceQty + 1);
  ut_free_2d_char (&(*pMeshPara).edge_op, (*pMeshPara).EdgeQty + 1);
  ut_free_2d (&(*pMeshPara).face_eq, (*pMeshPara).FaceQty + 1);

  (*pMeshPara).VerQty = 0;
  (*pMeshPara).EdgeQty = 0;
  (*pMeshPara).FaceQty = 0;
  (*pMeshPara).PolyQty = 0;

  return;
}
