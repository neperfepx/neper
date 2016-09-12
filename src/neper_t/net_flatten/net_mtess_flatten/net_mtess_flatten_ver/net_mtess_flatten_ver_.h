/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_t.h"
#include"net_mtess_flatten_ver.h"

extern void net_mtess_flatten_ver_init_fvertessver (struct TESS *Tess,
					      int *CTessIds, int CTessQty,
					      struct TESSE *TessE,
					      struct FLATTEN *pFlatten);
