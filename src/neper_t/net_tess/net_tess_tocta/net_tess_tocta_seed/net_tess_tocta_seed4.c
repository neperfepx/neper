/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_tocta_seed_.h"

int
net_tess_tocta_seed_set_w_pre (gsl_rng * r, struct TESS Dom, double *n,
                               char *wtype, double *w, int wqty,
                               double *plane, double *pdistmin,
                               double *pdistmax)
{
  int i;
  double *dist = ut_alloc_1d (Dom.VerQty + 1);

  (void) wtype;

  ut_array_1d_memcpy (n, 3, plane + 1);

  // distmin and distmax are the min and max values from the plane (with
  // constant = 0) and the vertices
  for (i = 1; i <= Dom.VerQty; i++)
    dist[i] = ut_space_point_plane_dist_signed (Dom.VerCoo[i], plane);

  *pdistmin = ut_array_1d_min (dist + 1, Dom.VerQty);
  *pdistmax = ut_array_1d_max (dist + 1, Dom.VerQty);

  // First plane: shifting it onto the distmin vertex then shifting it
  // further down by a random value within [0,sum_of_widths]
  plane[0] = *pdistmin - gsl_rng_uniform (r) * ut_array_1d_sum (w, wqty);

  ut_free_1d (&dist);

  return 0;
}

int
net_tess_tocta_seed_set_addlam (double distcur, double *n, double w, int w_id,
                                struct SEEDSET *pSSet)
{
  (*pSSet).N++;
  (*pSSet).LamEq = ut_realloc_2d_addline ((*pSSet).LamEq, (*pSSet).N + 1, 4);
  (*pSSet).LamWidth = ut_realloc_1d ((*pSSet).LamWidth, (*pSSet).N + 1);
  (*pSSet).LamWidthId =
    ut_realloc_1d_int ((*pSSet).LamWidthId, (*pSSet).N + 1);

  ut_array_1d_memcpy (n, 3, (*pSSet).LamEq[(*pSSet).N] + 1);
  (*pSSet).LamEq[(*pSSet).N][0] = distcur;
  (*pSSet).LamWidth[(*pSSet).N] = w;
  (*pSSet).LamWidthId[(*pSSet).N] = w_id + 1;

  return 0;
}
