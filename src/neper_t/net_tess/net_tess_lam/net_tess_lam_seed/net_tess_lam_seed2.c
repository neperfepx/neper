/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_lam_seed_.h"

int
net_tess_lam_seed_readargs (char *morpho,
		       struct MTESS MTess, struct TESS *Tess,
		       int domtess, int dompoly,
		       char **pwtype, double **pw, int *pwqty,
		       char **pvtype, double **pv, int *pvqty)
{
  int i, varqty;
  char **vars = NULL, **vals = NULL;

  (*pwqty) = 0;
  (*pvqty) = 0;
  ut_string_string ("none", pwtype);
  ut_string_string ("random", pvtype);

  ut_string_function_separate (morpho, NULL, &vars, &vals, &varqty);

  for (i = 0; i < varqty; i++)
  {
    if (!strcmp (vars[i], "w"))
      net_tess_lam_seed_readargs_w (vals[i], MTess, Tess, domtess, dompoly,
			       pwtype, pw, pwqty);

    else if (!strcmp (vars[i], "v"))
      net_tess_lam_seed_readargs_v (vals[i], MTess, Tess, domtess, dompoly,
	                       pvtype, pv, pvqty);

    else
      abort ();
  }

  if (*pwqty <= 0)
    ut_print_message (2, 2, "missing lamellar width!\n");

  ut_free_2d_char (vars, varqty);
  ut_free_2d_char (vals, varqty);

  return 0;
}

int
net_tess_lam_seed_set (struct IN_T In, int level, struct MTESS MTess, struct TESS *Tess, int dtess,
		       int dcell, struct TESS Dom, char *wtype, double *w, int wqty,
		       char *vtype, double *v, int vqty,
		       struct SEEDSET *SSet, struct SEEDSET *pSSet)
{
  double *n = ut_alloc_1d (3);
  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);

  net_tess_lam_seed_set_init (pSSet);

  net_tess_opt_init_sset_general (In, MTess, Tess, dtess, dcell, SSet,
      pSSet, 1, NULL);

  gsl_rng_set (r, (*pSSet).Random);
  net_tess_lam_seed_set_normal (SSet, dtess, dcell, r, vtype, v, vqty, n);

  net_tess_lam_seed_set_lam (Dom, r, n, wtype, w, wqty, pSSet);

  net_tess_lam_seed_set_finalize (pSSet);

  net_tess_opt_init_sset_ori (In, level, SSet, dtess, dcell, pSSet);

  gsl_rng_free (r);
  ut_free_1d (n);

  return 0;
}
