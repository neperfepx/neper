/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void net_transform_tess_cut (char *expr, struct TESS Dom, struct TESS *pTess);
  extern void net_transform_tess_cut_pre_prim (char *expr, struct PRIM **pPrim, int *pPrimQty, int verbosity);

#ifdef __cplusplus
}
#endif
