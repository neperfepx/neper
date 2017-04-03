/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_.h"
#include<ANN/ANN.h>

extern void net_polycomp_poly (struct POLY Domain, struct SEEDSET SSet,
				   ANNkd_tree * kdTree, int PolyId,
				   struct POLY *pPoly, struct TDYN *pTD);

void
net_polycomp_cells_inittdyn (struct TDYN *pTD)
{
  ut_free_1d_int_ (&(*pTD).cellchanged);
  (*pTD).cellchangedqty = 0;
  ut_free_1d_int_ (&(*pTD).changedneighs);
  (*pTD).changedneighqty = 0;

  return;
}

void
net_polycomp_cells_updatecell (struct POLY Domain, struct SEEDSET SSet,
			       ANNkd_tree *pkdTree, int cell,
			       struct POLY **pPoly, struct TDYN *pTD)
{
  if (ut_array_1d_int_eltpos ((*pTD).cellchanged, (*pTD).cellchangedqty,
			      cell) == -1)
    net_polycomp_poly (Domain, SSet, pkdTree, cell, (*pPoly) + cell, pTD);

  return;
}
