/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern int net_tess3d (struct TESS PTess, int poly, struct SEEDSET SSet,
			 int TessId, struct MTESS *pMTess,
			 struct TESS *pTess);

  extern void net_polys_tess (int level, struct SEEDSET SSet, int TessId,
			      struct POLY *Poly, struct TESS *pTess);

#ifdef __cplusplus
}
#endif
