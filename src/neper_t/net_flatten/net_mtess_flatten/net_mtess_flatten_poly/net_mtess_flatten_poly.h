/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void net_mtess_flatten_poly (struct MTESS MTess, struct TESS *Tess,
				int TessId, int *CTessIds, int CTessQty,
				struct TESS *pFTess, struct TESSE **pTessE,
				struct FLATTEN *pFlatten);

#ifdef __cplusplus
}
#endif
