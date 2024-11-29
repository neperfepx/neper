/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_lam_seed_.h"

int
net_tess_lam_seed_readargs_w (char *value, struct MTESS MTess,
                              struct TESS *Tess, int domtess, int dompoly,
                              char **pwtype, double **pw, int *pwqty)
{
  int i, status;
  char *string = ut_alloc_1d_char (1000);
  char **tmp = NULL;
  char *mid = NULL;
  int wqty = 0;

  status = 0;

  // reading string
  if (ut_string_isfilename (value))
  {
    if (pwtype)
      ut_string_string ("file", pwtype);
    neut_mtess_tess_poly_mid (MTess, Tess[domtess], dompoly, &mid);
    net_multiscale_arg_0d_char_fscanf (value, mid, string);
  }
  else
  {
    if (pwtype)
      ut_string_string ("expression", pwtype);
    ut_string_string (value, &string);
  }

  // converting string into *pw / *pwqty
  ut_list_break (string, NEUT_SEP_DEP, &tmp, &wqty);

  if (strcmp (string, "from_morpho"))
  {
    (*pw) = ut_alloc_1d (wqty);
    for (i = 0; i < wqty; i++)
      if (sscanf (tmp[i], "%lf", (*pw) + i) != 1)
        ut_print_neperbug ();
  }
  else
  {
    status = -1;
    ut_string_string ("none", pwtype);
  }

  ut_free_1d_char (&string);
  ut_free_2d_char (&tmp, wqty);
  ut_free_1d_char (&mid);

  if (pwqty)
    *pwqty = wqty;

  return status;
}

int
net_tess_lam_seed_readargs_v (char *value, struct MTESS MTess,
                              struct TESS *Tess, int domtess, int dompoly,
                              char **pvtype, double **pv)
{
  char *mid = NULL;

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
                              gsl_rng * r, char *vtype, double *v,
                              double *n)
{
  int dim = SSet[1].Dim;
  double theta;
  double *q = NULL;

  if (dtess != 0)
    q = SSet[dtess].SeedOri[dcell];

  if (!strcmp (vtype, "file"))
    ut_array_1d_memcpy (v, 3, n);

  else if (!strncmp (vtype, "random", 6))
  {
    char *fct = NULL, **vars = NULL, **vals = NULL;
    int i, valqty, *qty = ut_alloc_1d_int (3);

    ut_string_function (vtype, &fct, &vars, &vals, &valqty);
    for (i = 0; i < valqty; i++)
      if (strcmp (vals[i], "x") && strcmp (vals[i], "y") && strcmp (vals[i], "z"))
        ut_print_message (2, 3, "Unknown direction `%s'\n", vals[i]);
      else if (++qty[vals[i][0] - 'x'] > 1)
        ut_print_message (2, 3, "Duplicate direction `%s'\n", vals[i]);

    if (dim == 3 && (valqty == 0 || valqty == 3))
      ol_nb_r (gsl_rng_uniform (r), gsl_rng_uniform (r), n);
    else if (dim == 2 || valqty == 1 || valqty == 2)
    {
      theta = 2 * M_PI * gsl_rng_uniform (r);

      if (dim == 2)
      {
        n[0] = cos (theta);
        n[1] = sin (theta);
      }
      else if (valqty == 1)
        n[vals[0][0] - 'x'] = 1;
      else if (valqty == 2)
      {
        n[vals[0][0] - 'x'] = cos (theta);
        n[vals[1][0] - 'x'] = sin (theta);
      }
      else
        abort ();
    }
    else
      abort ();

    ut_free_1d_char (&fct);
    ut_free_2d_char (&vars, valqty);
    ut_free_2d_char (&vals, valqty);
    ut_free_1d_int (&qty);
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
net_tess_lam_seed_set_lam (struct TESS Dom, gsl_rng * r, double *normal,
                           char *ntype, int *n,
                           char **pwtype, double **pw, int *pwqty,
                           char *postype, char *pos, double reps, struct SEEDSET *pSSet)
{
  int w_id;
  double coo, distmin, distmax;
  double *plane = ut_alloc_1d (4);

  net_tess_lam_seed_set_w_pre (r, Dom, normal, ntype, n, pwtype, pw, pwqty, postype, pos, plane,
                               reps, &distmin, &distmax);

  (*pSSet).N = 0;
  coo = plane[0];
  w_id = -1;
  do
  {
    w_id = ut_array_rotpos (0, *pwqty - 1, w_id, 1);
    coo += (*pw)[w_id];
    if (coo < distmin)
      continue;

    net_tess_lam_seed_set_addlam (coo, normal, (*pw)[w_id], *pwqty > 1 ? w_id : -1, pSSet);
  }
  while (coo < distmax - reps * (*pw)[w_id]);

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
