/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_.h"
#include"neut_nanoflann_struct.hpp"

extern void net_polycomp_poly (struct POLY Domain, struct SEEDSET SSet,
                               NFTREE ** pnf_tree, int *ptid_seedid,
                               int PolyId, struct POLY *pPoly,
                               struct TDYN *pTD);

void
net_polycomp_cells_inittdyn (struct TDYN *pTD)
{
  ut_free_1d_int (&(*pTD).cellchanged);
  (*pTD).cellchangedqty = 0;
  ut_free_1d_int (&(*pTD).changedneighs);
  (*pTD).changedneighqty = 0;

  return;
}

void
net_polycomp_cells_updatecell (struct POLY Domain, struct SEEDSET SSet,
                               NFTREE ** pnf_tree, int *ptid_seedid, int cell,
                               struct POLY **pPoly, struct TDYN *pTD)
{
  int eltpos;

#pragma omp critical
  eltpos =
    ut_array_1d_int_eltpos ((*pTD).cellchanged, (*pTD).cellchangedqty, cell);

  if (eltpos == -1)
    net_polycomp_poly (Domain, SSet, pnf_tree, ptid_seedid, cell,
                       (*pPoly) + cell, pTD);

  return;
}
