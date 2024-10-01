/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"
#include "neut_qcloud_struct.hpp"

void
neut_odf_comp_elts_all (struct OL_SET *pOSet, double *q, struct ODF *pOdf, int id)
{
  int i, j, n = ol_crysym_qty ((*pOSet).crysym);
  double sigma_theta, theta, *qs = ol_q_alloc ();

  if (!(*pOdf).odf)
    (*pOdf).odf = ut_alloc_1d ((*pOdf).odfqty);

  sigma_theta = (*pOdf).sigma;
  for (i = 0; i < (int) (*pOSet).size; i++)
  {
    if ((*pOSet).theta)
      sigma_theta = sqrt (pow ((*pOdf).sigma, 2) + pow ((*pOSet).theta[i], 2));

    for (j = 1; j <= n; j++)
    {
      ol_q_crysym ((*pOSet).q[i], (*pOSet).crysym, j, qs);
      ol_q_q_misori_rad (q, qs, &theta);
      (*pOdf).odf[id] += (*pOSet).weight[i] / sqrt (2 * M_PI * sigma_theta * sigma_theta)
        * exp (-theta * theta / (2 * sigma_theta * sigma_theta)) * (((*pOdf).sigma * (*pOdf).sigma) / (sigma_theta * sigma_theta));
    }
  }

  ol_q_free (qs);

  return;
}

void
neut_odf_comp_nodes_all (struct OL_SET *pOSet, double *q, struct ODF *pOdf, int id)
{
  int i, j, n = ol_crysym_qty ((*pOSet).crysym);
  double theta, *qs = ol_q_alloc (), sigma_theta;

  sigma_theta = (*pOdf).sigma;
  for (i = 0; i < (int) (*pOSet).size; i++)
  {
    if ((*pOSet).theta)
      sigma_theta = sqrt (pow ((*pOdf).sigma, 2) + pow ((*pOSet).theta[i], 2));

    for (j = 1; j <= n; j++)
    {
      ol_q_crysym ((*pOSet).q[i], (*pOSet).crysym, j, qs);
      ol_q_q_misori_rad (q, qs, &theta);
      (*pOdf).odfn[id] += (*pOSet).weight[i] / sqrt (2 * M_PI * sigma_theta * sigma_theta)
        * exp (-theta * theta / (2 * sigma_theta * sigma_theta)) * (((*pOdf).sigma * (*pOdf).sigma) / (sigma_theta * sigma_theta));
    }
  }

  ol_q_free (qs);

  return;
}

// input: q & id: orientation and id of the mesh elt
void
neut_odf_orifield_comp_elts_all (struct OL_SET *pOSet, double *oridata, double *q,
                                 int id, struct ODF *pOdf)
{
  int i, j, n = ol_crysym_qty ((*pOSet).crysym);
  double theta, tmp, *oridata_weight = ut_alloc_1d ((*pOSet).size), *qs = ol_q_alloc ();

  for (i = 0; i < (int) (*pOSet).size; i++)
  {
    // Computing weight (sum of gaussian kernels)
    for (j = 1; j <= n; j++)
    {
      ol_q_crysym ((*pOSet).q[i], (*pOSet).crysym, j, qs);
      ol_q_q_misori_rad (q, qs, &theta);

      tmp = 1 / sqrt (2 * M_PI * (*pOdf).sigma * (*pOdf).sigma)
        * exp (-theta * theta / (2 * (*pOdf).sigma * (*pOdf).sigma));

      oridata_weight[i] += tmp;
    }

    // Multiplying by the orientation weight
    oridata_weight[i] *= (*pOSet).weight[i];
  }

  // Computing weighted average
  // weighting is done respective to the the previously computed weights and
  // the orientation weights, if defined

  // we use (*pOdf).odf as a placeholder, as we are not actually computing an odf
  (*pOdf).odf[id] = 0;
  for (i = 0; i < (int) (*pOSet).size; i++)
    (*pOdf).odf[id] += oridata_weight[i] * oridata[i];
  (*pOdf).odf[id] /= ut_array_1d_sum (oridata_weight, (int) (*pOSet).size);

  ol_q_free (qs);
  ut_free_1d (&oridata_weight);

  return;
}

