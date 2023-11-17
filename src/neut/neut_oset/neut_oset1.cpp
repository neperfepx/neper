/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_oset_.hpp"

void
neut_oset_kdtree (struct OL_SET *pOSet, struct QCLOUD *pqcloud,
                  my_kd_tree_t ** pqindex)
{
  neut_oset_kdtree_cloud (pOSet, pqcloud);
  neut_oset_kdtree_build (pqcloud, pqindex);

  return;
}

void
neut_oset_clustering (struct OL_SET OSet, struct OL_SET Grid, char *method, struct OL_SET *pOSet)
{
  int i, id;

  ut_array_1d_zero (Grid.weight, Grid.size);

  if (!method || !strcmp (method, "kdtree"))
  {
    my_kd_tree_t *nano_index = nullptr;
    nanoflann::SearchParams params;
    QCLOUD nano_cloud;

    neut_oset_kdtree (&Grid, &nano_cloud, &nano_index);

    size_t num_results = 1;
    std::vector<long unsigned int> ret_index(num_results);
    std::vector<double>    out_dist_sqr(num_results);

    for (i = 0; i < (int) OSet.size; i++)
    {
      nano_index->knnSearch (OSet.q[i], num_results, &ret_index[0], &out_dist_sqr[0]);

      id = ret_index[0] % Grid.size;

      Grid.weight[id] += OSet.weight ? OSet.weight[i] : 1;
    }

    delete nano_index;
  }

  else if (!strcmp (method, "direct"))
  {
    int j;
    double mintheta, theta;

    for (i = 0; i < (int) OSet.size; i++)
    {
      id = 0;
      mintheta = DBL_MAX;
      for (j = 0; j < (int) Grid.size; j++)
      {
        ol_q_q_disori (OSet.q[i], Grid.q[j], OSet.crysym, &theta);
        if (theta < mintheta)
        {
          mintheta = theta;
          id = j;
        }
      }

      Grid.weight[id] += OSet.weight ? OSet.weight[i] : 1;
    }
  }

  ol_set_free (pOSet);
  ut_string_string (Grid.crysym, &(*pOSet).crysym);
  for (i = 0; i < (int) Grid.size; i++)
    if (Grid.weight[i] > 0)
    {
      (*pOSet).size++;
      (*pOSet).q = ut_realloc_2d_addline ((*pOSet).q, (*pOSet).size, 4);
      ol_q_memcpy (Grid.q[i], (*pOSet).q[(*pOSet).size - 1]);
      (*pOSet).weight = ut_realloc_1d ((*pOSet).weight, (*pOSet).size);
      (*pOSet).weight[(*pOSet).size - 1] = Grid.weight[i];
    }

  if ((*pOSet).size == 0)
    abort ();

  return;
}

void
neut_oset_odf_clustering (struct OL_SET OSet, struct ODF Odf, char *method, struct OL_SET *pOSet)
{
  struct OL_SET Grid;

  neut_odf_mesh_olset (Odf, &Grid);
  ut_array_1d_zero (Grid.weight, Grid.size);

  neut_oset_clustering (OSet, Grid, method, pOSet);

  ol_set_free (&Grid);

  return;
}
