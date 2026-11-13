/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_compress_.h"

void
neut_tess_compress (struct TESS *pTess)
{
  if ((*pTess).Dim == 3)
    neut_tess_compress_polys (pTess);

  neut_tess_compress_faces (pTess);
  neut_tess_compress_edges (pTess);
  neut_tess_compress_vers (pTess);

  // in a periodic tessellation, the periodic domain entities include no
  // tessellation entities, and must be kept
  if (!(*pTess).Type || strcmp ((*pTess).Type, "periodic"))
  {
    neut_tess_compress_domfaces (pTess);
    neut_tess_compress_domedges (pTess);
    neut_tess_compress_domvers (pTess);
  }

  return;
}
