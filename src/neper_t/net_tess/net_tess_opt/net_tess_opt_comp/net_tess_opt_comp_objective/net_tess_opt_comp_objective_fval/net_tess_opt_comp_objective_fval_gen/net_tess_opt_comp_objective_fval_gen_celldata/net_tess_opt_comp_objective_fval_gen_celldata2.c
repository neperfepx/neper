/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_celldata_.h"

void
net_tess_opt_comp_objective_fval_gen_celldata_scalar (struct TOPT *pTOpt,
                                                      int id)
{
  int i;
  double *val = ut_alloc_1d ((*pTOpt).CellQty + 1);

  for (i = 1; i <= (*pTOpt).CellQty; ++i)
    if ((*pTOpt).curcellpenalty[i] == 0)
    {
      if ((*pTOpt).tarcellvalqty[id] == 1)
        val[i] =
          (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][0];

      else if ((*pTOpt).tarcellvalqty[id] == 2)
      {
        if ((*pTOpt).curcellval[id][i][0] < (*pTOpt).tarcellval[id][i][0])
          val[i] =
            (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][0];
        else if ((*pTOpt).curcellval[id][i][0] <
                 (*pTOpt).tarcellval[id][i][1])
          val[i] = 0;
        else
          val[i] =
            (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][1];
      }

      else
        abort ();
    }
    else
      val[i] = 1000;

  (*pTOpt).curval[id] =
    ut_array_1d_lmean_expr (val + 1, (*pTOpt).CellQty,
                            (*pTOpt).tarobjective[id]);
  // (*pTOpt).curval[id] /= (*pTOpt).Dim;

  ut_free_1d (&val);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_celldata_centroid (struct TOPT *pTOpt,
                                                        int id)
{
  int i;
  double *val = ut_alloc_1d ((*pTOpt).CellQty + 1);

  for (i = 1; i <= (*pTOpt).CellQty; ++i)
  {
    // this happens when a cell is void
    if ((*pTOpt).curcellpenalty[i] == 0)
    {
      if ((*pTOpt).Dim == 2)
        val[i] =
          ut_space_dist_2d ((*pTOpt).curcellval[id][i],
                            (*pTOpt).tarcellval[id][i]);
      else
        val[i] =
          ut_space_dist ((*pTOpt).curcellval[id][i],
                         (*pTOpt).tarcellval[id][i]);
    }
    else
      val[i] = 1000 * (*pTOpt).curcellpenalty[i];

    val[i] /= (*pTOpt).tarrefval[id];
  }

  (*pTOpt).curval[id] =
    ut_array_1d_lmean_expr (val + 1, (*pTOpt).CellQty,
                            (*pTOpt).tarobjective[id]);

  ut_free_1d (&val);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_celldata_centroidtol (struct TOPT *pTOpt,
                                                           int id)
{
  int i, qty;
  double delta, *val = ut_alloc_1d ((*pTOpt).CellQty + 1);

  qty = 0;
  for (i = 1; i <= (*pTOpt).CellQty; ++i)
  {
    delta = (*pTOpt).tarcellval[id][i][(*pTOpt).Dim];

    // this happens when a cell is void
    if ((*pTOpt).curcellpenalty[i] == 0)
    {
      if (delta < 1000)
      {
        qty++;
        if ((*pTOpt).Dim == 2)
          val[qty] =
            ut_space_dist_2d ((*pTOpt).curcellval[id][i],
                              (*pTOpt).tarcellval[id][i]);
        else
          val[qty] =
            ut_space_dist ((*pTOpt).curcellval[id][i],
                           (*pTOpt).tarcellval[id][i]);

        val[qty] /= delta;
      }
    }
    else
    {
      qty++;
      val[qty] = 1000 * (*pTOpt).curcellpenalty[i];
    }

    if (((*pTOpt).curcellpenalty[i] == 0 && delta < 1000)
        || (*pTOpt).curcellpenalty[i])
      val[qty] /= (*pTOpt).tarrefval[id];
  }

  (*pTOpt).curval[id] =
    ut_array_1d_lmean_expr (val + 1, qty, (*pTOpt).tarobjective[id]);

  ut_free_1d (&val);

  return;
}

void
net_tess_opt_comp_objective_fval_gen_celldata_centroiddiameq (struct TOPT
                                                              *pTOpt, int id)
{
  int i;
  double val1, val2, *val = ut_alloc_1d ((*pTOpt).CellQty + 1);

  (*pTOpt).curval[id] = 0;
  for (i = 1; i <= (*pTOpt).CellQty; ++i)
  {
    // this happens when a cell is void
    if ((*pTOpt).curcellpenalty[i] == 0)
    {
      if ((*pTOpt).Dim == 2)
        val1 =
          ut_space_dist_2d ((*pTOpt).curcellval[id][i],
                            (*pTOpt).tarcellval[id][i]);
      else
        val1 =
          ut_space_dist ((*pTOpt).curcellval[id][i],
                         (*pTOpt).tarcellval[id][i]);
      val2 =
        (*pTOpt).tarcellval[id][i][(*pTOpt).Dim] -
        (*pTOpt).curcellval[id][i][(*pTOpt).Dim];
    }
    else
    {
      val1 = 1000 * (*pTOpt).curcellpenalty[i];
      val2 = 1000 * (*pTOpt).curcellpenalty[i];
    }

    val[i] = sqrt (pow (val1, 2) + 0.25 * pow (val2, 2));
    val[i] /= (*pTOpt).tarrefval[id];
  }

  (*pTOpt).curval[id] =
    ut_array_1d_lmean_expr (val + 1, (*pTOpt).CellQty,
                            (*pTOpt).tarobjective[id]);

  ut_free_1d (&val);

  return;
}
