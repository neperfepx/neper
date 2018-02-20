/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_meshpara_.h"

void
neut_meshpara_set_zero (struct MESHPARA *pMeshPara)
{
  (*pMeshPara).cltype = NULL;
  (*pMeshPara).clratio = NULL;

  (*pMeshPara).poly_cl = NULL;
  (*pMeshPara).face_cl = NULL;
  (*pMeshPara).edge_cl = NULL;
  (*pMeshPara).ver_cl = NULL;

  (*pMeshPara).face_eq = NULL;
  (*pMeshPara).face_op = NULL;
  (*pMeshPara).edge_op = NULL;

  return;
}

void
neut_meshpara_free (struct MESHPARA MeshPara)
{
  ut_free_1d_char (MeshPara.cltype);
  ut_free_1d (MeshPara.clratio);

  ut_free_1d (MeshPara.poly_cl);
  ut_free_1d (MeshPara.face_cl);
  ut_free_1d (MeshPara.edge_cl);
  ut_free_1d (MeshPara.ver_cl);

  return;
}
