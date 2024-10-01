/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

extern int neut_tess_test_ver (struct TESS, int, int);
extern int neut_tess_test_edge (struct TESS, int, int);
extern int neut_tess_test_face (struct TESS, int, int);
extern int neut_tess_test_poly (struct TESS, int, int);
extern int neut_tess_test_dom (struct TESS, int);
