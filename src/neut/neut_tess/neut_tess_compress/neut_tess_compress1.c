/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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

  neut_tess_compress_domfaces (pTess);
  neut_tess_compress_domedges (pTess);
  neut_tess_compress_domvers (pTess);

  return;
}
