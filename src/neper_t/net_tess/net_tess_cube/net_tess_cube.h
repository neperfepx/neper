/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern int net_tess_cube (struct IN_T In, int level, struct MTESS *pMTess,
                            struct TESS *Tess, int dtess, int dcell, struct TESS Dom,
                            int TessId, struct SEEDSET *SSet);

#ifdef __cplusplus
}
#endif
