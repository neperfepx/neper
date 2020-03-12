/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_COMPRESS_H
#define NEUT_TESS_COMPRESS_H

  /// Compressing tessellation, by removing useless vertices, edges, faces and polys.
  /// If the domain is defined, then the DomTess* members are updated as well, for
  /// consistency.
  /// This function does not remove useless domain vertices, edges, faces and polys.
  extern void neut_tess_compress (struct TESS *);
  extern void neut_tess_compress_polys (struct TESS *);
  extern void neut_tess_compress_faces (struct TESS *);
  extern void neut_tess_compress_edges (struct TESS *);
  extern void neut_tess_compress_vers (struct TESS *);

  extern void neut_tess_compress_domfaces (struct TESS *pTess);
  extern void neut_tess_compress_domedges (struct TESS *pTess);
  extern void neut_tess_compress_domvers (struct TESS *pTess);

#endif				/* NEUT_TESS_COMPRESS_H */

#ifdef __cplusplus
}
#endif
