/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_periodic_.h"

void
net_tess3d_periodic (struct TESS *pTess)
{
  neut_tess_init_seedslave (pTess);
  net_tess3d_periodic_face (pTess);
  net_tess3d_periodic_edge (pTess);
  net_tess3d_periodic_ver (pTess);

  return;
}
