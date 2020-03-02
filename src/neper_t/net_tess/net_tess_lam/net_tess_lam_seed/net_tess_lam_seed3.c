/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_lam_seed_.h"

int
net_tess_lam_seed_readargs_w (char *value, struct MTESS MTess,
                              struct TESS *Tess, int domtess, int dompoly,
                              char **pwtype, double **pw, int *pwqty)
{
  int i;
  char *string = ut_alloc_1d_char (1000);
  char **tmp = NULL;
  char *mid = NULL;

  (*pwqty) = 0;

  // reading string
  if (ut_string_isfilename (value))
  {
    ut_string_string ("file", pwtype);
    neut_mtess_tess_poly_mid (MTess, Tess[domtess], dompoly, &mid);
    net_multiscale_arg_0d_char_fscanf (value, mid, string);
  }
  else
  {
    ut_string_string ("expression", pwtype);
    ut_string_string (value, &string);
  }

  // converting string into *pw / *pwqty
  ut_list_break (string, NEUT_SEP_DEP, &tmp, pwqty);

  (*pw) = ut_alloc_1d (*pwqty);
  for (i = 0; i < *pwqty; i++)
    if (sscanf (tmp[i], "%lf", (*pw) + i) != 1)
      ut_print_neperbug ();

  ut_free_1d_char (&string);
  ut_free_2d_char (&tmp, *pwqty);
  ut_free_1d_char (&mid);

  return 0;
}

int
net_tess_lam_seed_readargs_v (char *value, struct MTESS MTess,
                              struct TESS *Tess, int domtess, int dompoly,
                              char **pvtype, double **pv, int *pvqty)
{
  char *mid = NULL;

  (*pvqty) = 0;

  if (!strcmp (value, "random"))
    ut_string_string ("random", pvtype);
  else if (!strncmp (value, "random(", 7))
    ut_string_string (value, pvtype);
  else if (!strncmp (value, "(", 1))
    ut_string_string (value, pvtype);
  else if (!strncmp (value, "crysdir(", 8))
    ut_string_string (value, pvtype);
  else if (ut_string_isfilename (value))
  {
    ut_string_string ("file", pvtype);
    (*pvqty) = 1;
    (*pv) = ut_alloc_1d (3);
    neut_mtess_tess_poly_mid (MTess, Tess[domtess], dompoly, &mid);
    net_multiscale_arg_1d_fscanf (value, mid, *pv, 3);
    ut_array_1d_normalize (*pv, 3);
  }
  else
    ut_print_message (2, 2, "Failed to read 'v' argument.\n");

  ut_free_1d_char (&mid);

  return 0;
}

int
net_tess_lam_seed_readargs_pos (char *value, struct MTESS MTess,
                                struct TESS *Tess, int domtess, int dompoly,
                                char **ppostype, char **ppos)
{
  char *mid = NULL;

  (*ppos) = ut_alloc_1d_char (1000);

  // reading string
  if (ut_string_isfilename (value))
  {
    ut_string_string ("file", ppostype);
    neut_mtess_tess_poly_mid (MTess, Tess[domtess], dompoly, &mid);
    net_multiscale_arg_0d_char_fscanf (value, mid, *ppos);
  }
  else
  {
    ut_string_string ("value", ppostype);
    ut_string_string (value, ppos);
  }

  ut_free_1d_char (&mid);

  return 0;
}

int
net_tess_lam_seed_set_init (struct SEEDSET *pSSet)
{
  (*pSSet).LamEq = ut_alloc_2d (1, 4);
  (*pSSet).LamWidthId = ut_alloc_1d_int (1);
  (*pSSet).LamWidth = ut_alloc_1d (1);

  return 0;
}

int
net_tess_lam_seed_set_normal (struct SEEDSET *SSet, int dtess, int dcell,
                              gsl_rng * r, char *vtype, double *v, int vqty,
                              double *n)
{
  int id, dim = SSet[1].Dim;
  double theta;
  double *q = NULL;

  if (dtess != 0)
    q = SSet[dtess].q[dcell];

  if (!strcmp (vtype, "file") && vqty == 1)
    ut_array_1d_memcpy (v, 3, n);

  else if (!strcmp (vtype, "random"))
  {
    if (dim == 3)
      ol_nb_r (gsl_rng_uniform (r), gsl_rng_uniform (r), n);
    else if (dim == 2)
    {
      theta = 2 * M_PI * gsl_rng_uniform (r);
      n[0] = cos (theta);
      n[1] = sin (theta);
    }
    else
      abort ();
  }

  else if (!strncmp (vtype, "random(", 7))
  {
    int ptqty;
    double **pts = NULL;
    sscanf (vtype, "random(%d)", &ptqty);
    pts = ut_alloc_2d (ptqty, 3);

    ut_space_sphere_points (ptqty, 1, pts);

    id = ptqty * gsl_rng_uniform (r);
    ut_array_1d_memcpy (pts[id], 3, n);

    ut_free_2d (&pts, ptqty);
  }

  else if (!strncmp (vtype, "(", 1))
  {
    sscanf (vtype, "(%lf,%lf,%lf)", n, n + 1, n + 2);
    ut_array_1d_normalize (n, 3);
  }

  else if (!strncmp (vtype, "crysdir(", 8))
  {
    double **g = ol_g_alloc ();
    sscanf (vtype, "crysdir(%lf,%lf,%lf)", n, n + 1, n + 2);
    ol_q_g (q, g);
    ol_g_inverse (g, g);
    ol_g_vect_vect (g, n, n);
    ut_array_1d_normalize (n, 3);
    ol_g_free (g);
  }

  else
    abort ();

  return 0;
}

int
net_tess_lam_seed_set_lam (struct TESS Dom, gsl_rng * r, double *n,
                           char *wtype, double *w, int wqty, char *postype,
                           char *pos, struct SEEDSET *pSSet)
{
  int w_id;
  double coo, distmin, distmax;
  double *plane = ut_alloc_1d (4);

  net_tess_lam_seed_set_w_pre (r, Dom, n, wtype, w, wqty, postype, pos, plane,
                               &distmin, &distmax);

  (*pSSet).N = 0;
  coo = plane[0];
  w_id = -1;
  do
  {
    w_id = ut_array_rotpos (0, wqty - 1, w_id, 1);
    coo += w[w_id];
    if (coo < distmin)
      continue;

    net_tess_lam_seed_set_addlam (coo, n, w[w_id], w_id, pSSet);
  }
  while (coo < distmax);

  ut_free_1d (&plane);

  return 0;
}

int
net_tess_lam_seed_set_finalize (struct SEEDSET *pSSet)
{
  (*pSSet).Nall = (*pSSet).N;
  (*pSSet).SeedCoo = ut_alloc_2d ((*pSSet).N + 1, 3);
  (*pSSet).SeedCoo0 = ut_alloc_2d ((*pSSet).N + 1, 3);
  (*pSSet).SeedWeight = ut_alloc_1d ((*pSSet).N + 1);
  (*pSSet).Size = ut_alloc_2d (3, 2);

  return 0;
}
