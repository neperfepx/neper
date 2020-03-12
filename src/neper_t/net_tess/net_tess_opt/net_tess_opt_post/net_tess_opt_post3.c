/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_post_.h"

void
net_tess_opt_post_modes (struct TOPT TOpt, struct TESS *Tess, int tessid)
{
  int tar, i, j, size, pos, tmp;
  double weight, *val = ut_alloc_1d (TOpt.CellQty);
  int *id = ut_alloc_1d_int (TOpt.CellQty);
  double **modeval = NULL;
  double *x = NULL;
  int *modehint = NULL;
  int *mode = ut_alloc_1d_int (TOpt.CellQty);
  double *tmpval = NULL;

  tar = -1;
  for (i = 0; i < TOpt.tarqty; i++)
    if (TOpt.tarmodeqty[i] > 1)
    {
      tar = i;
      break;
    }

  Tess[tessid].CellModeId = ut_alloc_1d_int (Tess[tessid].CellQty + 1);

  if (tar != -1)
  {
    size = TOpt.tarmodecdf0[tar][0].size;
    x = ut_alloc_1d (size);
    ut_array_1d_memcpy (TOpt.tarmodecdf0[tar][0].x, size, x);

    modeval = ut_alloc_1d_pdouble (TOpt.tarmodeqty[tar]);
    modehint = ut_alloc_1d_int (TOpt.tarmodeqty[tar]);
    tmpval = ut_alloc_1d (TOpt.tarmodeqty[tar]);

    double *qty = ut_alloc_1d (TOpt.tarmodeqty[tar]);
    ut_array_1d_memcpy (TOpt.tarmodefact[tar], TOpt.tarmodeqty[tar], qty);
    ut_array_1d_scale (qty, TOpt.tarmodeqty[tar], TOpt.CellQty);
    ut_array_1d_round_keepsum (qty, TOpt.tarmodeqty[tar], qty);

    for (i = 0; i < TOpt.tarmodeqty[tar]; i++)
    {
      modeval[i] = ut_alloc_1d (size);
      ut_array_1d_memcpy (TOpt.tarmodecdf0[tar][i].y, size, modeval[i]);
      ut_array_1d_scale (modeval[i], size, qty[i]);
    }

    for (i = 0; i < TOpt.CellQty; i++)
      val[i] = TOpt.curcellval[tar][i + 1][0];

    ut_array_1d_sort_index (val, TOpt.CellQty, id);

    pos = 0;
    for (i = 0; i < size && pos < TOpt.CellQty; i++)
      if (x[i] > val[id[pos]])
      {
        for (j = 0; j < TOpt.tarmodeqty[tar]; j++)
        {
          weight =
            ut_num_min (1000,
                        1 / (TOpt.tarmodecdf0[tar][j].y[i] *
                             (1 - TOpt.tarmodecdf0[tar][j].y[i])));
          tmpval[j] = (modeval[j][i] - modehint[j]) * weight;
        }

        tmp = ut_array_1d_max_index (tmpval, TOpt.tarmodeqty[tar]);

        if (mode[id[pos]] != 0)
          abort ();

        if (tmpval[tmp] * qty[tmp] > 0.5)
        {
          mode[id[pos]] = tmp + 1;
          modehint[tmp]++;
          i--;
          pos++;
        }
      }

    ut_free_2d (&modeval, TOpt.tarmodeqty[tar]);
    ut_free_1d_int (&modehint);
    ut_free_1d (&tmpval);

    for (i = 1; i <= Tess[tessid].CellQty; i++)
      Tess[tessid].CellModeId[i] = mode[Tess[tessid].CellId[i] - 1];

    if (ut_array_1d_int_eltpos
        (Tess[tessid].CellModeId + 1, Tess[tessid].CellQty, 0) != -1)
      ut_print_neperbug ();

    ut_free_1d (&qty);
  }

  else
    ut_array_1d_int_set (Tess[tessid].CellModeId + 1, Tess[tessid].CellQty,
                         1);

  ut_free_1d (&val);
  ut_free_1d_int (&id);
  ut_free_1d (&x);

  return;
}
