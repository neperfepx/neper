/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_.h"

void
net_tess_opt_comp_rand_shift_seeds (double *x, struct TOPT *pTOpt, int seedqty,
                                    int dimqty, double min, double max,
                                    int *alldims, int alldimqty, gsl_rng *r)
{
  int i, j, seed, dim, id;
  int* seeds = ut_alloc_1d_int ((*pTOpt).SSet.N);
  double *tmp = ut_alloc_1d (ut_num_max ((*pTOpt).SSet.N, 4));
  double *v = ut_alloc_1d ((*pTOpt).Dim + 1);
  int *dims = ut_alloc_1d_int (4);

  // seeds
  ut_array_1d_set_rand (tmp, (*pTOpt).SSet.N, 0, 1, r);
  ut_array_1d_sort_index (tmp, (*pTOpt).SSet.N, seeds);
  ut_array_1d_int_addval (seeds, seedqty, 1, seeds);

  // change x
  for (i = 0; i < seedqty; i++)
  {
    seed = seeds[i];

    // dims
    ut_array_1d_int_choose (alldims, alldimqty, dims, dimqty, r);
    ut_space_random (r, dims, dimqty, min, max, v);

    for (j = 0; j < dimqty; j++)
    {
      dim = dims[j];
      id = (*pTOpt).seedvar_x[seed][dim];
      x[id] += v[dim];
      x[id] = ut_num_max (x[id], (*pTOpt).boundl[id]);
      x[id] = ut_num_min (x[id], (*pTOpt).boundu[id]);
    }
  }

  ut_free_1d_int (seeds);
  ut_free_1d (tmp);
  ut_free_1d (v);
  ut_free_1d_int (dims);

  return;
}

void
net_tess_opt_comp_rand_shift_crystal (double *x, struct TOPT *pTOpt,
                                      int dimqty, double min, double max,
                                      int *alldims, int alldimqty, gsl_rng *r)
{
  int j, dim;
  double *tmp = ut_alloc_1d (ut_num_max ((*pTOpt).SSet.N, 4));
  double *v = ut_alloc_1d ((*pTOpt).Dim + 1);
  int *dims = ut_alloc_1d_int (4);

  // dims
  ut_array_1d_int_choose (alldims, alldimqty, dims, dimqty, r);
  ut_space_random (r, dims, dimqty, min, max, v);

  for (j = 0; j < dimqty; j++)
  {
    dim = dims[j];
    x[dim] += v[dim];
    x[dim] = ut_num_max (x[dim], (*pTOpt).boundl[dim]);
    x[dim] = ut_num_min (x[dim], (*pTOpt).boundu[dim]);
  }

  ut_free_1d (tmp);
  ut_free_1d (v);
  ut_free_1d_int (dims);

  return;
}
