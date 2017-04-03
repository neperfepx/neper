/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"../neut_tess_test_.h"

extern int neut_tess_test_verEdgeQtyNNb (struct TESS, int, int);
extern int neut_tess_test_verEdgeReciprocity (struct TESS, int, int);
extern int neut_tess_test_verBoundNCoo (struct TESS, int, int);
extern int neut_tess_test_verFaceCoplaneity (struct TESS, int, int);
