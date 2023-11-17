/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_periodic_.h"

void
net_tess3d_periodic (struct TESS *pTess)
{
  // standard case
  if (strncmp ((*pTess).DomType, "rodrigues", 9))
  {
    neut_tess_init_seedslave (pTess);
    net_tess3d_periodic_face (pTess);
    net_tess3d_periodic_edge (pTess);
    net_tess3d_periodic_ver (pTess);
  }

  // Rodrigues space
  else
  {
    // setting periodicity general (not actually used, but allows to get through)
    ut_string_string ("periodic", &(*pTess).Type);
    ut_array_1d_int_set ((*pTess).Periodic, 3, 1);
    neut_tess_bboxsize (*pTess, (*pTess).PeriodicDist);

    net_tess3d_periodic_face_rodrigues (pTess);
    net_tess3d_periodic_edge_rodrigues (pTess);
    net_tess3d_periodic_ver_rodrigues (pTess);

    // unsetting domain stuff
    ut_array_1d_int_set ((*pTess).DomTessFaceQty + 1, (*pTess).DomFaceQty, 0);
    ut_array_1d_int_set ((*pTess).DomTessEdgeQty + 1, (*pTess).DomEdgeQty, 0);
    ut_array_1d_int_set ((*pTess).DomTessVerNb + 1, (*pTess).DomVerQty, 0);
  }

  return;
}
