/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_polycomp_poly_.h"
#include"neut_struct_nfcloud.hpp"

void
net_tess3d_polycomp_poly_neigh (struct SEEDSET SSet, int id, int neighqty, NFTREE ** pnf_tree,
                                int *ptid_seedid, struct TDYN *pTD)
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
  (**pnf_tree).findNeighbors (resultSet, &((*pTD).neighrefcoo[id][0]),
                              nanoflann::SearchParams (INT_MAX));

  for (i = 1; i <= (*pTD).neighqty[id]; i++)
    (*pTD).neighlist[id][i] = ptid_seedid[ret_index[i - 1]];

  // squared distances, so taking sqrt ()
  for (i = 1; i <= (*pTD).neighqty[id]; i++)
    (*pTD).neighdist[id][i] = sqrt (out_dist_sqr[i - 1]);

  return;
}

// net_tess3d_polycomp_poly_test returns 1 if the bissecting plane between
// seed id and neighbor nid, of weight nweight, can cut the polyhedron, i.e.
// is closer to seed id than the furthest vertex of the polyhedron, of
// distance maxdist (see net_tess3d_polycomp_poly_maxdist); 0 else.
int
net_tess3d_polycomp_poly_test (struct SEEDSET SSet, int id, int nid, double nweight, double maxdist)
{
  double neidist, seeddist;

  seeddist = ut_space_dist (SSet.SeedCoo[id], SSet.SeedCoo[nid]);

  neidist = .5 * (seeddist - (nweight - SSet.SeedWeight[id]) / seeddist);

  return neidist < maxdist;
}

// net_tess3d_polycomp_poly_maxdist returns the distance between seed coo and
// the furthest vertex of the polyhedron.
double
net_tess3d_polycomp_poly_maxdist (struct POLY Poly, double *coo)
{
  int i;
  double dist, maxdist = 0;

  for (i = 1; i <= Poly.VerQty; i++)
  {
    dist = ut_space_dist (coo, Poly.VerCoo[i]);
    if (dist > maxdist)
      maxdist = dist;
  }

  return maxdist;
}

// net_tess3d_polycomp_poly_cut cuts the polyhedron by a plane (pPrim = NULL) or
// a primitive (plane = NULL).  The normal of the plane must point to the
// removed side, and the polyhedron must then be planar.  The polyhedron is
// kept on the positive side of the primitive (see neut_prim_point_side), and
// the new vertices are then computed as the intersections between the
// polyhedron edges and the primitive, so that the polyhedron faces do not need
// to be planar (see net_tess3d_polycomp_poly_cut_mod).  The removed vertices
// and faces are removed from the polyhedron (the order of the others is
// kept).
int
net_tess3d_polycomp_poly_cut (int NeiId, double *plane, struct PRIM *pPrim, struct POLY *pPoly)
{
  int status = 0;
  double *primplane = NULL;

  // a half-space primitive is also defined by a plane, whose normal points
  // to the removed side
  if (pPrim && !plane
      && (!strcmp ((*pPrim).Type, "hspace") || !strcmp ((*pPrim).Type, "hspacei")))
  {
    primplane = ut_alloc_1d (4);
    ut_array_1d_memcpy ((*pPrim).Eq, 4, primplane);
    ut_array_1d_scale (primplane, 4, -1);
    plane = primplane;
  }

  /* BadVer contains the number of the polyhedron vertices situated      */
  /* in the bad side of the cutting plane or primitive.                  */
  /* BadVer[0] is the quantity of vertices situated in the bad side.     */
  /* BadVer[i] are the number these vertices.                            */
  int *BadVer = NULL;

  /* net_tess3d_polycomp_poly_cut_badver searches the vertices that are on   */
  /* the bad side of the cutting plane or primitive, which are written   */
  /* in BadVer.                                                          */
  net_tess3d_polycomp_poly_cut_badver (plane, pPrim, *pPoly, &BadVer);

  /* If there are vertices to suppress, the polyhedron is modified       */
  if (BadVer[0] != 0)
  {
    net_tess3d_polycomp_poly_cut_mod (NeiId, plane, pPrim, pPoly, BadVer);
    status = 1;
  }

  ut_free_1d_int (&BadVer);
  ut_free_1d (&primplane);

  return status;
}

// net_tess3d_polycomp_poly_addchangedneigh adds a cell to TD.changedneighs, if not
// already in.
void
net_tess3d_polycomp_poly_addchangedneigh (struct TDYN *pTD, int cell)
{
  if (!(*pTD).changedneighflag[cell])
  {
    (*pTD).changedneighflag[cell] = 1;
    ut_array_1d_int_list_addval_nocheck (&(*pTD).changedneighs,
                                         &(*pTD).changedneighqty, cell);
  }

  return;
}
