/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"neut.h"

#include "neut_tess_merge_edges/neut_tess_merge_edges.h"
#include "neut_tess_merge_faces/neut_tess_merge_faces.h"
#include "neut_tess_merge_polys/neut_tess_merge_polys.h"

extern void neut_tess_merge_ondomain (struct TESS *pTess);
extern void neut_tess_merge_finalize (struct TESS *pTess);
