/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_polycomp_.h"
#include"neut_struct_nfcloud.hpp"

extern void net_tess3d_polycomp_kdtree (struct SEEDSET SSet, NFCLOUD * pnf_cloud,
                                        NFTREE ** pnf_tree, int **pptid_seedid, int **pseedid_ptid,
                                        struct TDYN *pTD);
extern void net_tess3d_polycomp_cells (struct POLY Domain, struct SEEDSET SSet, NFTREE ** pnf_tree,
                                       int *ptid_seedid, int *updatedseeds, int updatedcellqty,
                                       struct TDYN *pTD, struct POLY **pPoly);

/* net_tess3d_polycomp builds the set of Voronoi polyhedra from the set
 * of seed and the initial domain.
 */
void
net_tess3d_polycomp (struct POLY Domain, struct SEEDSET SSet, NFCLOUD * pnf_cloud,
                     NFTREE ** pnf_tree, int **pptid_seedid, int **pseedid_ptid,
                     struct POLY **pPoly, int *updatedseeds_in, int updatedseedqty_in,
                     struct TDYN *pTD)
{
  int updatedseedqty, *updatedseeds = NULL;
  struct timeval time;

  // Starting ----------------------------------------------------------
  gettimeofday (&time, NULL);

  // updatedseeds_in can be (*pTD).seedchanged, which is reallocated by
  // net_tess3d_polycomp_inittdyn
  int *updatedseeds_in_cpy = NULL;
  if (updatedseedqty_in > 0)
  {
    updatedseeds_in_cpy = ut_alloc_1d_int (updatedseedqty_in);
    ut_array_1d_int_memcpy (updatedseeds_in, updatedseedqty_in, updatedseeds_in_cpy);
  }
  updatedseeds_in = updatedseeds_in_cpy;

  (*pTD).iter++;

  // If first increment, initializing TD -------------------------------

  net_tess3d_polycomp_inittdyn (Domain, SSet, pTD, pPoly);

  // Computing KD tree -------------------------------------------------

  net_tess3d_polycomp_kdtree (SSet, pnf_cloud, pnf_tree, pptid_seedid, pseedid_ptid, pTD);

  // Computing shifts --------------------------------------------------

  net_tess3d_polycomp_shift (SSet, pTD);

  // Recording updated seeds -------------------------------------------

  net_tess3d_polycomp_updatedseeds (SSet, pTD, updatedseeds_in, updatedseedqty_in, &updatedseeds,
                                    &updatedseedqty);

  // Calculating cells -------------------------------------------------

  net_tess3d_polycomp_cells (Domain, SSet, pnf_tree, *pptid_seedid, updatedseeds, updatedseedqty,
                             pTD, pPoly);

  // Closing ----------------------------------------------------------

  (*pTD).cell_total_dur = ut_time_subtract (&time, NULL);
  neut_tdyn_init_otherdur (pTD);

  ut_free_1d_int (&updatedseeds);
  ut_free_1d_int (&updatedseeds_in_cpy);

  return;
}
