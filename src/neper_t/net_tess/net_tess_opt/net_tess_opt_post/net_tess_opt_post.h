/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"structIn_t.h"

extern void net_tess_opt_post (struct MTESS *pMTess, struct TESS *Tess,
		   int dtess, int dcell, int tessid,
		   struct POLY *Poly, struct TOPT TOpt,
		   struct SEEDSET *SSet);
