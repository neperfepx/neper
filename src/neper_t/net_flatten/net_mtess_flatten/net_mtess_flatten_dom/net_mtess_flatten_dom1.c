/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_dom_.h"

void
net_mtess_flatten_dom (struct TESS *Tess, int TessId, struct TESS *pFTess,
                       struct TESSE *TessE)
{
  net_mtess_flatten_dom_gen (Tess[TessId], pFTess);

  net_mtess_flatten_dom_ver (Tess, TessId, pFTess, TessE);

  net_mtess_flatten_dom_edge (Tess, TessId, pFTess, TessE);

  net_mtess_flatten_dom_face (Tess, TessId, pFTess, TessE);

  neut_tess_init_alldom_fromdomtess (pFTess);

  return;
}
