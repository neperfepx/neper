/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_weight_.h"

int
net_tess_opt_init_sset_weight_stat_radeq (int pos, char
				   *var, struct TOPT TOpt, double *weight)
{
  int i, id, dim;
  double avradeq, totvol, domvol;
  double *val = ut_alloc_1d (TOpt.CellQty + 1);
  int *id2 = ut_alloc_1d_int (TOpt.CellQty + 1);

  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, TOpt.SSet.Random);
  for (i = 1; i <= TOpt.CellQty; i++)
    val[i] = gsl_rng_uniform (r);
  ut_array_1d_sort_index (val + 1, TOpt.CellQty, id2 + 1);
  ut_array_1d_int_addval (id2 + 1, TOpt.CellQty, 1, id2 + 1);

  neut_tess_cellavradeq (TOpt.Dom, TOpt.CellQty, &avradeq);

  id = 1;
  totvol = 0;

  if (!strcmp (var, "diameq") || !strcmp (var, "radeq"))
    dim = 1;
  else if (!strcmp (var, "size"))
    dim = TOpt.Dim;
  else
    abort ();

  for (i = TOpt.tarcdf0[pos].size - 1; i >= 0; i--)
    while (id <= TOpt.CellQty
	   && TOpt.tarcdf0[pos].y[i] <
	   (TOpt.CellQty - id + 0.5) / TOpt.CellQty)
    {
      weight[id2[id]] = pow (avradeq * TOpt.tarcdf0[pos].x[i], 1. / dim);

      if (TOpt.Dim == 2)
	totvol += M_PI * pow (weight[id2[id]], 2);
      else if (TOpt.Dim == 3)
	totvol += 4. * M_PI * 0.33333333333 * pow (weight[id2[id]], 3);
      else
	abort ();

      id++;
    }

  neut_tess_size (TOpt.Dom, &domvol);

  ut_array_1d_scale (weight + 1, TOpt.CellQty, pow (domvol / totvol,
						    1. / TOpt.Dim));

  gsl_rng_free (r);
  ut_free_1d (val);
  ut_free_1d_int (id2);

  return 0;
}

int
net_tess_opt_init_sset_weight_celldata_radeq (int pos, char *var, struct TOPT TOpt,
                                              double *radeq)
{
  int i;
  double avradeq, avsize;

  if (!strcmp (var, "diameq") || !strcmp (var, "radeq"))
  {
    neut_tess_cellavradeq (TOpt.Dom, TOpt.CellQty, &avradeq);

    for (i = 1; i <= TOpt.CellQty; i++)
      radeq[i] = avradeq * TOpt.tarcellval[pos][i][0];
  }

  else if (!strcmp (var, "size"))
  {
    neut_tess_cellavsize (TOpt.Dom, TOpt.CellQty, &avsize);

    for (i = 1; i <= TOpt.CellQty; i++)
      ut_space_size_radeq (TOpt.Dim, avsize * TOpt.tarcellval[pos][i][0], radeq + i);
  }

  else
    abort ();

  return 0;
}
