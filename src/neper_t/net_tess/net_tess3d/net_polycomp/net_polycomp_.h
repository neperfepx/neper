/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

#ifdef HAVE_OPENMP
#include<omp.h>
#endif

#include"neut_t.h"
#include"net_polycomp.h"

extern void net_polycomp_inittdyn (struct POLY Domain, struct SEEDSET SSet,
				   struct TDYN *pTD, struct POLY **pPoly);
extern void net_polycomp_updatedseeds (struct SEEDSET SSet, struct TDYN *pTD,
			   int *updatedseeds_in, int updatedseedqty_in,
			   int **pupdatedseeds, int *pupdatedseedqty);
extern void net_polycomp_shift (struct SEEDSET SSet, struct TDYN *pTD);

extern void net_polycomp_cells_inittdyn (struct TDYN *pTD);
extern int net_polycomp_cells_updatecell_test (struct TDYN TD, int cell);

#ifdef __cplusplus
}
#endif
