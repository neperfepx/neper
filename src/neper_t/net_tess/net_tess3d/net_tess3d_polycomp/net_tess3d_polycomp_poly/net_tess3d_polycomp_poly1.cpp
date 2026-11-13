/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_polycomp_poly_.h"
#include"neut_struct_nfcloud.hpp"

extern void net_tess3d_polycomp_poly_neigh (struct SEEDSET SSet, int id, int neighqty,
                                            NFTREE ** pnf_tree, int *ptid_seedid, struct TDYN *pTD);

/* net_tess3d_polycomp_poly searches out the polyhedron associated
* to the considered seed.
*/
void
net_tess3d_polycomp_poly (struct POLY Domain, struct SEEDSET SSet, NFTREE ** pnf_tree,
                          int *ptid_seedid, int id, double wcut, int *heavyseeds,
                          int heavyseedqty, struct POLY *pPoly, struct TDYN *pTD)
{
  int i, j, k, nid, cutqty, changed;
  double plane[4], maxdist, wstop;
  int pseudodim;
  double pseudosize;
  struct timeval time;
  int qty1 = 0, *tmp1 = NULL;
  int qty2 = 0, *tmp2 = NULL;

  neut_poly_neighpolys (*pPoly, SSet, &tmp1, &qty1);

  if ((*pTD).iter > 1)
    neut_poly_free (pPoly);

  // the polyhedron is initialized to the domain, and then cut (its pseudo-
  // dimension is kept)
  pseudodim = (*pPoly).PseudoDim;
  pseudosize = (*pPoly).PseudoSize;
  neut_poly_poly (Domain, pPoly);
  (*pPoly).PseudoDim = pseudodim;
  (*pPoly).PseudoSize = pseudosize;

  cutqty = 0;

  // Standard Voronoi tessellation
  if (!SSet.LamEq)
  {
    gettimeofday (&time, NULL);
    net_tess3d_polycomp_poly_neigh (SSet, id, 100, pnf_tree, ptid_seedid, pTD);
#pragma omp atomic
    (*pTD).cell_neigh_dur += ut_time_subtract (&time, NULL);

    maxdist = net_tess3d_polycomp_poly_maxdist (*pPoly, SSet.SeedCoo[id]);
    wstop = ut_num_max (wcut, SSet.SeedWeight[id]);

    for (i = 2; i <= SSet.Nall; i++)
    {
      // The number of recorded neighbors is exceeded.  Recomputing a
      // larger number of neighbors.
      if (i > (*pTD).neighqty[id])
      {
        gettimeofday (&time, NULL);
        net_tess3d_polycomp_poly_neigh (SSet, id, 2 * (*pTD).neighqty[id], pnf_tree, ptid_seedid,
                                        pTD);
        (*pTD).cell_neigh_dur += ut_time_subtract (&time, NULL);
      }

      gettimeofday (&time, NULL);
      nid = (*pTD).neighlist[id][i];
      if (net_tess3d_polycomp_poly_test (SSet, id, nid, SSet.SeedWeight[nid], maxdist))
      {
        // cutting by the bissecting plane between the seed and the neighbor
        ut_space_points_bisplane_power (SSet.SeedCoo[id], SSet.SeedCoo[nid],
                                        SSet.SeedWeight[id], SSet.SeedWeight[nid], plane);
        if (net_tess3d_polycomp_poly_cut (nid, plane, NULL, pPoly))
        {
          cutqty++;
          maxdist = net_tess3d_polycomp_poly_maxdist (*pPoly, SSet.SeedCoo[id]);
        }
      }
      // no further neighbor can cut the polyhedron, except the heavy seeds
      // (wstop: maximum weight of the other seeds, wcut, but not lower than
      // the weight of the seed, for the test to increase with the distance)
      else if (!net_tess3d_polycomp_poly_test (SSet, id, nid,
                                               wstop == 0 ? SSet.SeedWeight[nid] : wstop, maxdist))
        break;
      (*pTD).cell_cell_dur += ut_time_subtract (&time, NULL);
    }

    // heavy seeds that were not reached by the neighbor search (i.e., that
    // are not in neighlist[2...i-1])
    for (j = 0; j < heavyseedqty; j++)
    {
      nid = heavyseeds[j];
      if (nid == id)
        continue;

      for (k = 2; k < i && k <= (*pTD).neighqty[id]; k++)
        if ((*pTD).neighlist[id][k] == nid)
          break;
      if (k < i && k <= (*pTD).neighqty[id])
        continue;

      if (net_tess3d_polycomp_poly_test (SSet, id, nid, SSet.SeedWeight[nid], maxdist))
      {
        ut_space_points_bisplane_power (SSet.SeedCoo[id], SSet.SeedCoo[nid],
                                        SSet.SeedWeight[id], SSet.SeedWeight[nid], plane);
        if (net_tess3d_polycomp_poly_cut (nid, plane, NULL, pPoly))
        {
          cutqty++;
          maxdist = net_tess3d_polycomp_poly_maxdist (*pPoly, SSet.SeedCoo[id]);
        }
      }
    }

    neut_poly_fixverorder (pPoly);
  }

  // Lamellae tessellation
  else
  {
    // plane normals pointing to the removed side
    if (id > 1)
    {
      ut_array_1d_memcpy (SSet.LamEq[id - 1], 4, plane);
      ut_array_1d_scale (plane, 4, -1);
      net_tess3d_polycomp_poly_cut (id - 1, plane, NULL, pPoly);
    }

    if (id < SSet.N)
      net_tess3d_polycomp_poly_cut (id + 1, SSet.LamEq[id], NULL, pPoly);

    neut_poly_fixverorder (pPoly);
    net_poly_centroid (*pPoly, SSet.SeedCoo[id]);
  }

  neut_poly_neighpolys (*pPoly, SSet, &tmp2, &qty2);

  // Recording changes in TD (in a single critical section, to limit
  // synchronization between threads)
#pragma omp critical
  {
    for (j = 0; j < qty1; j++)
      if (ut_array_1d_int_eltpos (tmp2, qty2, tmp1[j]) == -1)
        net_tess3d_polycomp_poly_addchangedneigh (pTD, tmp1[j]);
    for (j = 0; j < qty2; j++)
      if (ut_array_1d_int_eltpos (tmp1, qty1, tmp2[j]) == -1)
        net_tess3d_polycomp_poly_addchangedneigh (pTD, tmp2[j]);

#pragma omp atomic read
    changed = (*pTD).cellchangedflag[id];

    if (!changed)
    {
#pragma omp atomic write
      (*pTD).cellchangedflag[id] = 1;
      ut_array_1d_int_list_addval_nocheck (&(*pTD).cellchanged, &(*pTD).cellchangedqty, id);
    }

    if (cutqty == 0)
      ut_array_1d_int_list_addval (&(*pTD).domcells, &(*pTD).domcellqty, id);
    else
      ut_array_1d_int_list_rmval (&(*pTD).domcells, &(*pTD).domcellqty, id);

    if (neut_poly_isvoid (*pPoly))
      ut_array_1d_int_list_addval (&(*pTD).cellvoid, &(*pTD).cellvoidqty, id);
    else
      ut_array_1d_int_list_rmval (&(*pTD).cellvoid, &(*pTD).cellvoidqty, id);
  }

  ut_free_1d_int (&tmp1);
  ut_free_1d_int (&tmp2);

  return;
}
