/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

// number of facets of the faceted cylinder an hourglass is generated from
#define NET_DOMAIN_HOURGLASS_FACETQTY 100

// relative gap between the faceted hull of a curved domain and its surface
#define NET_DOMAIN_HULLGAP 0.01

  extern void net_domain (struct IN_T In, struct MTESS *pMTess,
			  struct TESS *pDomain);
  extern void net_domain_fromtess (struct TESS Tess, struct TESS *pDomain);
  extern void net_domain_cut (struct TESS Dom, struct TESS *pTess);
  extern void net_domain_clip (struct POLY *, double **, int);

#ifdef __cplusplus
}
#endif
