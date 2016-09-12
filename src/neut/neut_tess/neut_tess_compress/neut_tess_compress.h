/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_COMPRESS_H
#define NEUT_TESS_COMPRESS_H

  extern void neut_tess_compress (struct TESS *);
  extern void neut_tess_compress_polys (struct TESS *);
  extern void neut_tess_compress_faces (struct TESS *);
  extern void neut_tess_compress_edges (struct TESS *);
  extern void neut_tess_compress_vers (struct TESS *);

#endif				/* NEUT_TESS_COMPRESS_H */

#ifdef __cplusplus
}
#endif
