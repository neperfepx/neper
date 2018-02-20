/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<string.h>
#include<orilib.h>

#include"../structIn_t.h"
#include"neut_t.h"

#include"net_transform.h"

extern void net_transform_tess (struct IN_T In, struct TESS *pTess);
extern void net_transform_tesr (struct IN_T In, struct TESR *pTesr);

extern void net_tesr_transform_cell (struct TESR Tesr, int cell, int dim,
				  struct TESR *pTesr2, int *prptqty,
				  int *pcellqty);
extern void net_tesr_cell_segment (struct TESR Tesr, int cell, int dim,
				   struct TESR *pSeg);
extern void net_tesr_segment (struct TESR Tesr, int dim, struct TESR *pSeg);

extern void net_tesr_transform_cell_find (struct TESR Tesr, int dim,
				       int *prptqty, int ***prptpos);
extern void net_tesr_transform_cell_remove (struct TESR *pTesr, int dim,
					 int rptqty, int **rptpos);

extern int net_tesr_transform_noise (struct TESR *pTesr, int poly, int
				  qty, int itermax, int *piterqty);
