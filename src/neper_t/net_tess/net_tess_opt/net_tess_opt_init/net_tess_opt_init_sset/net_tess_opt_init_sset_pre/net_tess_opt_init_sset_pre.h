/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"structIn_t.h"

extern void net_tess_opt_init_sset_general (struct IN_T In,
					    struct MTESS MTess,
					    struct TESS *Tess, int dtess,
					    int dcell, struct SEEDSET *SSet,
					    struct SEEDSET *pSSet, int
					    CellQty, struct TOPT *pTOpt);

extern void net_tess_opt_init_sset_pre_randseed (struct MTESS MTess,
						 struct TESS *Tess,
						 int domtess, int dompoly,
						 struct SEEDSET *SSet,
						 int CellQty,
						 struct SEEDSET *pSSet);

extern void net_tess_opt_init_sset_pre (struct IN_T In, int level,
					char **pvar, int *ppos,
					char **pweightexpr, char **pcooexpr,
					struct TOPT *pTOpt);
