/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"neut.h"

#include "neut_tess_edges_merge/neut_tess_edges_merge.h"
#include "neut_tess_faces_merge/neut_tess_faces_merge.h"
#include "neut_tess_polys_merge/neut_tess_polys_merge.h"

// merging faces of a poly, when they can be, are contiguous and coplanar (to
// the level provided by 'coplanar')
extern void neut_tess_merge_polyfaces (struct TESS *pTess, int poly, double coplanar);

// merging edges of a poly, when they can be, are contiguous and colinear (to
// the level provided by 'colinear')
extern void neut_tess_merge_polyedges (struct TESS *pTess, int poly, double colinear);
