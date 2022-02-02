/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_poly_.h"
#include"neut_nfcloud_struct.hpp"

extern void net_polycomp_seed_tdyn (struct SEEDSET SSet, int id, int,
                                    NFTREE ** pnf_tree, int *ptid_seedid,
                                    struct TDYN *pTD);

/* net_polycomp_poly searches out the polyhedron associated
* to the considered seed.
*/
void
net_polycomp_poly (struct POLY Domain, struct SEEDSET SSet,
                   NFTREE ** pnf_tree, int *ptid_seedid, int id,
                   struct POLY *pPoly, struct TDYN *pTD)
{
  int i, j, cutqty;
  struct POLYMOD Polymod;
  double wmax = 0;
  struct timeval time;
  int qty1 = 0, *tmp1 = NULL;
  int qty2 = 0, *tmp2 = NULL;

  neut_poly_neighpolys (*pPoly, SSet, &tmp1, &qty1);

  if ((*pTD).iter > 1)
    neut_poly_free (pPoly);

  // FIXME
  // in input, *pneighqty is taken as the average of the number of
  // neighbors used in previous computations. If 0, a default value of
  // 100 is taken instead. In output, the number of neighbors used is
  // returned.

  wmax = ut_array_1d_max (SSet.SeedWeight + 1, SSet.N);

  neut_polymod_set_zero (&Polymod);

  neut_poly_polymod (Domain, &Polymod);

  cutqty = 0;

  // Standard Voronoi tessellation
  if (!SSet.LamEq)
  {
    gettimeofday (&time, NULL);
    net_polycomp_seed_tdyn (SSet, id, 100, pnf_tree, ptid_seedid, pTD);
#pragma omp atomic
    (*pTD).cell_neigh_dur += ut_time_subtract (&time, NULL);

    for (i = 2; i <= SSet.Nall; i++)
    {
      // The number of recorded neighbors is exceeded.  Recomputing a
      // larger number of neighbors.
      if (i > (*pTD).neighqty[id])
      {
        gettimeofday (&time, NULL);
        net_polycomp_seed_tdyn (SSet, id, 2 * (*pTD).neighqty[id], pnf_tree,
                                ptid_seedid, pTD);
        (*pTD).cell_neigh_dur += ut_time_subtract (&time, NULL);
      }

      gettimeofday (&time, NULL);
      if (CurrentPolyTest (Polymod, SSet, id, i, *pTD, 0) == 1)
        cutqty += NewPolyhedron (SSet, id, (*pTD).neighlist[id][i], &Polymod);
      else if (CurrentPolyTest (Polymod, SSet, id, i, *pTD, wmax) == 0)
        break;
      (*pTD).cell_cell_dur += ut_time_subtract (&time, NULL);
    }

    net_polymod_poly (Polymod, pPoly);
  }

  // Lamellae tessellation
  else
  {
    if (id > 1)
      NewPolyhedron_plane (id - 1, 1, SSet.LamEq[id - 1], &Polymod);

    if (id < SSet.N)
      NewPolyhedron_plane (id + 1, -1, SSet.LamEq[id], &Polymod);

    net_polymod_poly (Polymod, pPoly);
    net_poly_centroid (*pPoly, SSet.SeedCoo[id]);
  }

  neut_poly_neighpolys (*pPoly, SSet, &tmp2, &qty2);
  for (j = 0; j < qty1; j++)
    if (ut_array_1d_int_eltpos (tmp2, qty2, tmp1[j]) == -1)
#pragma omp critical
      ut_array_1d_int_list_addval (&(*pTD).changedneighs,
                                   &(*pTD).changedneighqty, tmp1[j]);
  for (j = 0; j < qty2; j++)
    if (ut_array_1d_int_eltpos (tmp1, qty1, tmp2[j]) == -1)
#pragma omp critical
      ut_array_1d_int_list_addval (&(*pTD).changedneighs,
                                   &(*pTD).changedneighqty, tmp2[j]);

#pragma omp critical
  ut_array_1d_int_list_addval (&(*pTD).cellchanged, &(*pTD).cellchangedqty,
                               id);

  if (cutqty == 0)
#pragma omp critical
    ut_array_1d_int_list_addval (&(*pTD).domcells, &(*pTD).domcellqty, id);
  else
#pragma omp critical
    ut_array_1d_int_list_rmval (&(*pTD).domcells, &(*pTD).domcellqty, id);

  if (neut_poly_isvoid (*pPoly))
#pragma omp critical
    ut_array_1d_int_list_addval (&(*pTD).cellvoid, &(*pTD).cellvoidqty, id);
  else
#pragma omp critical
    ut_array_1d_int_list_rmval (&(*pTD).cellvoid, &(*pTD).cellvoidqty, id);

  neut_polymod_free (&Polymod);

  ut_free_1d_int (&tmp1);
  ut_free_1d_int (&tmp2);

  return;
}
