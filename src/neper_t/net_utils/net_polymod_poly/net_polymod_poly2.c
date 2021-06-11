/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polymod_poly_.h"

void
Pm2PComp (struct POLYMOD Polymod, int *CompVer, int *CompVerInv,
          int *CompFace, int *CompFaceInv)
{
  /* Determination of CompVer and CompVerInv
   */
  Pm2PCompVerNVerInv (Polymod, CompVer, CompVerInv);
  /* Determination of CompFace and CompFaceInv
   */
  Pm2PCompFaceNFaceInv (Polymod, CompFace, CompFaceInv);

  return;
}

/* Writing of VerQty.
 */
void
Pm2PVerQty (struct POLY *pPoly, int *CompVer)
{
  (*pPoly).VerQty = CompVer[0];

  return;
}

/* Writing of FaceQty.
 */
void
Pm2PFaceQty (struct POLY *pPoly, int *CompFace)
{
  (*pPoly).FaceQty = CompFace[0];

  return;
}

/* Writing of VerFace.
 */
void
Pm2PVerFace (struct POLYMOD Polymod, struct POLY *pPoly, int *CompVer,
             int *CompFaceInv)
{
  int i, j;

  for (i = 1; i <= (*pPoly).VerQty; i++)
    for (j = 0; j <= 2; j++)
      (*pPoly).VerFace[i][j] = CompFaceInv[Polymod.VerFace[CompVer[i]][j]];

  return;
}

/* Writing of VerCoo.
 */
void
Pm2PVerCoo (struct POLYMOD Polymod, struct POLY *pPoly, int *CompVer)
{
  int i, j;

  for (i = 1; i <= (*pPoly).VerQty; i++)
    for (j = 0; j <= 2; j++)
      (*pPoly).VerCoo[i][j] = Polymod.VerCoo[CompVer[i]][j];

  return;
}

/* Writing of FacePoly
 */
void
Pm2PFacePoly (struct POLYMOD Polymod, struct POLY *pPoly, int *CompFace)
{
  int i;

  for (i = 1; i <= (*pPoly).FaceQty; i++)
    (*pPoly).FacePoly[i] = Polymod.FacePoly[CompFace[i]];

  return;
}

/* Writing of FaceEq
 */
void
Pm2PFaceEq (struct POLYMOD Polymod, struct POLY *pPoly, int *CompFace)
{
  int i, j;

  for (i = 1; i <= (*pPoly).FaceQty; i++)
    for (j = 0; j <= 3; j++)
      (*pPoly).FaceEq[i][j] = Polymod.FaceEq[CompFace[i]][j];

  return;
}

/* Writing of FaceVerQty
 */
void
Pm2PFaceVerQty (struct POLYMOD Polymod, struct POLY *pPoly, int *CompFace)
{
  int i;

  for (i = 1; i <= (*pPoly).FaceQty; i++)
    (*pPoly).FaceVerQty[i] = Polymod.FaceVerQty[CompFace[i]];

  return;
}

/* Writing of FaceVerNb
 */
void
Pm2PFaceVerNb (struct POLYMOD Polymod, struct POLY *pPoly, int *CompFace,
               int *CompVerInv)
{
  int i, j;

  for (i = 1; i <= (*pPoly).FaceQty; i++)
  {
    (*pPoly).FaceVerNb[i] = ut_alloc_1d_int ((*pPoly).FaceVerQty[i] + 1);
    for (j = 1; j <= (*pPoly).FaceVerQty[i]; j++)
      (*pPoly).FaceVerNb[i][j] =
        CompVerInv[Polymod.FaceVerNb[CompFace[i]][j]];
  }

  return;
}
