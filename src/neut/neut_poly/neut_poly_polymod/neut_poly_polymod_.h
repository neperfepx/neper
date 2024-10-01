/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"neut.h"

#include"neut_poly_polymod.h"

extern void neut_poly_polymodVerQty (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodFaceQty (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodVerProp (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodFaceProp (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodVerFace (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodVerCoo (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodVerUse (struct POLYMOD *);
extern void neut_poly_polymodFacePoly (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodFaceEq (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodFaceVerQty (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodFaceVerNb (struct POLY, struct POLYMOD *);
extern void neut_poly_polymodFaceUse (struct POLYMOD *);
