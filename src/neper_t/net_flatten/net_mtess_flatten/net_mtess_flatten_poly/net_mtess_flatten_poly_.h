/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_t.h"
#include"net_mtess_flatten_poly.h"

extern void net_mtess_flatten_cell_body (struct MTESS MTess, struct TESS *Tess,
				   int *CTessIds, int CTessQty,
				   struct TESS *pFTess, struct TESSE **pTessE,
				   struct FLATTEN *pFlatten);
