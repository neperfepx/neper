/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

extern int net_reg_merge_del_per_vermerge (struct TESS *pTess, int *edgenewvers, int *edgedelvers, int edgeqty);
extern void net_reg_merge_del_per_verfaces (struct TESS Tess, int *vers, int **offsets, int verqty, int **pfaces, double ***pfacetrans, int *pfaceqty);
extern void net_reg_merge_del_per_setvers (struct TESS *pTess, int ver, int *vers, int **offsets, int verqty);

#ifdef __cplusplus
}
#endif
