/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_uniform_.h"

void
net_ori_uniform_opt_forces_comp (struct OL_SET *pOSet, int i, int j,
                                 double *fij, double *pEij)
{
  unsigned int k;
  double *w = ol_q_alloc ();
  double *f = ut_alloc_1d (3);
  double d, dinv, pimdinv;

  ut_array_1d_zero (fij, 3);
  (*pEij) = 0;
  for (k = 1; k <= (*pOSet).nc; ++k)
  {
    ol_q_crysym ((*pOSet).q[j], (*pOSet).crysym, k, w);
    ol_q_q_qmisori ((*pOSet).q[i], w, w);

    ol_q_lnq (w, f);
    d = ut_array_1d_norm (f, 3);
    dinv = 1 / d;
    pimdinv = 1 / (M_PI - d);
    ut_array_1d_scale (f, 3, -dinv * (dinv * dinv - pimdinv * pimdinv));

    ut_array_1d_add (fij, f, 3, fij);
    (*pEij) += dinv + pimdinv;
  }

  ut_free_1d (&w);
  ut_free_1d (&f);

  return;
}

void
net_ori_uniform_opt_forces_comp_neigh (struct OL_SET *pOSet, int i,
                                       int orioptineigh, double *fij,
                                       double *pEij, QCLOUD * pqcloud)
{
  double d, dinv, *w = ol_q_alloc ();

  ut_array_1d_zero (fij, 3);
  (*pEij) = 0;
  ol_q_inverse ((*pOSet).q[i], w);
  ol_q_q_q_ref (w, (*pqcloud).pts[orioptineigh].q, w);

  ol_q_lnq (w, fij);
  d = ut_array_1d_norm (fij, 3);
  dinv = 1 / d;

  ut_array_1d_scale (fij, 3, -dinv * dinv * dinv);
  (*pEij) += dinv;

  ut_free_1d (&w);

  return;
}
