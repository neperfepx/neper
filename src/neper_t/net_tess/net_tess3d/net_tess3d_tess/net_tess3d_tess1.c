/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_tess_.h"

/* net_tess3d_tess builds the tessellation from the polyhedra, by merging
 * the vertices and faces shared by several polyhedra, and recording the
 * edges.
 */
void
net_tess3d_tess (int level, struct SEEDSET SSet, int TessId, struct POLY *Poly, struct TESS *pTess)
{
  /* TFNb[i][j] and TVNb[i][j] are the tessellation ids of face j and
   * vertex j of polyhedron i.
   */
  int **TFNb = ut_alloc_1d_pint (SSet.N + 1);
  int **TVNb = ut_alloc_1d_pint (SSet.N + 1);
  /* VerSeed[i][j] (j=0...3) are the 4 parent seeds of vertex i. */
  int **VerSeed = NULL;

  neut_tess_reset (pTess);

  /* Recording of the general properties */
  net_tess3d_tess_general (SSet, level, TessId, pTess);

  /* Recording of TFNb & TVNb, and of the numbers of faces and vertices */
  net_tess3d_tess_txnb (Poly, TFNb, TVNb, pTess);

  /* Recording of the face properties */
  net_tess3d_tess_face (Poly, TFNb, TVNb, pTess);

  /* Recording of the vertex properties */
  net_tess3d_tess_ver (Poly, TVNb, pTess, &VerSeed);

  /* Recording of the polyhedron properties */
  net_tess3d_tess_poly (Poly, TVNb, TFNb, pTess);

  /* Recording of the edge properties */
  net_tess3d_tess_edge (VerSeed, pTess);

  /* Recording of the seed properties */
  net_tess3d_tess_seed (SSet, pTess);

  ut_free_2d_int (&TFNb, SSet.N + 1);
  ut_free_2d_int (&TVNb, SSet.N + 1);
  ut_free_2d_int (&VerSeed, (*pTess).VerQty + 1);

  return;
}
