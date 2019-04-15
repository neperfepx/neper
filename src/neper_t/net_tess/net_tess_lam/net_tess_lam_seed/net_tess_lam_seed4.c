/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_lam_seed_.h"

int
net_tess_lam_seed_set_w_pre (gsl_rng *r, struct TESS Dom, double *n,
			char *wtype, double *w, int wqty,
                        char *postype, char *pos, double *plane,
                        double *pdistmin, double *pdistmax)

{
  int i;
  double *dist = ut_alloc_1d (Dom.VerQty + 1);

  (void) wtype;
  (void) postype;

  ut_array_1d_memcpy (plane + 1, 3, n);

  // distmin and distmax are the min and max values from the plane (with
  // constant = 0) and the vertices
  for (i = 1; i <= Dom.VerQty; i++)
    ut_space_point_plane_dist_signed (Dom.VerCoo[i], plane, &(dist[i]));

  *pdistmin = ut_array_1d_min (dist + 1, Dom.VerQty);
  *pdistmax = ut_array_1d_max (dist + 1, Dom.VerQty);

  // First plane: shifting it onto the distmin vertex then shifting it
  // further down by a random value within [0,sum_of_widths]
  if (!strcmp (pos, "random"))
    plane[0] = *pdistmin - gsl_rng_uniform (r) * ut_array_1d_sum (w, wqty);
  else if (!strcmp (pos, "start"))
    plane[0] = *pdistmin;
  else
    ut_print_message (2, 3, "Failed to process expression `pos=%s'.\n", postype);

  ut_free_1d (dist);

  return 0;
}

int
net_tess_lam_seed_set_addlam (double distcur, double *n, double w, int w_id,
			 struct SEEDSET *pSSet)
{
  (*pSSet).N++;
  (*pSSet).LamEq =
    ut_realloc_2d_addline ((*pSSet).LamEq, (*pSSet).N + 1, 4);
  (*pSSet).LamWidth = ut_realloc_1d ((*pSSet).LamWidth, (*pSSet).N + 1);
  (*pSSet).LamWidthId = ut_realloc_1d_int ((*pSSet).LamWidthId, (*pSSet).N + 1);

  ut_array_1d_memcpy ((*pSSet).LamEq[(*pSSet).N] + 1, 3, n);
  (*pSSet).LamEq[(*pSSet).N][0] = distcur;
  (*pSSet).LamWidth[(*pSSet).N] = w;
  (*pSSet).LamWidthId[(*pSSet).N] = w_id + 1;

  return 0;
}
