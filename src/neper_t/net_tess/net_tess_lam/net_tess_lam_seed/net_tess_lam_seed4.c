/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_lam_seed_.h"

int
net_tess_lam_seed_set_w_pre (gsl_rng * r, struct TESS Dom, double *normal,
                             char *ntype, int *n, char **pwtype, double **pw, int *pwqty,
                             char *postype, char *pos, double *plane, double reps,
                             double *pdistmin, double *pdistmax)
{
  int i;
  double fact, *dist = ut_alloc_1d (Dom.VerQty + 1);
  double tmp1, tmp2, dirlength, wall;

  if (strcmp (*pwtype, "none"))
    wall = ut_array_1d_sum (*pw, *pwqty);
  else if (strcmp (ntype, "none"))
  {
    neut_tess_poly_dir_length (Dom, 1, normal, &dirlength);
    *pwqty = 1;
    (*pw) = ut_alloc_1d (1);
    (*pw)[0] = dirlength / n[0];
    wall = (*pw)[0];
    ut_string_string ("from_n", pwtype);
    ut_string_string ("start", &pos);
  }
  else
    abort ();

  ut_array_1d_memcpy (normal, 3, plane + 1);

  // distmin and distmax are the min and max values from the plane (with
  // constant = 0) and the vertices
  for (i = 1; i <= Dom.VerQty; i++)
    dist[i] = ut_space_point_plane_dist_signed (Dom.VerCoo[i], plane);

  *pdistmin = ut_array_1d_min (dist + 1, Dom.VerQty);
  *pdistmax = ut_array_1d_max (dist + 1, Dom.VerQty);

  // First plane: shifting it onto the distmin vertex then shifting it
  // further down by a random value within [0,sum_of_widths]
  if (!strcmp (pos, "random"))
    plane[0] = *pdistmin - gsl_rng_uniform (r) * wall;
  else if (!strcmp (pos, "start"))
    plane[0] = *pdistmin;
  else if (!strcmp (pos, "half"))
    plane[0] = *pdistmin - 0.5 * (*pw)[0];
  else if (!strcmp (pos, "optimal"))
  {
    tmp1 = (*pdistmax - *pdistmin) / wall;

    if (tmp1 - floor (tmp1) < reps)
      tmp2 = floor (tmp1);
    else
      tmp2 = ceil (tmp1);

    plane[0] = *pdistmin - (tmp2 - tmp1) * wall / 2;
  }
  else if (sscanf (pos, "%lf", &fact))
    plane[0] = *pdistmin - fact * (*pw)[0];
  else
    ut_print_message (2, 3, "Failed to process expression `pos=%s'.\n",
                      postype);

  ut_free_1d (&dist);

  return 0;
}

int
net_tess_lam_seed_set_addlam (double distcur, double *normal, double w, int w_id,
                              struct SEEDSET *pSSet)
{
  (*pSSet).N++;
  (*pSSet).LamEq = ut_realloc_2d_addline ((*pSSet).LamEq, (*pSSet).N + 1, 4);
  (*pSSet).LamWidth = ut_realloc_1d ((*pSSet).LamWidth, (*pSSet).N + 1);

  ut_array_1d_memcpy (normal, 3, (*pSSet).LamEq[(*pSSet).N] + 1);
  (*pSSet).LamEq[(*pSSet).N][0] = distcur;
  (*pSSet).LamWidth[(*pSSet).N] = w;

  if (w_id != -1)
  {
    (*pSSet).LamWidthId =
      ut_realloc_1d_int ((*pSSet).LamWidthId, (*pSSet).N + 1);
    (*pSSet).LamWidthId[(*pSSet).N] = w_id + 1;
  }

  return 0;
}
