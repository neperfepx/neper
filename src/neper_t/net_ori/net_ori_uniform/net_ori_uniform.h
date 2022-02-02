/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include"orilib.h"

  extern void net_ori_uniform (struct IN_T In, int level, struct MTESS MTess,
                               struct TESS *Tess, int dtess, int dcell, long
                               random, struct OL_SET *pOSet, int
                               verbositylevel);

#ifdef __cplusplus
}
#endif
