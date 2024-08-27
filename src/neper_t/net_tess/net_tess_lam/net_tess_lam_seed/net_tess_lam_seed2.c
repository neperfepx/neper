/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_lam_seed_.h"

int
net_tess_lam_seed_readargs (struct IN_T In, int level, char *morpho, struct MTESS MTess,
                            struct TESS *Tess, int domtess, int dompoly,
                            char **pntype, int **pn,
                            char **pwtype, double **pw, int *pwqty,
                            char **pvtype, double **pv,
                            char **ppostype, char **ppos, double *preps)
{
  int i, j, varqty, tmpi;
  double *tmp = NULL;
  char **vars = NULL, **vals = NULL;
  double *n_double = NULL;

  (*pwqty) = 0;
  ut_string_string ("none", pntype);
  ut_string_string ("none", pwtype);
  ut_string_string ("random", pvtype);
  ut_string_string ("scal", ppostype);
  ut_string_string ("random", ppos);
  (*preps) = 1e-2;

  ut_string_function (morpho, NULL, &vars, &vals, &varqty);

  if (strcmp (In.n[level], "from_morpho"))
  {
    varqty++;
    vars = ut_realloc_1d_pchar_null (vars, varqty, 1);
    vals = ut_realloc_1d_pchar_null (vals, varqty, 1);
    ut_string_string ("n", vars + varqty - 1);
    ut_string_string (In.n[level], vals + varqty - 1);
  }

  for (i = 0; i < varqty; i++)
  {
    if (!strcmp (vars[i], "n"))
    {
      net_tess_lam_seed_readargs_w (vals[i], MTess, Tess, domtess, dompoly,
                                    pntype, &n_double, &tmpi);
      (*pn) = ut_alloc_1d_int (1);
      for (j = 0; j < 1; j++)
        (*pn)[j] = n_double[j];
    }

    else if (!strcmp (vars[i], "w"))
      net_tess_lam_seed_readargs_w (vals[i], MTess, Tess, domtess, dompoly,
                                    pwtype, pw, pwqty);

    else if (!strcmp (vars[i], "v"))
      net_tess_lam_seed_readargs_v (vals[i], MTess, Tess, domtess, dompoly,
                                    pvtype, pv);

    else if (!strcmp (vars[i], "pos"))
      net_tess_lam_seed_readargs_pos (vals[i], MTess, Tess, domtess, dompoly,
                                      ppostype, ppos);

    else if (!strcmp (vars[i], "reps"))
    {
      net_tess_lam_seed_readargs_w (vals[i], MTess, Tess, domtess, dompoly,
                                      NULL, &tmp, NULL);
      (*preps) = tmp[0];
    }

    else
      abort ();
  }

  if ((strcmp (*pntype, "none") || strcmp (In.n[level], "from_morpho")) && strcmp (*pwtype, "none"))
    ut_print_message (2, 2, "w and n are mutually exclusive.");

  if (!strcmp (*pntype, "none") && !strcmp (*pwtype, "none") && !strcmp (In.n[level], "from_morpho"))
    ut_print_message (2, 2, "Either w or n must be defined.");

  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d (&tmp);
  ut_free_1d (&n_double);

  return 0;
}

int
net_tess_lam_seed_set (struct IN_T In, int level, struct MTESS MTess,
                       struct TESS *Tess, int dtess, int dcell,
                       struct TESS Dom, char *ntype, int *n,
                       char **pwtype, double **pw, int *pwqty,
                       char *vtype, double *v, char *postype,
                       char *pos, double reps, struct SEEDSET *SSet,
                       struct SEEDSET *pSSet)
{
  double *normal = ut_alloc_1d (3);
  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);

  (void) level;

  net_tess_lam_seed_set_init (pSSet);

  neut_seedset_set_zero (pSSet);

  ut_string_string ("standard", &(*pSSet).Type);

  struct TESS Cell;
  neut_tess_set_zero (&Cell);
  neut_tess_poly_tess (Tess[dtess], dcell, &Cell);
  (*pSSet).Size = ut_alloc_2d (3, 2);
  neut_tess_bbox (Cell, (*pSSet).Size);
  neut_tess_free (&Cell);

  net_ori_mtess_id (In, MTess, Tess, dtess, dcell, pSSet);

  net_ori_mtess_randseed (MTess, Tess, dtess, dcell, SSet, 1, pSSet);

  gsl_rng_set (r, (*pSSet).Random);
  net_tess_lam_seed_set_normal (SSet, dtess, dcell, r, vtype, v, normal);

  net_tess_lam_seed_set_lam (Dom, r, normal, ntype, n, pwtype, pw, pwqty, postype, pos, reps, pSSet);

  net_tess_lam_seed_set_finalize (pSSet);

  gsl_rng_free (r);
  ut_free_1d (&normal);

  return 0;
}
