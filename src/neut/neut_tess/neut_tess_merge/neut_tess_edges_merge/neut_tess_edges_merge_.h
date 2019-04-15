/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"neut.h"

#include"neut_tess_edges_merge.h"

extern void neut_tess_edges_merge_edgelists (struct TESS *pTess, int *edges,
    int edgeqty, int *pkeepedge, int **pdeledges, int *pdeledgeqty);

extern void neut_tess_edges_merge_verlists (struct TESS *pTess, int *edges, int
    edgeqty, int keepedge, int **pkeepvers, int **pdelvers, int *pdelverqty);

extern void neut_tess_edges_merge_checkfaces (struct TESS *pTess, int keepedge);
