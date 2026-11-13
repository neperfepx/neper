/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void net_mtess_flatten_per_init (struct MTESS MTess,
                                          struct TESS *Tess, int TessId,
                                          struct MTESS *pMTess2,
                                          struct TESS **pTess2,
                                          int **pVTessIds, int *pVTessQty);
  extern void net_mtess_flatten_per_free (struct MTESS MTess, int TessId,
                                          struct MTESS *pMTess2,
                                          struct TESS **pTess2,
                                          int **pVTessIds, int VTessQty);
  extern void net_mtess_flatten_per (struct MTESS MTess, struct TESS *Tess,
                                     int TessId, int *VTessIds,
                                     int VTessQty, struct TESSE *TessE,
                                     struct TESS *pFTess);

#ifdef __cplusplus
}
#endif
