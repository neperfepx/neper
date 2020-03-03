/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"neut.h"

extern void neut_tess_polys_merge_polylists (struct TESS *pTess, int
					     *cells, int cellqty,
					     int *pkeeppoly, int **pdelpolys,
					     int *pdelpolyqty);

extern void neut_tess_polys_merge_facelists (struct TESS *pTess, int
					     *cells, int cellqty,
					     int **pkeepfaces,
					     int *pkeepfaceqty,
					     int **pdelfaces,
					     int *pdelfaceqty);

extern void neut_tess_polys_merge_edgelists (struct TESS *pTess, int
					     *delfaces, int delfaceqty,
					     int **pkeepedges,
					     int *pkeepedgeqty,
					     int **pdeledges,
					     int *pdeledgeqty);

extern void neut_tess_polys_merge_mergepolys (struct TESS *pTess, int
					      newpoly, int *delpolys,
					      int delpolyqty, int *keepfaces,
					      int keepfaceqty);

extern void neut_tess_polys_merge_updatefaces (struct TESS *pTess,
					       int *delfaces, int delfaceqty);

extern void neut_tess_polys_merge_updateedges (struct TESS *pTess,
					       int *delfaces, int delfaceqty,
					       int *keepedges,
					       int keepedgeqty, int *deledges,
					       int deledgeqty);

#include "../neut_tess_faces_merge/neut_tess_faces_merge.h"