// same as neut_odf_orifield_comp_elts_all, except that we copy to odfn[id]
// input: q & id: orientation and id of the mesh elt
void
neut_odf_orifield_comp_nodes_all (struct OL_SET *pOSet, double *oridata, double *q,
                                  int id, struct ODF *pOdf)
{
  int i, j, n = ol_crysym_qty ((*pOSet).crysym);
  double theta, tmp, *oridata_weight = ut_alloc_1d ((*pOSet).size), *qs = ol_q_alloc ();

  for (i = 0; i < (int) (*pOSet).size; i++)
  {
    // Computing weight (sum of gaussian kernels)
    for (j = 1; j <= n; j++)
    {
      ol_q_crysym ((*pOSet).q[i], (*pOSet).crysym, j, qs);
      ol_q_q_misori_rad (q, qs, &theta);

      tmp = 1 / sqrt (2 * M_PI * (*pOdf).sigma * (*pOdf).sigma)
        * exp (-theta * theta / (2 * (*pOdf).sigma * (*pOdf).sigma));

      oridata_weight[i] += tmp;
    }

    // Multiplying by the orientation weight
    oridata_weight[i] *= (*pOSet).weight[i];
  }

  // Computing weighted average
  // weighting is done respective to the the previously computed weights and
  // the orientation weights, if defined

  // we use (*pOdf).odfn as a placeholder, as we are not actually computing an odfn

  (*pOdf).odfn[id] = 0;
  for (i = 0; i < (int) (*pOSet).size; i++)
    (*pOdf).odfn[id] += oridata_weight[i] * oridata[i];
  (*pOdf).odfn[id] /= ut_array_1d_sum (oridata_weight, (int) (*pOSet).size);

  ol_q_free (qs);
  ut_free_1d (&oridata_weight);

  return;
}

void
neut_odf_comp_elts_neigh (double *q, double cut_fact, QCLOUD qcloud,
                          my_kd_tree_t *nano_index, struct ODF *pOdf, int id)
{
  int i, neighqty;
  double theta, weight, cut_dist;
  double *tmp = NULL;
  double sigma_theta, theta_max;

  tmp = ut_alloc_1d (qcloud.pts.size());
  for (i = 0; i < (int) qcloud.pts.size(); i++)
    tmp[i] = weight = qcloud.pts[i].theta;
  theta_max = ut_array_1d_max (tmp, qcloud.pts.size());
  ut_free_1d (&tmp);

  cut_dist = pow (2 * sin (0.25 * cut_fact * sqrt (pow ((*pOdf).sigma, 2) + pow (theta_max, 2))), 2);

  nanoflann::SearchParams params;
  std::vector < std::pair < size_t, double > >ret_matches;
  neighqty = nano_index->radiusSearch (q, cut_dist, ret_matches, params);

  for (i = 0; i < neighqty; i++)
  {
    theta = 4 * asin (sqrt (ret_matches[i].second) * 0.5);

    weight = qcloud.pts[ret_matches[i].first].weight;

    sigma_theta = sqrt (pow ((*pOdf).sigma, 2) + pow (qcloud.pts[ret_matches[i].first].theta, 2));

    if (theta < M_PI)
      (*pOdf).odf[id] += weight / sqrt (2 * M_PI * sigma_theta * sigma_theta)
        * exp (-theta * theta / (2 * sigma_theta * sigma_theta)) * (((*pOdf).sigma * (*pOdf).sigma) / (sigma_theta * sigma_theta));
  }

  return;
}

void
neut_odf_comp_nodes_neigh (double *q, double cut_fact, QCLOUD qcloud, my_kd_tree_t *nano_index,
                           struct ODF *pOdf, int id)

{
  int i, neighqty;
  double theta, weight, cut_dist;
  double *tmp = NULL;
  double sigma_theta, theta_max;

  tmp = ut_alloc_1d (qcloud.pts.size());
  for (i = 0; i < (int) qcloud.pts.size(); i++)
    tmp[i] = weight = qcloud.pts[i].theta;
  theta_max = ut_array_1d_max (tmp, qcloud.pts.size());
  ut_free_1d (&tmp);

  cut_dist = pow (2 * sin (0.25 * cut_fact * sqrt (pow ((*pOdf).sigma, 2) + pow (theta_max, 2))), 2);

  nanoflann::SearchParams params;
  std::vector < std::pair < size_t, double > >ret_matches;
  neighqty = nano_index->radiusSearch (q, cut_dist, ret_matches, params);

  for (i = 0; i < neighqty; i++)
  {
    theta = 4 * asin (sqrt (ret_matches[i].second) * 0.5);

    weight = qcloud.pts[ret_matches[i].first].weight;

    sigma_theta = sqrt (pow ((*pOdf).sigma, 2) + pow (qcloud.pts[ret_matches[i].first].theta, 2));

    if (theta < M_PI)
      (*pOdf).odfn[id] += weight / sqrt (2 * M_PI * sigma_theta * sigma_theta)
        * exp (-theta * theta / (2 * sigma_theta * sigma_theta)) * (((*pOdf).sigma * (*pOdf).sigma) / (sigma_theta * sigma_theta));
  }

  return;
}
