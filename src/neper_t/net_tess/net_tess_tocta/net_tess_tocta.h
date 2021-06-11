/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern int net_tess_tocta (struct IN_T In, int level, char *morpho,
      struct MTESS *pMTess, struct TESS *Tess, int domtess, int dompoly,
      int TessId, struct SEEDSET *SSet);

#ifdef __cplusplus
}
#endif
