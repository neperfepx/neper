/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_poly_.h"

void
UpdFaceQty (struct POLYMOD *pPolymod)
{
  (*pPolymod).FaceQty++;

  return;
}

void
UpdFaceEq (struct POLYMOD *pPolymod, double *plane)
{
  int i;

  (*pPolymod).FaceEq =
    ut_realloc_2d_addline ((*pPolymod).FaceEq, (*pPolymod).FaceQty + 1, 4);

  for (i = 0; i <= 3; i++)
    (*pPolymod).FaceEq[(*pPolymod).FaceQty][i] = plane[i];

  return;
}

void
UpdFaceUse (struct POLYMOD *pPolymod)
{
  (*pPolymod).FaceUse =
    ut_realloc_1d_int ((*pPolymod).FaceUse, (*pPolymod).FaceQty + 1);

  (*pPolymod).FaceUse[(*pPolymod).FaceQty] = 1;

  return;
}

void
UpdFacePoly (struct POLYMOD *pPolymod, int Nei)
{
  (*pPolymod).FacePoly =
    ut_realloc_1d_int ((*pPolymod).FacePoly, (*pPolymod).FaceQty + 1);

  (*pPolymod).FacePoly[(*pPolymod).FaceQty] = Nei;

  return;
}

void
UpdVerQty (struct POLYMOD *pPolymod)
{
  (*pPolymod).VerQty++;

  return;
}

void
UpdVerUse (struct POLYMOD *pPolymod)
{
  (*pPolymod).VerUse =
    ut_realloc_1d_int ((*pPolymod).VerUse, (*pPolymod).VerQty + 1);

  (*pPolymod).VerUse[(*pPolymod).VerQty] = 1;

  return;
}

void
UpdVerFace (struct POLYMOD *pPolymod, int first, int second, int third)
{
  (*pPolymod).VerFace =
    ut_realloc_2d_int_addline ((*pPolymod).VerFace, (*pPolymod).VerQty + 1,
                               3);

  (*pPolymod).VerFace[(*pPolymod).VerQty][0] = first;
  (*pPolymod).VerFace[(*pPolymod).VerQty][1] = second;
  (*pPolymod).VerFace[(*pPolymod).VerQty][2] = third;

  return;
}

void
UpdVerCoo (struct POLYMOD *pPolymod, int first, int second, int third,
           double *coo)
{
  (*pPolymod).VerCoo =
    ut_realloc_2d_addline ((*pPolymod).VerCoo, (*pPolymod).VerQty + 1, 3);

  if (!coo)
    neut_polymod_faces_inter (*pPolymod, first, second, third,
                              (*pPolymod).VerCoo[(*pPolymod).VerQty]);
  else
    ut_array_1d_memcpy (coo, 3, (*pPolymod).VerCoo[(*pPolymod).VerQty]);

  return;
}

void
UpdFaceVerQty (struct POLYMOD *pPolymod)
{
  (*pPolymod).FaceVerQty[(*pPolymod).FaceQty]++;

  return;
}

void
UpdFaceVerNb (struct POLYMOD *pPolymod)
{
  (*pPolymod).FaceVerNb[(*pPolymod).FaceQty] =
    ut_realloc_1d_int ((*pPolymod).FaceVerNb[(*pPolymod).FaceQty],
                       (*pPolymod).FaceVerQty[(*pPolymod).FaceQty] + 1);

  (*pPolymod).FaceVerNb[(*pPolymod).FaceQty][(*pPolymod).
                                             FaceVerQty[(*pPolymod).
                                                        FaceQty]] =
    (*pPolymod).VerQty;

  return;
}

void
SnNS (struct POLYMOD Polymod, int ns, int nf, int *Ver, int *BadVer)
{
  int face;

  Ver[1] = BadVer[ns];
  face = Polymod.VerFace[Ver[1]][nf];
  Ver[0] = NeiVer (Polymod, face, Ver[1], -1);
  Ver[2] = NeiVer (Polymod, face, Ver[1], +1);

  return;
}

int
NeiVer (struct POLYMOD Polymod, int face, int sum, int near)
{
  int NeiPos;

  NeiPos =
    1 + ut_array_1d_int_eltpos (Polymod.FaceVerNb[face] + 1,
                                Polymod.FaceVerQty[face], sum);
  NeiPos = ut_array_rotpos (1, Polymod.FaceVerQty[face], NeiPos, near);

  return Polymod.FaceVerNb[face][NeiPos];
}

