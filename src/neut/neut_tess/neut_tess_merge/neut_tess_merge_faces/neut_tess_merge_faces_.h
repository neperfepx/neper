/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"neut.h"

#include"neut_tess_merge_faces.h"

extern void
neut_tess_merge_faces_edges (struct TESS *pTess, int *cells, int cellqty,
			     int **pkeepedges, int *pkeepedgeqty,
			     int **pdeledges, int *pdeledgeqty);

extern void neut_tess_merge_faces_facelists (struct TESS *pTess,
					     int *cells, int cellqty,
					     int *pkeepface,
					     int **pdelfaces,
					     int *pdelfaceqty);

extern void neut_tess_merge_faces_edgelists (struct TESS *pTess,
					     int *cells, int cellqty,
					     int **pkeepedges,
					     int *pkeepedgeqty,
					     int **pdeledges,
					     int *pdeledgeqty);

extern void neut_tess_merge_faces_mergefaces (struct TESS *pTess,
					      int keepface, int *delfaces,
					      int delface, int *keepedges,
					      int keepedgeqty);
extern void neut_tess_merge_faces_updateedges (struct TESS *pTess,
					       int newface, int *delfaces,
					       int delfaceqty, int *keepedges,
					       int keepedgeqty, int *deledges,
					       int deledgeqty);
