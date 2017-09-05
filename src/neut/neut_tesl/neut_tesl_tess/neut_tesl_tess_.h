/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"neut_tesl_tess.h"

extern void neut_tesl_tess_general (struct TESL, struct SEEDSET, int, int,
				    struct TESS *);
extern void neut_tesl_tess_ver (struct TESL, struct TESS *);
extern void neut_tesl_tessVerQty (struct TESL, struct TESS *);
extern void neut_tesl_tessVerCoo (struct TESL, struct TESS *);
extern void neut_tesl_tessVerEdge (struct TESL, struct TESS *);
extern void neut_tesl_tessVerState (struct TESS *);
extern void neut_tesl_tess_edge (struct TESL, struct TESS *);
extern void neut_tesl_tessEdgeVerNb (struct TESL, struct TESS *);
extern void neut_tesl_tessEdgeFace (struct TESL, struct TESS *);
extern void neut_tesl_tessEdgeState (struct TESS *);
extern void neut_tesl_tessEdgeDel (struct TESS *);
extern void neut_tesl_tess_face (struct TESL, struct TESS *);
extern void neut_tesl_tessFaceQty (struct TESL, struct TESS *);
extern void neut_tesl_tessFacePoly (struct TESL, struct TESS *);
extern void neut_tesl_tessFaceVer (struct TESL, struct TESS *);
extern void neut_tesl_tessFaceEdge (struct TESL, struct TESS *);
extern void neut_tesl_tessFaceState (struct TESS *);
extern void neut_tesl_tessFacePt (struct TESS *);
extern void neut_tesl_tessFaceEq (struct TESL, struct TESS *);
extern void neut_tesl_tess_poly (struct TESL, struct TESS *);
extern void neut_tesl_tess_seed (struct SEEDSET, struct TESS *);
extern void neut_tesl_tessPolyQty (struct TESL, struct TESS *);
extern void neut_tesl_tessPolyFace (struct TESL, struct TESS *);
extern void neut_tesl_tessPolyState (struct TESS *);
