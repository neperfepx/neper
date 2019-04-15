/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"../neut_tess_test_.h"

extern int neut_tess_test_edgeLength (struct TESS, int, int);
extern int neut_tess_test_edgeReciprocityVer (struct TESS, int, int);
extern int neut_tess_test_edgeReciprocityFace (struct TESS, int, int);
