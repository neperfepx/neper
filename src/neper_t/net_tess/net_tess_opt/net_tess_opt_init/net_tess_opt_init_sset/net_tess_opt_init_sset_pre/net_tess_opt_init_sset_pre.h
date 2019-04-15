/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"structIn_t.h"

extern void net_tess_opt_init_sset_general (struct IN_T In,
					    struct MTESS MTess,
					    struct TESS *Tess, int dtess,
					    int dcell, struct SEEDSET *SSet,
					    struct SEEDSET *pSSet, int
					    CellQty, struct TOPT *pTOpt);

extern void net_tess_opt_init_sset_pre (struct IN_T In, int level,
					struct MTESS MTess, struct TESS *Tess,
					int domtess, int domcell,
					char **pvar, int *ppos,
					char **pweightexpr, char **pcooexpr,
					struct TOPT *pTOpt);

extern void net_tess_opt_init_sset_pre_default_coo (struct TOPT TOpt, int pos, char
    *var, char **pcooexpr);
extern void net_tess_opt_init_sset_pre_default_weight (struct TOPT TOpt, char
    *var, char **pweightexpr);
