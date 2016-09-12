/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"../neut_tess_test_.h"

extern int neut_tess_test_faceReciprocityEdge (struct TESS, int, int);
extern int neut_tess_test_faceReciprocityVer (struct TESS, int, int);
extern int neut_tess_test_faceReciprocityPoly (struct TESS, int, int);
extern int neut_tess_test_faceState (struct TESS, int, int);
extern int neut_tess_test_faceBound (struct TESS, int, int);
extern int neut_tess_test_faceSelfIntersect (struct TESS, int, int);
extern int neut_tess_test_vervsedge (struct TESS Tess, int face, int
				     verbosity);
