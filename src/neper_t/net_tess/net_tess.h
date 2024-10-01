/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern int net_tess (struct IN_T In, int level,
		       struct TESS *Tess, int tess, int cell,
		       struct SEEDSET *SSet, int TessId,
		       struct MTESS *pMTess);

  extern void net_tess_perdomain (struct IN_T In, struct TESS Ptess, int cell,
				  struct TESS *pDom);

  extern int net_tess_3dto2d (struct TESS *pTess);

#ifdef __cplusplus
}
#endif
