/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include "ol_map_filter.h"

void
ol_map_filter_kuwahara_iter (struct OL_MAP Map, size_t size,
                             struct OL_MAP *pMap2, double *pdis)
{
  double prevdis, ratio;
  struct OL_MAP Map3 = ol_map_alloc (Map.xsize, Map.ysize, Map.stepsize,
                                     Map.crysym);

  ol_map_memcpy (Map, &Map3);

  prevdis = OL_EPS_DEG;
  do
  {
    ol_map_filter_kuwahara (Map3, size, pMap2, pdis);
    ol_map_memcpy (*pMap2, &Map3);
    ratio = fabs (prevdis - (*pdis)) / prevdis;
    prevdis = (*pdis);
  }
  while (ratio > 0.01);         /* until convergence */

  ol_map_memcpy (Map3, pMap2);
  ol_map_free (&Map3);

  return;
}

void
ol_map_filter_kuwahara (struct OL_MAP Map, size_t size,
                        struct OL_MAP *pMap2, double *pdis)
{
  size_t i, j;
  int qty, skip;
  double tmpdis;
  struct OL_MAP SubMap = ol_map_alloc (size, size, Map.stepsize,
                                       Map.crysym);


  skip = (size - 1) / 2;

  ol_map_memcpy (Map, pMap2);

  (*pdis) = 0;
  qty = 0;
  for (i = skip; i < (*pMap2).xsize - skip; i++)
    for (j = skip; j < (*pMap2).ysize - skip; j++)
      if (Map.id[i][j] == 1)    /* treating indexed orientations only */
      {
        ol_map_submap_center (Map, i, j, size, size, &SubMap);
        ol_map_filter_kuwahara_region (SubMap, (*pMap2).q[i][j], &tmpdis);
        qty++;
        (*pdis) += tmpdis;
      }
  if (qty > 0)
    (*pdis) /= qty;

  ol_map_free (&SubMap);

  return;
}

void
ol_map_filter_kuwahara_region (struct OL_MAP Map, double *q, double *pdis)
{
  int id;
  size_t i;
  struct OL_MAP SubMap;
  struct OL_SET SubSet;
  size_t subsize = (Map.xsize + 1) / 2;
  int **coo = ut_alloc_2d_int (4, 2);
  double **mean = ut_alloc_2d (4, 4);
  double *dis = ut_alloc_1d (4);
  double *qref = ol_q_alloc ();

  coo[0][0] = 0;
  coo[0][1] = 0;
  coo[1][0] = subsize - 1;
  coo[1][1] = 0;
  coo[2][0] = 0;
  coo[2][1] = subsize - 1;
  coo[3][0] = subsize - 1;
  coo[3][1] = subsize - 1;
  // reference orientation: the central orientation of the region (shared
  // by the 4 subregions)
  ol_q_memcpy (Map.q[subsize - 1][subsize - 1], qref);

  for (i = 0; i < 4; i++)
  {
    SubMap = ol_map_alloc (subsize, subsize, Map.stepsize, Map.crysym);
    ol_map_submap (Map, coo[i][0], coo[i][1], subsize, subsize, &SubMap);
    ol_map_set (SubMap, &SubSet);
    ol_map_free (&SubMap);
    if (SubSet.size > 0)
    {
      ol_set_mean_ref (SubSet, qref, 1, mean[i]);
      ol_set_disori (SubSet, mean[i], &(dis[i]));
    }
    else                        // no defined orientation: subregion not considered
    {
      ol_q_memcpy (qref, mean[i]);
      dis[i] = DBL_MAX;
    }
    ol_set_free (&SubSet);
  }

  id = ut_array_1d_min_index (dis, 4);
  ol_q_memcpy (mean[id], q);
  (*pdis) = dis[id];

  ut_free_2d_int (&coo, 4);
  ut_free_2d (&mean, 4);
  ut_free_1d (&dis);
  ol_q_free (&qref);

  return;
}
