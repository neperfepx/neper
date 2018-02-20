/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_.h"
#include<ANN/ANN.h>

extern void net_polycomp_poly (struct POLY Domain, struct SEEDSET SSet,
				   ANNkd_tree * kdTree, int PolyId,
				   struct POLY *pPoly, struct TDYN *pTD);
extern void net_polycomp_kdtree (struct SEEDSET SSet, ANNkd_tree **
				 pkdTree, struct TDYN *pTD);
extern void net_polycomp_cells (struct POLY Domain, struct SEEDSET SSet,
				ANNkd_tree *pkdTree, int *updatedseeds,
				int updatedcellqty, struct TDYN *pTD,
				struct POLY **pPoly);

/* net_polycomp builds the set of Voronoi polyhedra from the set
 * of seed and the initial domain.
 */
void
net_polycomp (struct POLY Domain, struct SEEDSET SSet,
	      ANNkd_tree **pkdTree, struct POLY **pPoly,
	      int *updatedseeds_in, int updatedseedqty_in,
	      struct TDYN *pTD)
{
  int updatedseedqty, *updatedseeds = NULL;
  struct timeval time;

  // Starting ----------------------------------------------------------
  gettimeofday (&time, NULL);

  (*pTD).iter++;

  // If first increment, initializing TD -------------------------------

  net_polycomp_inittdyn (Domain, SSet, pTD, pPoly);

  // Computing KD tree -------------------------------------------------

  if (!strcmp ((*pTD).algoneigh, "ann"))
    net_polycomp_kdtree (SSet, pkdTree, pTD);

  // Computing shifts --------------------------------------------------

  net_polycomp_shift (SSet, pTD);

  // Recording updated seeds -------------------------------------------

  net_polycomp_updatedseeds (SSet, pTD, updatedseeds_in, updatedseedqty_in,
			     &updatedseeds, &updatedseedqty);

  // Calculating cells -------------------------------------------------

  net_polycomp_cells (Domain, SSet, *pkdTree, updatedseeds, updatedseedqty, pTD, pPoly);

  // Closing ----------------------------------------------------------

  (*pTD).cell_total_dur = ut_time_subtract (&time, NULL);
  neut_tdyn_init_otherdur (pTD);

  ut_free_1d_int (updatedseeds);

  return;
}
