/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess3d_.h"

void
net_polys_tess (int level, struct SEEDSET SSet, int TessId, struct POLY *Poly,
                struct TESS *pTess)
{
  struct TESL Tesl;

  neut_tesl_set_zero (&Tesl);

  net_tesl (SSet, Poly, &Tesl);
  neut_tesl_tess (Tesl, SSet, level, TessId, pTess);

  neut_tesl_free (&Tesl);

  return;
}
