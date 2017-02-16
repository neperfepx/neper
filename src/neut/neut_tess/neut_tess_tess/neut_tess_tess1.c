/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_tess_.h"

void
neut_tess_tess (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_free (pTessB);

  neut_tess_tess_gen (TessA, pTessB);
  neut_tess_tess_cell (TessA, pTessB);
  neut_tess_tess_ver (TessA, pTessB);
  neut_tess_tess_edge (TessA, pTessB);
  neut_tess_tess_face (TessA, pTessB);
  neut_tess_tess_poly (TessA, pTessB);
  neut_tess_tess_seed (TessA, pTessB);
  neut_tess_tess_domain (TessA, pTessB);
  if (TessA.Type && !strcmp (TessA.Type, "periodic"))
    neut_tess_tess_per (TessA, pTessB);
  neut_tess_tess_scale (TessA, pTessB);

  return;
}
