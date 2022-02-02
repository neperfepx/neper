/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesl_tess_.h"

void
neut_tesl_tess (struct TESL Tesl, struct SEEDSET SSet, int level, int tessid,
                struct TESS *pTess)
{
  neut_tess_reset (pTess);

  neut_tesl_tess_general (Tesl, SSet, level, tessid, pTess);
  neut_tesl_tess_ver (Tesl, pTess);
  neut_tesl_tess_edge (Tesl, pTess);
  neut_tesl_tess_face (Tesl, pTess);
  neut_tesl_tess_poly (Tesl, pTess);
  neut_tesl_tess_seed (SSet, pTess);

  return;
}
