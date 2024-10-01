/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_show_tesr_.h"

void
nev_print_show_tesr_cell (char *expr, struct TESR Tesr,
                    struct PRINT *pPrint)
{
  int i, j, k, cell, status, varqty = 0;
  char **vars = NULL;
  double *vals = NULL;
  int *showcell = NULL;

  if (Tesr.Dim == 3)
  {
    (*pPrint).showpoly = ut_alloc_1d_int (Tesr.CellQty + 1);
    showcell = (*pPrint).showpoly;
  }
  else if (Tesr.Dim == 2)
  {
    (*pPrint).showface = ut_alloc_1d_int (Tesr.CellQty + 1);
    showcell = (*pPrint).showface;
  }

  // ut_alloc_1d_int (Tesr.CellQty + 1);

  if (!(*pPrint).showvox)
    (*pPrint).showvox = ut_alloc_3d_int (Tesr.size[0] + 1, Tesr.size[1] + 1, Tesr.size[2] + 1);

  ut_array_3d_int_zero ((*pPrint).showvox, Tesr.size[0] + 1, Tesr.size[1] + 1, Tesr.size[2] + 1);

  // checking for a general expression
  status = nev_print_show_genexpr (expr, showcell, Tesr.CellQty);

  if (status != 0)
  {
    ut_math_vars (expr, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Tesr.CellQty; i++)
    {
      for (j = 0; j < varqty; j++)
      {
        status = neut_tesr_var_val_one (Tesr, "cell", i, vars[j], vals + j, NULL);

        if (status)
          ut_print_exprbug (vars[j]);
      }

      status = ut_math_eval_int (expr, varqty, vars, vals, showcell + i);
      if (status)
        ut_print_exprbug (expr);
    }
  }

  for (cell = 1; cell <= Tesr.CellQty; cell++)
    if (showcell[cell])
      for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
        for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
          for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
            if (Tesr.VoxCell[i][j][k] == cell)
              (*pPrint).showvox[i][j][k] = 1;

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return;
}

void
nev_print_show_tesr_edge (char *expr, struct PRINT *pPrint)
{
  ut_string_string (expr, &(*pPrint).showedgestring);

  return;
}

void
nev_print_show_tesr_vox (char *option, char *expr, struct TESR Tesr,
                   struct PRINT *pPrint)
{
  int i, j, k, varqty, status;
  char **vars = NULL;

  ut_string_string (expr, &expr);
  if (!strcmp (expr, "all"))
    ut_string_string ("1", &expr);
  else if (!strcmp (expr, "none"))
    ut_string_string ("0", &expr);

  if (!strcmp (option, "vox"))
    ut_string_string (expr, &(*pPrint).showvoxstring);
  else
    abort ();

  if (!(*pPrint).showvox)
    (*pPrint).showvox =
      ut_alloc_3d_int (Tesr.size[0] + 1, Tesr.size[1] + 1, Tesr.size[2] + 1);

  ut_math_vars (expr, &vars, &varqty);

  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
      {
        int l, pos;
        double res;
        double *vals = ut_alloc_1d (varqty);

        neut_tesr_pos3_vox (Tesr, i, j, k, &pos);
        for (l = 0; l < varqty; l++)
        {
          status = neut_tesr_var_val_one (Tesr, "vox", pos, vars[l], vals +
                                          l, NULL);
          if (status)
            ut_print_exprbug (vars[l]);
        }

        if (!ut_math_eval (expr, varqty, vars, vals, &res))
          (*pPrint).showvox[i][j][k] = res;
        else
          ut_print_exprbug (expr);

        ut_free_1d (&vals);
      }

  ut_free_2d_char (&vars, varqty);

  return;
}
