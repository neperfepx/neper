/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"../neut_tess_test_.h"

extern int neut_tess_test_polyReciprocityFace (struct TESS, int, int);
extern int neut_tess_test_polyCoplaneityFace (struct TESS, int, int);
extern int neut_tess_test_polyOrientationFace (struct TESS, int, int);
extern int neut_tess_test_poly_pinching (struct TESS Tess, int i, int verbosity);
