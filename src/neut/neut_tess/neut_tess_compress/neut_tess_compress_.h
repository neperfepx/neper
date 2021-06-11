/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"neut.h"

extern void neut_tess_compress_movepoly (struct TESS *, int, int);
extern void neut_tess_compress_moveface (struct TESS *, int, int);
extern void neut_tess_compress_moveedge (struct TESS *, int, int);
extern void neut_tess_compress_movever (struct TESS *, int, int);

extern void neut_tess_compress_movedomface (struct TESS *, int, int);
extern void neut_tess_compress_movedomedge (struct TESS *, int, int);
extern void neut_tess_compress_movedomver (struct TESS *, int, int);
