/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

void
net_tess_opt_init (struct IN_T In, int level, char *optitype,
                   char *optistring, struct MTESS MTess,
		   struct TESS *Tess, int dtess, int dcell, int TessId,
		   struct SEEDSET *SSet, struct TOPT *pTOpt);

#ifdef __cplusplus
}
#endif
