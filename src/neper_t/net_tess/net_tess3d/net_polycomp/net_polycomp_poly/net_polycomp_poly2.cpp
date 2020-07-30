/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp_poly_.h"
#include"neut/neut_structs/neut_nanoflann_struct.hpp"

void
net_polycomp_seed_tdyn (struct SEEDSET SSet, int id, int neighqty,
                        NFTREE ** pnf_tree, int *ptid_seedid,
                        struct TDYN *pTD)
{
  int i;

  (*pTD).neighqty[id] = ut_num_min_int (neighqty, SSet.Nall);

  (*pTD).neighlist[id] =
    ut_realloc_1d_int ((*pTD).neighlist[id], (*pTD).neighqty[id] + 1);
  (*pTD).neighdist[id] =
    ut_realloc_1d ((*pTD).neighdist[id], (*pTD).neighqty[id] + 1);
  ut_array_1d_memcpy (SSet.SeedCoo[id], 3, (*pTD).neighrefcoo[id]);
  (*pTD).neighrefw[id] = SSet.SeedWeight[id];
  (*pTD).shift[id] = 0;

  std::vector < size_t > ret_index ((*pTD).neighqty[id]);
  std::vector < double >out_dist_sqr ((*pTD).neighqty[id]);
  nanoflann::KNNResultSet < double >resultSet ((*pTD).neighqty[id]);
  resultSet.init (&ret_index[0], &out_dist_sqr[0]);
  (*pnf_tree)->findNeighbors (resultSet, &((*pTD).neighrefcoo[id][0]),
                              nanoflann::SearchParams (INT_MAX));

  for (i = 1; i <= (*pTD).neighqty[id]; i++)
    (*pTD).neighlist[id][i] = ptid_seedid[ret_index[i - 1]];

  // squared distances, so taking sqrt ()
  for (i = 1; i <= (*pTD).neighqty[id]; i++)
    (*pTD).neighdist[id][i] = sqrt (out_dist_sqr[i - 1]);

  return;
}

int
CurrentPolyTest (struct POLYMOD Polymod, struct SEEDSET SSet, int id,
                 int neighpos, struct TDYN TD, double wmax)
{
  int i, nid;
  double dist, neidist, seeddist, nweight;

  nid = TD.neighlist[id][neighpos];

  nweight = (wmax == 0) ? SSet.SeedWeight[nid] : wmax;

  seeddist = ut_space_dist (SSet.SeedCoo[id], SSet.SeedCoo[nid]);
  // seeddist -= 2 * TD.shiftmax;

  neidist =
    .5 * (seeddist -
          (pow (nweight, 2) - pow (SSet.SeedWeight[id], 2)) / seeddist);

  // max distance between the poly seed and its further vertex.
  dist = 0;
  for (i = 1; i <= Polymod.VerQty; i++)
    if (Polymod.VerUse[i] == 1)
    {
      dist = ut_space_dist (SSet.SeedCoo[id], Polymod.VerCoo[i]);

      if (neidist < dist)
        return 1;
    }

  return 0;
}

/* NewPolyhedron modifies the current polyhedron to take into
 * account neighbor Nei.
 */
int
NewPolyhedron (struct SEEDSET SSet, int PolyId, int NeiId,
               struct POLYMOD *pPolymod)
{
  int status = 0;

  /* plane contains the parameters of the bissecting plane of segment    */
  /* which lies the center to the considered neighbor, i.e. the new face. */
  double *plane = ut_alloc_1d (4);

  /* BadVer contains the number of the polyhedron vertices situated      */
  /* in the bad side of the bissecting plane.                            */
  /* BadVer[0] is the quantity of vertices situated in the bad side.     */
  /* BadVer[i] are the number these vertices.                            */
  int *BadVer = NULL;

  /* A new neighbor will be considered. The first step is to search out
     the bissecting plane between the center and the neighbor.
     Then, we look at the polyhedron vertices which are on the "bad"
     hand side of the plane -that of the neighbor-, i.e. vertices
     that are nearer from the neighbor than from the center. If they
     exist, the polyhedron must be modified.                             */

  /* ut_space_points_bisplane_power calculates the equation of the bissecting plane of segment */
  /* which lies the center to the considered neighbor.                  */
  ut_space_points_bisplane_power (SSet.SeedCoo[PolyId], SSet.SeedCoo[NeiId],
                                  SSet.SeedWeight[PolyId],
                                  SSet.SeedWeight[NeiId], plane);

  /* BadVerDet searches the vertices that are on the bad side of the     */
  /* bissecting plane -the bad vertices-, which are written in BadVer.   */
  BadVerDet (plane, -1, *pPolymod, &BadVer);

  /* If there are vertices to suppress, the polyhedron is modified       */
  if (BadVer[0] != 0)
  {
    /* PolyhedronModification modifies the polyhedron to take into account
     * the new neighbor.
     */
    PolyhedronModification (NeiId, plane, pPolymod, BadVer);
    status = 1;
  }

  /* The memory of the temporary arrays plane and vertices is unallocated. */
  ut_free_1d (&plane);
  ut_free_1d_int (&BadVer);

  return status;
}

// NewPolyhedron_plane is a variant of function NewPolyhedron, which
// takes the cutting plane and the CenterSide as argument.
void
NewPolyhedron_plane (int NeiId, int CenterSide, double *plane,
                     struct POLYMOD *pPolymod)
{
  int *BadVer = NULL;

  BadVerDet (plane, CenterSide, *pPolymod, &BadVer);

  if (BadVer[0] != 0)
    PolyhedronModification (NeiId, plane, pPolymod, BadVer);

  ut_free_1d_int (&BadVer);

  return;
}
