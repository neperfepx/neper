/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<limits.h>

#include"neut_t.h"

#include"net_polycomp_poly.h"

  extern int CurrentPolyTest (struct POLYMOD Polymod, struct SEEDSET SSet,
			      int id, int neighpos, struct TDYN TD,
			      double wmax);

  extern int NewPolyhedron (struct SEEDSET SSet, int PolyId, int Nei,
			     struct POLYMOD *pPolymod);

  extern void BadVerDet (double *plane, int CenterSide,
			 struct POLYMOD Polymod, int **pBadVer);

  extern int VerUseUpdating (POLYMOD * pPolymod, int *BadVer);
  extern void FaceUseUpdating (POLYMOD * pPolymod, int *BadVer);

  extern int CheckFace (int *BadVer, struct POLYMOD Polymod, int face);
  extern void NewFace (int Nei, POLYMOD * pPolymod, double *plane);
  extern void FnLFacesDet (POLYMOD Polymod, int *BadVer, int *FnLFaces);
  extern int FaceModif (int pFace, int Face, POLYMOD * pPolymod, int
			*BadVer, int last);
  extern int NewVer (POLYMOD * pPolymod, int first, int second, int third, double *coo);
  extern void NewFaceAddVer (POLYMOD * pPolymod);

  extern void UpdFaceQty (struct POLYMOD *pPolymod);
  extern void UpdFaceEq (struct POLYMOD *pPolymod, double *plane);
  extern void UpdFaceUse (struct POLYMOD *pPolymod);
  extern void UpdFacePoly (struct POLYMOD *pPolymod, int Nei);
  extern void UpdVerQty (struct POLYMOD *pPolymod);
  extern void UpdVerUse (struct POLYMOD *pPolymod);
  extern void UpdVerFace (struct POLYMOD *pPolymod, int first, int second,
			  int third);
  extern void UpdVerCoo (struct POLYMOD *pPolymod, int first, int second,
			 int third, double *coo);
  extern void UpdFaceVerQty (struct POLYMOD *pPolymod);
  extern void UpdFaceVerNb (struct POLYMOD *pPolymod);
  extern void SnNS (struct POLYMOD Polymod, int ns, int nf, int *Ver,
		    int *BadVer);
  extern int NeiVer (struct POLYMOD Polymod, int face, int sum, int near);
  extern int CommonFaces (POLYMOD pPolymod, int first, int second, int *res);
  extern int VerCoupleNFaces (struct POLYMOD *pPolymod, int *BadVer, int Face,
			      int pFace, int *couple1, int *couple2,
			      int *faces1, int *faces2, int *pNbBadVer);
  extern void FaceModifGenPara (struct POLYMOD *pPolymod, int Face,
				int NbBadVer);
  extern void FaceModif1stNewVer (struct POLYMOD *pPolymod, int Face);
  extern void FaceModif2ndNewVer (struct POLYMOD *pPolymod, int Face, int bel2,
				 int next, double *coo);
  extern void FaceModifAddBegVer (struct POLYMOD *pPolymod, int Face,
				  int last, int bel2);

  extern int RotFaceVerNb (struct POLYMOD *pPolymod, int *BadVer, int Face);
#ifdef __cplusplus
}
#endif
