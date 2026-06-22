/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

extern int net_tess_opt (struct IN_T In, int level, char *optitype,
                         char *optistring, struct TESS *Tess, int dtess,
                         int dcell, int TessId, struct MTESS *pMTess,
                         struct SEEDSET *SSet);

#ifdef __cplusplus
}
#endif
