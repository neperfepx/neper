/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_post_.h"

void
net_tess_opt_post_modes (struct TOPT TOpt, struct TESS *Tess, int tessid)
{
  int tar, i, j, size, pos, tmp;
  double *val = ut_alloc_1d (TOpt.CellQty);
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
    ut_array_1d_memcpy (x, size, TOpt.tarmodecdf0[tar][0].x);

    modeval = ut_alloc_1d_pdouble (TOpt.tarmodeqty[tar]);
    modehint = ut_alloc_1d_int (TOpt.tarmodeqty[tar]);
    tmpval = ut_alloc_1d (TOpt.tarmodeqty[tar]);

    for (i = 0; i < TOpt.tarmodeqty[tar]; i++)
    {
      modeval[i] = ut_alloc_1d (size);
      ut_array_1d_memcpy (modeval[i], size, TOpt.tarmodecdf0[tar][i].y);
      ut_array_1d_scale (modeval[i], size, TOpt.CellQty * TOpt.tarmodefact[tar][i]);
    }

    for (i = 0; i < TOpt.CellQty; i++)
      val[i] = TOpt.curcellval[tar][i + 1][0];

    ut_array_1d_sort_index (val, TOpt.CellQty, id);

    pos = 0;
    for (i = 0; i < size && pos < TOpt.CellQty; i++)
      if (x[i] > val[id[pos]])
      {
	for (j = 0; j < TOpt.tarmodeqty[tar]; j++)
	  tmpval[j] = modeval[j][i] - modehint[j];

	tmp = ut_array_1d_max_index (tmpval, TOpt.tarmodeqty[tar]);

	if (mode[id[pos]] != 0)
	  abort ();

	if (tmpval[tmp] > 0.5)
	{
	  mode[id[pos]] = tmp + 1;
	  modehint[tmp]++;
	  i--;
	  pos++;
	}
      }

    ut_free_2d_ (&modeval, TOpt.tarmodeqty[tar]);
    ut_free_1d_int_ (&modehint);
    ut_free_1d_ (&tmpval);

    for (i = 1; i <= Tess[tessid].CellQty; i++)
      Tess[tessid].CellModeId[i] = mode[Tess[tessid].CellId[i] - 1];

    if (ut_array_1d_int_eltpos (Tess[tessid].CellModeId + 1,
				Tess[tessid].CellQty, 0) != -1)
      ut_error_reportbug ();
  }

  else
    ut_array_1d_int_set (Tess[tessid].CellModeId + 1,
			 Tess[tessid].CellQty, 1);

  ut_free_1d (val);
  ut_free_1d_int (id);
  ut_free_1d (x);

  return;
}
