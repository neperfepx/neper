/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_uniform_.h"
using namespace std;

void
net_ori_uniform_opt_forces_ser (struct OL_SET *pOSet, double **f, double *E)
{
  unsigned int i, j, k;

#pragma omp parallel for
  for (i = 0; i < (*pOSet).size; i++)
    E[i] = 0;

#pragma omp parallel for schedule(dynamic) private (j,k)
  for (i = 0; i < (*pOSet).size; i++)
    for (j = i + 1; j < (*pOSet).size; j++)
    {
      double Eij, *fij = ut_alloc_1d (3);
      net_ori_uniform_opt_forces_comp (pOSet, i, j, fij, &Eij);

      Eij *= 0.5;
#pragma omp atomic
      E[i] += Eij;
#pragma omp atomic
      E[j] += Eij;

      for (k = 0; k < 3; k++)
#pragma omp atomic
        f[i][k] += fij[k];

      for (k = 0; k < 3; k++)
#pragma omp atomic
        f[j][k] -= fij[k];
      ut_free_1d (fij);
    }

  return;
}

void
net_ori_uniform_opt_forces_ser_neigh (struct OL_SET *pOSet,
                              double **f, double *E, struct OOPT *pOOpt,
                              struct QCLOUD *pqcloud, my_kd_tree_t *qindex)
{
  unsigned int i, j, k, neighqty;

#pragma omp parallel for
  for (i = 0; i < (*pOSet).size; i++)
    E[i] = 0;

  (*pOOpt).avneighqty = 0;

#pragma omp parallel for private (j,k,neighqty)
  for (i = 0; i < (*pOSet).size; i++)
  {
    int *neighs = ut_alloc_1d_int ((*pqcloud).pts.size ());
    double Eij, *fij = ut_alloc_1d (3);

    nanoflann::SearchParams params;
    //params.sorted = false;

    std::vector < std::pair < size_t, double > >ret_matches;
    neighqty = qindex->radiusSearch ((*pOSet).q[i], (*pOOpt).neighdE, ret_matches, params);
    for (j = 0; j < neighqty; j++)
      neighs[j] = ret_matches[j].first;

    (*pOOpt).avneighqty += neighqty - 1;

    for (j = 1; j < neighqty; j++)
    {
      if (ret_matches[j].second > 3.999999)
        continue;

      net_ori_uniform_opt_forces_comp_neigh (pOSet, i, neighs[j], fij, &Eij, pqcloud);

      Eij *= 0.5;
      E[i] += Eij;

      for (k = 0; k < 3; k++)
        f[i][k] += fij[k];
    }

    ut_free_1d_int (neighs);
    ut_free_1d (fij);
  }

  (*pOOpt).avneighqty /= (*pOSet).size;

  return;
}

void
net_ori_uniform_opt_rot_ori (double **f, double alpha, int i,
                     struct OL_SET *pOSet, double **dq)
{
  double *r = ol_r_alloc ();
  double *q = ol_q_alloc ();

  neut_ori_f_qdot (f[i], q);
  ut_array_1d_scale (q, 4, alpha);

  ut_array_1d_memcpy (dq[i], 3, q + 1);

  ol_q_q_q_ref ((*pOSet).q[i], q, q);

  ut_array_1d_add ((*pOSet).q[i], q, 4, (*pOSet).q[i]);

  ut_array_1d_normalize ((*pOSet).q[i], 4);

  ol_q_q ((*pOSet).q[i], (*pOSet).q[i]);

  ol_r_free (r);
  ol_q_free (q);

  return;
}
