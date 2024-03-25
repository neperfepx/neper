/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include"orilib.h"

  extern void net_ori (struct IN_T In, int level, struct MTESS MTess, struct TESS *Tess,
                       struct SEEDSET *SSet, int dtess, int dcell, struct SEEDSET *pSSet,
                       int verbositylevel);

  extern void net_ori_pre (struct IN_T In, struct MTESS MTess,
                           struct TESS *Tess, struct SEEDSET **pSSet, int *pSSetQty);

  extern void net_ori_mtess_id (struct IN_T In, struct MTESS MTess, struct TESS *Tess,
                    int dtess, int dcell, struct SEEDSET *pSSet);

  extern void net_ori_mtess_randseed (struct MTESS MTess, struct TESS *Tess, int domtess,
                        int dompoly, struct SEEDSET *SSet, int CellQty,
                        struct SEEDSET *pSSet);

  extern void net_ori_random (long random, struct OL_SET *pOSet);

  extern void net_ori_file (char *label, struct OL_SET *pOSet);
  extern void net_ori_memcpy (struct OL_SET OSet, struct SEEDSET *pSSet);

  extern void net_ori_post (struct TESR *pTesr);

#ifdef __cplusplus
}
#endif