int
CommonFaces (POLYMOD pPolymod, int first, int second, int *res)
{
  int i, j;
  int nb = 0;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      if (pPolymod.VerFace[first][i] == pPolymod.VerFace[second][j])
      {
        res[nb] = pPolymod.VerFace[first][i];
        nb++;
        break;
      }

  return nb;
}

/* VerCoupleNFaces rotates FaceVerNb to place the bad vertices at the
 * end, searches couple1, couple2, faces1, faces2 and supplies bel2.
 */
int
VerCoupleNFaces (struct POLYMOD *pPolymod, int *BadVer, int Face, int pFace,
                 int *couple1, int *couple2, int *faces1, int *faces2,
                 int *pNbBadVer)
{
  /* FaceVerNb[Face] is rotated to put all the bad vertices at the end.
   * The amount of bad vertices is recorded.
   */
  (*pNbBadVer) = RotFaceVerNb (pPolymod, BadVer, Face);

  /* The 2 couples of vertices of which segment is cut by the new plane
   * are recorded as couple1 & couple2.
   * The faces associated to these couples of vertices are recorded.
   * faces1[0] and faces1[1] are the two common faces of vertices
   * couple1[0] & couple1[1]. The same for faces2.
   */
  couple1[0] = (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]];
  couple1[1] = (*pPolymod).FaceVerNb[Face][1];
  couple2[0] =
    (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face] - (*pNbBadVer)];
  couple2[1] =
    (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face] - (*pNbBadVer) +
                                1];

  /* Recording of the common faces of couple1 vertices and couple2
   * vertices.
   */
  CommonFaces (*pPolymod, couple1[0], couple1[1], faces1);
  CommonFaces (*pPolymod, couple2[0], couple2[1], faces2);

  return ut_array_1d_int_eltpos (faces2, 2, pFace);
}

/* Modification of Face propoerties to suppress the bad vertices.
 */
void
FaceModifGenPara (struct POLYMOD *pPolymod, int Face, int NbBadVer)
{
  int i;

  /* Suppression of the bad vertices */
  for (i = 1; i <= NbBadVer; i++)
    (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face] + 1 - i] = 0;
  (*pPolymod).FaceVerQty[Face] -= NbBadVer;

  return;
}

/* Adding of the first new vertex.
 */
void
FaceModif1stNewVer (struct POLYMOD *pPolymod, int Face)
{
  /* REGISTERING OF THE FIRST NEW SUMMIT, i.e. that created at the previous
   * step.
   */
  (*pPolymod).FaceVerQty[Face]++;
  (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]] =
    (*pPolymod).VerQty;

  return;
}

/* Adding of the second new vertex.
 */
void
FaceModif2ndNewVer (struct POLYMOD *pPolymod, int Face, int bel2, int next,
                    double *coo)
{
  /* The FaceVerQty is incremented.
   */
  (*pPolymod).FaceVerQty[Face]++;

  /* Creation of the new vertex as the intersection between faces
   * Face, next & (*pPolymod).FaceQty.
   */
  NewVer (pPolymod, Face, next, (*pPolymod).FaceQty, coo);

  /* Adding of this vertex
   */
  (*pPolymod).FaceVerNb[Face] =
    ut_realloc_1d_int ((*pPolymod).FaceVerNb[Face],
                       (*pPolymod).FaceVerQty[Face] + 1);

  (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]] =
    (*pPolymod).VerQty;

  if (bel2 == -1)
    ut_array_1d_int_switch ((*pPolymod).FaceVerNb[Face],
                            (*pPolymod).FaceVerQty[Face] - 1,
                            (*pPolymod).FaceVerQty[Face]);

  return;
}

/* Else, second vertex of the face is set to the first created vertex.
 */
void
FaceModifAddBegVer (struct POLYMOD *pPolymod, int Face, int last, int bel2)
{
  (*pPolymod).FaceVerQty[Face]++;

  (*pPolymod).FaceVerNb[Face] =
    ut_realloc_1d_int ((*pPolymod).FaceVerNb[Face],
                       (*pPolymod).FaceVerQty[Face] + 1);

  (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]] = last;
  if (bel2 == -1)
    ut_array_1d_int_switch ((*pPolymod).FaceVerNb[Face],
                            (*pPolymod).FaceVerQty[Face] - 1,
                            (*pPolymod).FaceVerQty[Face]);

  return;
}
