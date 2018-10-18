/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_MEMCPY_H
#define NEUT_TESS_MEMCPY_H

  extern void neut_tess_tess (struct TESS, struct TESS *);
  extern void neut_tess_tess_gen (struct TESS, struct TESS *);
  extern void neut_tess_tess_seed (struct TESS, struct TESS *);
  extern void neut_tess_tess_cell (struct TESS, struct TESS *);
  extern void neut_tess_tess_ver (struct TESS, struct TESS *);
  extern void neut_tess_tess_edge (struct TESS, struct TESS *);
  extern void neut_tess_tess_face (struct TESS, struct TESS *);
  extern void neut_tess_tess_domain (struct TESS, struct TESS *);
  extern void neut_tess_tess_scale (struct TESS TessA, struct TESS *pTessB);

#endif				/* NEUT_TESS_MEMCPY_H */

#ifdef __cplusplus
}
#endif
