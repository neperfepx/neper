/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void net_domain (struct IN_T In, struct MTESS *pMTess,
			  struct TESS *pDomain);
  extern void net_domain_clip (struct POLY *, double **, int);

#ifdef __cplusplus
}
#endif
