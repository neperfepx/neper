/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_.h"

#ifdef HAVE_NLOPT
int
net_tess_opt_comp_nlopt (double *x, struct TOPT *pTOpt)
{
  int res = 0, forcestop;
  double val;

  net_tess_opt_comp_nlopt_init (pTOpt);

  res = nlopt_optimize ((*pTOpt).opt, x, &val);
  forcestop = nlopt_get_force_stop ((*pTOpt).opt);

  if ((*pTOpt).iter != 1)
  {
    nlopt_set_maxeval ((*pTOpt).opt, 1);
    nlopt_optimize ((*pTOpt).opt, x, &val);
    nlopt_set_force_stop ((*pTOpt).opt, forcestop);
  }

  // nlopt_destroy ((*pTOpt).opt);

  return res;
}
#endif

int
net_tess_opt_comp_once (double *x, struct TOPT *pTOpt)
{
  net_tess_opt_comp_objective (0, x, NULL, pTOpt);

  return 0;
}

int
net_tess_opt_comp_rand (double *x, struct TOPT *pTOpt)
{
  int id, seedqty, alldimqty, dimqty, *alldims = NULL;
  double min, max, *x_cpy = ut_alloc_1d ((*pTOpt).xqty);
  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);

  net_tess_opt_comp_rand_init (pTOpt);

  net_tess_opt_comp_rand_read (*pTOpt, &seedqty, &dimqty, &min, &max, &id,
			       &alldims, &alldimqty);

  gsl_rng_set (r, id);

  do
  {
    net_tess_opt_comp_objective (0, x, NULL, pTOpt);

    if ((*pTOpt).iter % 2)
    {
      ut_array_1d_memcpy (x_cpy, (*pTOpt).xqty, x);
      net_tess_opt_comp_rand_shift (x, pTOpt, seedqty, dimqty, min, max,
	                            alldims, alldimqty, r);
    }
    else
      net_tess_opt_comp_rand_revert (x, *pTOpt, x_cpy);
  }
  while ((*pTOpt).iter <= (*pTOpt).itermax);

  ut_free_1d_int (alldims);
  ut_free_1d (x_cpy);
  gsl_rng_free (r);

  return 0;
}

int
net_tess_opt_comp_lloyd (double *x, struct TOPT *pTOpt)
{
  int i, seed, var, status;
  double **centroid = ut_alloc_2d ((*pTOpt).CellQty + 1, 3);
  double fact;

  if (sscanf ((*pTOpt).algoname, "lloyd(%lf)", &fact) != 1)
    fact = 1;

  do
  {
    if ((*pTOpt).iter > 0)
    {
      for (i = 1; i <= (*pTOpt).CellQty; i++)
	neut_poly_centroid ((*pTOpt).Poly[i], centroid[i]);

      for (i = 0; i < (*pTOpt).xqty; i++)
      {
	seed = (*pTOpt).x_seed[i];
	var = (*pTOpt).x_var[i];

	if (var >= 0 && var <= 2)
	  x[i] = x[i] * (1 - fact) + centroid[seed][var] * fact;
      }
    }

    net_tess_opt_comp_objective (0, x, NULL, pTOpt);

    status = 0;
    if ((*pTOpt).iter == (*pTOpt).itermax)
      status = 1;
    else if ((*pTOpt).objval < (*pTOpt).val)
      status = 1;
  }
  while (!status);

  ut_free_2d (centroid, (*pTOpt).CellQty + 1);

  return 0;
}
