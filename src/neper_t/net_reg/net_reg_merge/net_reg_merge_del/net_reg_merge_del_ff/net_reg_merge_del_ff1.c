/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_ff_.h"

double
net_reg_merge_del_ff (struct TESS *pTess, int ver)
{
  int i, faceqty;
  double res;
  int *face = NULL;
  double *FFF = NULL;

  neut_tess_ver_faces (*pTess, ver, &face, &faceqty);

  FFF = ut_alloc_1d (faceqty);
  for (i = 0; i < faceqty; i++)
    FFF[i] = FFBaryFace (pTess, face[i]);

  res = ut_array_1d_max (FFF, faceqty);

  ut_free_1d_int (face);
  ut_free_1d (FFF);

  return res;
}
