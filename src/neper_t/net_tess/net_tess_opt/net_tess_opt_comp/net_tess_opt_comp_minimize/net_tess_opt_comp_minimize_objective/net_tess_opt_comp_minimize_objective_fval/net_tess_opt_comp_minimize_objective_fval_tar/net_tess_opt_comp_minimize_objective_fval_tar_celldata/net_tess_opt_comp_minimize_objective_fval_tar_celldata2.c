/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_fval_tar_celldata_.h"

// The cell errors ((*pTOpt).curcellerror[id]) are the differences between
// the current and target cell values, 0 for the invalid cells (for which no
// value is computed), and the value of the target is computed from them
// (the penalties of the cells are added to the objective function, see
// net_tess_opt_comp_minimize_objective_fval_comp).

void
net_tess_opt_comp_minimize_objective_fval_tar_celldata_scalar (struct TOPT *pTOpt,
                                                               int id)
{
  int i;
  double *err = (*pTOpt).curcellerror[id];

  for (i = 1; i <= (*pTOpt).CellQty; ++i)
  {
    err[i] = 0;

    if ((*pTOpt).curcellpenalty[i] == 0)
    {
      if ((*pTOpt).tarcellvalqty[id] == 1)
        err[i] =
          (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][0];

      else if ((*pTOpt).tarcellvalqty[id] == 2)
      {
        if ((*pTOpt).curcellval[id][i][0] < (*pTOpt).tarcellval[id][i][0])
          err[i] =
            (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][0];
        else if ((*pTOpt).curcellval[id][i][0] >
                 (*pTOpt).tarcellval[id][i][1])
          err[i] =
            (*pTOpt).curcellval[id][i][0] - (*pTOpt).tarcellval[id][i][1];
      }

      else
        abort ();
    }
  }

  (*pTOpt).curval[id] =
    ut_array_1d_lmean_expr (err + 1, (*pTOpt).CellQty,
                            (*pTOpt).tarobjective[id]);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_celldata_centroid (struct TOPT *pTOpt,
                                                                 int id)
{
  int i;
  double *err = (*pTOpt).curcellerror[id];

  for (i = 1; i <= (*pTOpt).CellQty; ++i)
  {
    err[i] = 0;

    if ((*pTOpt).curcellpenalty[i] == 0)
    {
      if ((*pTOpt).Dim == 2)
        err[i] =
          ut_space_dist_2d ((*pTOpt).curcellval[id][i],
                            (*pTOpt).tarcellval[id][i]);
      else
        err[i] =
          ut_space_dist ((*pTOpt).curcellval[id][i],
                         (*pTOpt).tarcellval[id][i]);

      err[i] /= (*pTOpt).tarrefval[id];
    }
  }

  (*pTOpt).curval[id] =
    ut_array_1d_lmean_expr (err + 1, (*pTOpt).CellQty,
                            (*pTOpt).tarobjective[id]);

  return;
}

// the valid cells of tolerance (delta) >= 1000 are not considered
void
net_tess_opt_comp_minimize_objective_fval_tar_celldata_centroidtol (struct TOPT *pTOpt,
                                                                    int id)
{
  int i, qty;
  double delta, *err = (*pTOpt).curcellerror[id];
  double *val = ut_alloc_1d ((*pTOpt).CellQty + 1);

  qty = 0;
  for (i = 1; i <= (*pTOpt).CellQty; ++i)
  {
    delta = (*pTOpt).tarcellval[id][i][(*pTOpt).Dim];

    err[i] = 0;

    if ((*pTOpt).curcellpenalty[i] == 0 && delta < 1000)
    {
      if ((*pTOpt).Dim == 2)
        err[i] =
          ut_space_dist_2d ((*pTOpt).curcellval[id][i],
                            (*pTOpt).tarcellval[id][i]);
      else
        err[i] =
          ut_space_dist ((*pTOpt).curcellval[id][i],
                         (*pTOpt).tarcellval[id][i]);

      err[i] /= delta;
      err[i] /= (*pTOpt).tarrefval[id];
    }

    if ((*pTOpt).curcellpenalty[i] > 0 || delta < 1000)
      val[++qty] = err[i];
  }

  (*pTOpt).curval[id] =
    ut_array_1d_lmean_expr (val + 1, qty, (*pTOpt).tarobjective[id]);

  ut_free_1d (&val);

  return;
}

void
net_tess_opt_comp_minimize_objective_fval_tar_celldata_centroiddiameq (struct TOPT
                                                                       *pTOpt, int id)
{
  int i;
  double val1, val2, *err = (*pTOpt).curcellerror[id];

  for (i = 1; i <= (*pTOpt).CellQty; ++i)
  {
    err[i] = 0;

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

      err[i] = sqrt (pow (val1, 2) + 0.25 * pow (val2, 2));
      err[i] /= (*pTOpt).tarrefval[id];
    }
  }

  (*pTOpt).curval[id] =
    ut_array_1d_lmean_expr (err + 1, (*pTOpt).CellQty,
                            (*pTOpt).tarobjective[id]);

  return;
}

// The value of a cell is a weighted sum of the distances of its points of
// the raster tessellation, and the value of the target is a normalized sum
// of the cell values.  The cell errors are the shares of the cells in the
// value of the target, set so that their quadratic mean is the value of the
// target.
void
net_tess_opt_comp_minimize_objective_fval_tar_celldata_tesr (struct TOPT *pTOpt,
                                                             int id)
{
  int i;
  double val, sum, lv, size, avdiameq, *err = (*pTOpt).curcellerror[id];

  sum = 0;
  for (i = 1; i <= (*pTOpt).CellQty; ++i)
    if ((*pTOpt).curcellpenalty[i] == 0)
      sum += (*pTOpt).curcellval[id][i][0];

  if (!strcmp ((*pTOpt).objective_tesrval, "bounddist"))
  {
    neut_tess_cellavdiameq ((*pTOpt).Dom, (*pTOpt).CellQty, &avdiameq);
    val = avdiameq * sqrt (2. / (*pTOpt).tavoxqtyini * sum);
  }

  else if (!strcmp ((*pTOpt).objective_tesrval, "intervol"))
  {
    neut_tess_size ((*pTOpt).Dom, &size);
    neut_tesr_voxlengtheq ((*pTOpt).tartesr, &lv);
    val = pow (lv, (*pTOpt).Dim - 1) / size * sum;
  }

  else
    abort ();

  (*pTOpt).curval[id] = val;

  for (i = 1; i <= (*pTOpt).CellQty; ++i)
    err[i] = ((*pTOpt).curcellpenalty[i] == 0 && sum > 0) ?
      val * sqrt ((*pTOpt).CellQty * (*pTOpt).curcellval[id][i][0] / sum) : 0;

  return;
}
