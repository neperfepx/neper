/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"
#include "neut_qcloud_struct.hpp"

void
neut_odf_comp_elts_all (struct OL_SET *pOSet, double *q, struct ODF *pOdf, int id)
{
  int i, j, n = ol_crysym_qty ((*pOSet).crysym);
  double theta, *qs = ol_q_alloc ();

  for (i = 0; i < (int) (*pOSet).size; i++)
    for (j = 1; j <= n; j++)
    {
      ol_q_crysym ((*pOSet).q[i], (*pOSet).crysym, j, qs);
      ol_q_q_misori_rad (q, qs, &theta);
      (*pOdf).odf[id] += (*pOSet).weight[i] / sqrt (2 * M_PI * (*pOdf).sigma * (*pOdf).sigma)
        * exp (-theta * theta / (2 * (*pOdf).sigma * (*pOdf).sigma));
    }

  ol_q_free (qs);

  return;
}

void
neut_odf_comp_elts_neigh (double *q, double cut_fact, QCLOUD qcloud,
                          my_kd_tree_t *nano_index, struct ODF *pOdf, int id)
{
  int i, neighqty;
  double theta, weight, cut_dist = pow (2 * sin (0.25 * cut_fact * (*pOdf).sigma), 2);

  nanoflann::SearchParams params;
  std::vector < std::pair < size_t, double > >ret_matches;
  neighqty = nano_index->radiusSearch (q, cut_dist, ret_matches, params);

  for (i = 0; i < neighqty; i++)
  {
    theta = 4 * asin (sqrt (ret_matches[i].second) * 0.5);

    weight = qcloud.pts[ret_matches[i].first].weight;

    if (theta < M_PI)
      (*pOdf).odf[id] += weight / sqrt (2 * M_PI * (*pOdf).sigma * (*pOdf).sigma)
        * exp (-theta * theta / (2 * (*pOdf).sigma * (*pOdf).sigma));
  }

  return;
}

void
neut_odf_comp_nodes_all (struct OL_SET *pOSet, double *q, struct ODF *pOdf, int id)
{
  int i, j, n = ol_crysym_qty ((*pOSet).crysym);
  double theta, *qs = ol_q_alloc ();

  for (i = 0; i < (int) (*pOSet).size; i++)
    for (j = 1; j <= n; j++)
    {
      ol_q_crysym ((*pOSet).q[i], (*pOSet).crysym, j, qs);
      ol_q_q_misori_rad (q, qs, &theta);
      (*pOdf).odfn[id] += (*pOSet).weight[i] / sqrt (2 * M_PI * (*pOdf).sigma * (*pOdf).sigma)
        * exp (-theta * theta / (2 * (*pOdf).sigma * (*pOdf).sigma));
    }

  ol_q_free (qs);

  return;
}

void
neut_odf_comp_nodes_neigh (double *q, double cut_fact, QCLOUD qcloud, my_kd_tree_t *nano_index,
                           struct ODF *pOdf, int id)

{
  int i, neighqty;
  double theta, weight, cut_dist = pow (2 * sin (0.25 * cut_fact * (*pOdf).sigma), 2);

  nanoflann::SearchParams params;
  std::vector < std::pair < size_t, double > >ret_matches;
  neighqty = nano_index->radiusSearch (q, cut_dist, ret_matches, params);

  for (i = 0; i < neighqty; i++)
  {
    theta = 4 * asin (sqrt (ret_matches[i].second) * 0.5);
    weight = qcloud.pts[ret_matches[i].first].weight;

    if (theta < M_PI)
      (*pOdf).odfn[id] += weight / sqrt (2 * M_PI * (*pOdf).sigma * (*pOdf).sigma)
        * exp (-theta * theta / (2 * (*pOdf).sigma * (*pOdf).sigma));
  }

  return;
}
