/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_edge_.h"

void
net_mtess_flatten_edge_domedge_linkedge (int fedge, struct TESSE *TessE,
                                         int tess, int edge, int ori)
{
  TessE[tess].EdgeFEdgeQty[edge]++;
  TessE[tess].EdgeFEdgeNb[edge] =
    ut_realloc_1d_int (TessE[tess].EdgeFEdgeNb[edge],
                       TessE[tess].EdgeFEdgeQty[edge]);
  TessE[tess].EdgeFEdgeOri[edge] =
    ut_realloc_1d_int (TessE[tess].EdgeFEdgeOri[edge],
                       TessE[tess].EdgeFEdgeQty[edge]);

  if (ori != -1 && ori != 1)
    ut_print_neperbug ();

  TessE[tess].EdgeFEdgeNb[edge][TessE[tess].EdgeFEdgeQty[edge] - 1] = fedge;
  TessE[tess].EdgeFEdgeOri[edge][TessE[tess].EdgeFEdgeQty[edge] - 1] = ori;

  return;
}
