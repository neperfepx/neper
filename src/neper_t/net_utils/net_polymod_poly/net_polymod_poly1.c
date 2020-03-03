/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polymod_poly_.h"

/* The polyhedron description is as poly[Center].
 */
void
net_polymod_poly (struct POLYMOD Polymod, struct POLY *pPoly)
{
  int *CompVer = NULL;
  int *CompVerInv = NULL;
  int *CompFace = NULL;
  int *CompFaceInv = NULL;
  /* ver i in poly     == ver CompVer[i]      in polymod
   * ver i in polymod  == ver CompVerInv[i]   in poly
   */
  /* face i in poly    == face CompFace[i]    in polymod
   * face i in polymod == face CompFaceInv[i] in poly
   */

  /* Memory allocation of CompVer, CompVerInv, CompFace & CompFaceInv.
   */
  CompVer = ut_alloc_1d_int (Polymod.VerQty + 1);
  CompVerInv = ut_alloc_1d_int (Polymod.VerQty + 1);
  CompFace = ut_alloc_1d_int (Polymod.FaceQty + 1);
  CompFaceInv = ut_alloc_1d_int (Polymod.FaceQty + 1);

  /* Initialization of CompVer, CompVerInv, CompFace & CompFaceInv.
   */
  Pm2PComp (Polymod, CompVer, CompVerInv, CompFace, CompFaceInv);

  /* Writing of VerQty.
   */
  Pm2PVerQty (pPoly, CompVer);
  /* Writing of FaceQty.
   */
  Pm2PFaceQty (pPoly, CompFace);

  /* Memory allocation of VerFace & VerCoo.
   */
  (*pPoly).VerFace = ut_alloc_2d_int (CompVer[0] + 1, 3);
  (*pPoly).VerCoo = ut_alloc_2d (CompVer[0] + 1, 3);

  /* Writing of VerFace & VerCoo
   */
  Pm2PVerFace (Polymod, pPoly, CompVer, CompFaceInv);
  Pm2PVerCoo (Polymod, pPoly, CompVer);

  /* Memory allocation of Face****.
   */
  (*pPoly).FacePoly = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
  (*pPoly).FaceEq = ut_alloc_2d ((*pPoly).FaceQty + 1, 4);
  (*pPoly).FaceVerQty = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
  (*pPoly).FaceVerNb = ut_alloc_1d_pint ((*pPoly).FaceQty + 1);

  /* Writing of FacePoly
   */
  Pm2PFacePoly (Polymod, pPoly, CompFace);
  /* Writing of FaceEq
   */
  Pm2PFaceEq (Polymod, pPoly, CompFace);
  /* Writing of FaceVerQty
   */
  Pm2PFaceVerQty (Polymod, pPoly, CompFace);
  /* Writing of FaceVerNb
   */
  Pm2PFaceVerNb (Polymod, pPoly, CompFace, CompVerInv);

  /* Memory allocation of temporary used arrays.
   */
  ut_free_1d_int (&CompVer);
  ut_free_1d_int (&CompVerInv);
  ut_free_1d_int (&CompFace);
  ut_free_1d_int (&CompFaceInv);

  return;
}
