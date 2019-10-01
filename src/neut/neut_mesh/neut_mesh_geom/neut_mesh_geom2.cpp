/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_geom_.h"
#include"neut/neut_structs/neut_nanoflann_struct.hpp"

int
neut_mesh_elset_points_closestelts_search (struct MESH Mesh, int elset,
                                           double *coo, NFTREE *nf_tree,
                                           int qty, int *pelt)
{
  qty = ut_num_min (qty, Mesh.Elsets[elset][0]);

  std::vector < size_t > ret_index (qty);
  std::vector < double >out_dist_sqr (qty);
  nanoflann::KNNResultSet < double >resultSet (qty);
  resultSet.init (&ret_index[0], &out_dist_sqr[0]);

  nf_tree->findNeighbors (resultSet, coo, nanoflann::SearchParams (INT_MAX));
  (*pelt) = Mesh.Elsets[elset][ret_index[0] + 1];

  return 0;
}
