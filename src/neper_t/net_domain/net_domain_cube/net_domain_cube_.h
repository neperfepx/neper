/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>

#include"../../structIn_t.h"
#include"ut.h"
#include"neut_t.h"

#include"net_domain_cube.h"

extern void net_domain_cubeparms (char *domain, double **size, int *ppseudodim, double *ppseudosize);

extern void CubeVer (double **, struct POLY *);
extern void IniVerQty (struct POLY *);
extern void IniVerFace (struct POLY *);
extern void IniVerCoo (double **, struct POLY *);
extern void IniVerUse (struct POLY *);

extern void CubeFaces (double **, struct POLY *);
extern void IniFaceQty (struct POLY *);
extern void IniFaceEq (double **, struct POLY *);
extern void IniFacePoly (struct POLY *);
extern void IniFaceVerQty (struct POLY *);
extern void IniFaceVerNb (struct POLY *);
extern void IniFaceUse (struct POLY *);
