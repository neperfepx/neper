/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_cube_.h"

void
IniVerQty (struct POLY *pDomain)
{
  (*pDomain).VerQty = 8;

  return;
}

void
IniVerFace (struct POLY *pDomain)
{
  (*pDomain).VerFace = ut_alloc_2d_int (9, 3);

  (*pDomain).VerFace[1][0] = 1;
  (*pDomain).VerFace[1][1] = 3;
  (*pDomain).VerFace[1][2] = 5;
  (*pDomain).VerFace[2][0] = 2;
  (*pDomain).VerFace[2][1] = 3;
  (*pDomain).VerFace[2][2] = 5;
  (*pDomain).VerFace[3][0] = 2;
  (*pDomain).VerFace[3][1] = 4;
  (*pDomain).VerFace[3][2] = 5;
  (*pDomain).VerFace[4][0] = 1;
  (*pDomain).VerFace[4][1] = 4;
  (*pDomain).VerFace[4][2] = 5;
  (*pDomain).VerFace[5][0] = 1;
  (*pDomain).VerFace[5][1] = 3;
  (*pDomain).VerFace[5][2] = 6;
  (*pDomain).VerFace[6][0] = 1;
  (*pDomain).VerFace[6][1] = 4;
  (*pDomain).VerFace[6][2] = 6;
  (*pDomain).VerFace[7][0] = 2;
  (*pDomain).VerFace[7][1] = 4;
  (*pDomain).VerFace[7][2] = 6;
  (*pDomain).VerFace[8][0] = 2;
  (*pDomain).VerFace[8][1] = 3;
  (*pDomain).VerFace[8][2] = 6;

  return;
}

void
IniVerCoo (double **size, struct POLY *pDomain)
{
  (*pDomain).VerCoo = ut_alloc_2d (9, 3);

  (*pDomain).VerCoo[1][0] = size[0][0];
  (*pDomain).VerCoo[1][1] = size[1][0];
  (*pDomain).VerCoo[1][2] = size[2][0];

  (*pDomain).VerCoo[2][0] = size[0][1];
  (*pDomain).VerCoo[2][1] = size[1][0];
  (*pDomain).VerCoo[2][2] = size[2][0];

  (*pDomain).VerCoo[3][0] = size[0][1];
  (*pDomain).VerCoo[3][1] = size[1][1];
  (*pDomain).VerCoo[3][2] = size[2][0];

  (*pDomain).VerCoo[4][0] = size[0][0];
  (*pDomain).VerCoo[4][1] = size[1][1];
  (*pDomain).VerCoo[4][2] = size[2][0];

  (*pDomain).VerCoo[5][0] = size[0][0];
  (*pDomain).VerCoo[5][1] = size[1][0];
  (*pDomain).VerCoo[5][2] = size[2][1];

  (*pDomain).VerCoo[6][0] = size[0][0];
  (*pDomain).VerCoo[6][1] = size[1][1];
  (*pDomain).VerCoo[6][2] = size[2][1];

  (*pDomain).VerCoo[7][0] = size[0][1];
  (*pDomain).VerCoo[7][1] = size[1][1];
  (*pDomain).VerCoo[7][2] = size[2][1];

  (*pDomain).VerCoo[8][0] = size[0][1];
  (*pDomain).VerCoo[8][1] = size[1][0];
  (*pDomain).VerCoo[8][2] = size[2][1];

  return;
}

void
IniFaceQty (struct POLY *pDomain)
{
  (*pDomain).FaceQty = 6;

  return;
}

void
IniFaceEq (double **size, struct POLY *pDomain)
{
  (*pDomain).FaceEq = ut_alloc_2d (7, 4);

  // an outgoing vector would require scaling by -1
  (*pDomain).FaceEq[1][1] = 1;
  (*pDomain).FaceEq[1][2] = 0;
  (*pDomain).FaceEq[1][3] = 0;
  (*pDomain).FaceEq[1][0] = size[0][0];

  (*pDomain).FaceEq[2][1] = 1;
  (*pDomain).FaceEq[2][2] = 0;
  (*pDomain).FaceEq[2][3] = 0;
  (*pDomain).FaceEq[2][0] = size[0][1];

  // an outgoing vector would require scaling by -1
  (*pDomain).FaceEq[3][1] = 0;
  (*pDomain).FaceEq[3][2] = 1;
  (*pDomain).FaceEq[3][3] = 0;
  (*pDomain).FaceEq[3][0] = size[1][0];

  (*pDomain).FaceEq[4][1] = 0;
  (*pDomain).FaceEq[4][2] = 1;
  (*pDomain).FaceEq[4][3] = 0;
  (*pDomain).FaceEq[4][0] = size[1][1];

  // an outgoing vector would require scaling by -1
  (*pDomain).FaceEq[5][1] = 0;
  (*pDomain).FaceEq[5][2] = 0;
  (*pDomain).FaceEq[5][3] = 1;
  (*pDomain).FaceEq[5][0] = size[2][0];

  (*pDomain).FaceEq[6][1] = 0;
  (*pDomain).FaceEq[6][2] = 0;
  (*pDomain).FaceEq[6][3] = 1;
  (*pDomain).FaceEq[6][0] = size[2][1];

  return;
}

void
IniFacePoly (struct POLY *pDomain)
{
  int i;

  (*pDomain).FacePoly = ut_alloc_1d_int (7);

  for (i = 1; i <= 6; i++)
    (*pDomain).FacePoly[i] = -i;

  return;
}

void
IniFaceVerQty (struct POLY *pDomain)
{
  int i;

  (*pDomain).FaceVerQty = ut_alloc_1d_int (7);

  for (i = 1; i <= 6; i++)
    (*pDomain).FaceVerQty[i] = 4;

  return;
}

void
IniFaceVerNb (struct POLY *pDomain)
{
  (*pDomain).FaceVerNb = ut_alloc_2d_int (7, 5);

  (*pDomain).FaceVerNb[1][1] = 4;
  (*pDomain).FaceVerNb[1][2] = 6;
  (*pDomain).FaceVerNb[1][3] = 5;
  (*pDomain).FaceVerNb[1][4] = 1;
  (*pDomain).FaceVerNb[2][1] = 2;
  (*pDomain).FaceVerNb[2][2] = 8;
  (*pDomain).FaceVerNb[2][3] = 7;
  (*pDomain).FaceVerNb[2][4] = 3;
  (*pDomain).FaceVerNb[3][1] = 1;
  (*pDomain).FaceVerNb[3][2] = 5;
  (*pDomain).FaceVerNb[3][3] = 8;
  (*pDomain).FaceVerNb[3][4] = 2;
  (*pDomain).FaceVerNb[4][1] = 4;
  (*pDomain).FaceVerNb[4][2] = 3;
  (*pDomain).FaceVerNb[4][3] = 7;
  (*pDomain).FaceVerNb[4][4] = 6;
  (*pDomain).FaceVerNb[5][1] = 1;
  (*pDomain).FaceVerNb[5][2] = 2;
  (*pDomain).FaceVerNb[5][3] = 3;
  (*pDomain).FaceVerNb[5][4] = 4;
  (*pDomain).FaceVerNb[6][1] = 5;
  (*pDomain).FaceVerNb[6][2] = 6;
  (*pDomain).FaceVerNb[6][3] = 7;
  (*pDomain).FaceVerNb[6][4] = 8;

  return;
}
