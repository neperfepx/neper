/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void net_tess3d_periodic (struct TESS *pTess);
  extern void net_tess3d_periodic_ver_shrinkmaster (struct TESS *);
  extern void net_tess3d_periodic_edge_shrinkmaster (struct TESS *);
  extern void net_tess2d_periodic_edge_peredgeori (struct TESS *pTess);
  extern void net_tess3d_periodic_edge_peredgeori (struct TESS *pTess);

#ifdef __cplusplus
}
#endif
