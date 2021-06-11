/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_.h"
#include"neut_nanoflann_struct.hpp"

extern void net_polycomp_kdtree (struct SEEDSET SSet, NFCLOUD * pnf_cloud,
                                 NFTREE ** pnf_tree, int **pptid_seedid,
                                 int **pseedid_ptid, struct TDYN *pTD);
extern void net_polycomp_cells (struct POLY Domain, struct SEEDSET SSet,
                                NFTREE ** pnf_tree, int *ptid_seedid,
                                int *updatedseeds, int updatedcellqty,
                                struct TDYN *pTD, struct POLY **pPoly);

/* net_polycomp builds the set of Voronoi polyhedra from the set
 * of seed and the initial domain.
 */
void
net_polycomp (struct POLY Domain, struct SEEDSET SSet, NFCLOUD * pnf_cloud,
              NFTREE ** pnf_tree, int **pptid_seedid, int **pseedid_ptid,
              struct POLY **pPoly, int *updatedseeds_in,
              int updatedseedqty_in, struct TDYN *pTD)
{
  int updatedseedqty, *updatedseeds = NULL;
  struct timeval time;

  // Starting ----------------------------------------------------------
  gettimeofday (&time, NULL);

  (*pTD).iter++;

  // If first increment, initializing TD -------------------------------

  net_polycomp_inittdyn (Domain, SSet, pTD, pPoly);

  // Computing KD tree -------------------------------------------------

  net_polycomp_kdtree (SSet, pnf_cloud, pnf_tree, pptid_seedid,
                       pseedid_ptid, pTD);

  // Computing shifts --------------------------------------------------

  net_polycomp_shift (SSet, pTD);

  // Recording updated seeds -------------------------------------------

  net_polycomp_updatedseeds (SSet, pTD, updatedseeds_in, updatedseedqty_in,
                             &updatedseeds, &updatedseedqty);

  // Calculating cells -------------------------------------------------

  net_polycomp_cells (Domain, SSet, pnf_tree, *pptid_seedid, updatedseeds,
                      updatedseedqty, pTD, pPoly);

  // Closing ----------------------------------------------------------

  (*pTD).cell_total_dur = ut_time_subtract (&time, NULL);
  neut_tdyn_init_otherdur (pTD);

  ut_free_1d_int (&updatedseeds);

  return;
}
