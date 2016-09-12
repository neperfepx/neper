/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"
#include"neut_t.h"

#include"net_polymod_poly.h"

extern void Pm2PComp (struct POLYMOD, int *, int *, int *, int *);
extern void Pm2PVerQty (struct POLY *, int *);
extern void Pm2PFaceQty (struct POLY *, int *);
extern void Pm2PCompVerNVerInv (struct POLYMOD, int *, int *);
extern void Pm2PCompFaceNFaceInv (struct POLYMOD, int *, int *);
extern void Pm2PVerCoo (struct POLYMOD, struct POLY *, int *);
extern void Pm2PVerFace (struct POLYMOD, struct POLY *, int *, int *);
extern void Pm2PFacePoly (struct POLYMOD, struct POLY *, int *);
extern void Pm2PFaceEq (struct POLYMOD, struct POLY *, int *);
extern void Pm2PFaceVerQty (struct POLYMOD, struct POLY *, int *);
extern void Pm2PFaceVerNb (struct POLYMOD, struct POLY *, int *, int *);
