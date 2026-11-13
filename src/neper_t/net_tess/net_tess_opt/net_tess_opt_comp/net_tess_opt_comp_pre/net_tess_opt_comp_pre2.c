/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_pre_.h"

void
net_tess_opt_comp_pre_morpho (struct TOPT *pTOpt)
{
  int i, j, k, seed, dim, qty;
  char **parts = NULL;
  int *seeds = ut_alloc_1d_int ((*pTOpt).seedoptiqty);

  ut_array_1d_int_memcpy ((*pTOpt).seedopti, (*pTOpt).seedoptiqty, seeds);

  ut_list_break ((*pTOpt).dof, NEUT_SEP_NODEP, &parts, &qty);

  (*pTOpt).x = ut_alloc_1d (qty * (*pTOpt).seedoptiqty);

  (*pTOpt).xqty = qty * (*pTOpt).seedoptiqty;

  (*pTOpt).x_pvar = ut_alloc_1d_pdouble ((*pTOpt).xqty);

  (*pTOpt).x_seed = ut_alloc_1d_int ((*pTOpt).xqty);
  (*pTOpt).x_var = ut_alloc_1d_int ((*pTOpt).xqty);
  (*pTOpt).seedvar_x =
    ut_alloc_2d_int ((*pTOpt).SSet.N + 1, (*pTOpt).Dim + 1);
  ut_array_2d_int_set ((*pTOpt).seedvar_x + 1, (*pTOpt).SSet.N,
                       (*pTOpt).Dim + 1, -1);


  k = 0;
  for (j = 0; j < (*pTOpt).seedoptiqty; j++)
  {
    seed = seeds[j];
    for (i = 0; i < qty; i++)
    {
      if (!strcmp (parts[i], "x") || !strcmp (parts[i], "y")
          || !strcmp (parts[i], "z"))
      {
        dim = parts[i][0] - 'x';
        (*pTOpt).x_seed[k] = seed;
        (*pTOpt).x_var[k] = dim;
        (*pTOpt).x_pvar[k] = &((*pTOpt).SSet.SeedCoo0[seed][dim]);
        (*pTOpt).x[k] = (*pTOpt).SSet.SeedCoo0[seed][dim];
        (*pTOpt).seedvar_x[seed][dim] = k;
      }
      else if (!strcmp (parts[i], "w"))
      {
        (*pTOpt).x_seed[k] = seed;
        (*pTOpt).x_var[k] = (*pTOpt).SSet.Dim;
        (*pTOpt).x_pvar[k] = &((*pTOpt).SSet.SeedWeight[seed]);
        // the optimization variable is sqrt (w), which is homogeneous to x, y and z
        (*pTOpt).x[k] = sqrt ((*pTOpt).SSet.SeedWeight[seed]);
        (*pTOpt).seedvar_x[seed][(*pTOpt).SSet.Dim] = k;
      }
      else
        abort ();

      k++;
    }
  }

  if ((*pTOpt).iter == 0)
    for (k = 0; k < qty * (*pTOpt).seedoptiqty; k++)
      if ((*pTOpt).x[k] < (*pTOpt).boundl[k]
          || (*pTOpt).x[k] > (*pTOpt).boundu[k])
        ut_print_message (2, 3, "Initial solution out of bounds.\n");

  ut_free_2d_char (&parts, qty);
  ut_free_1d_int (&seeds);

  return;
}

void
net_tess_opt_comp_pre_ori (struct TOPT *pTOpt)
{
  int i, j, k, seed, dim, qty;
  char **parts = NULL;

  ut_list_break ((*pTOpt).dof, NEUT_SEP_NODEP, &parts, &qty);

  (*pTOpt).x = ut_alloc_1d (qty * (*pTOpt).seedoptiqty);

  (*pTOpt).xqty = qty * (*pTOpt).seedoptiqty;

  (*pTOpt).x_pvar = ut_alloc_1d_pdouble ((*pTOpt).xqty);

  (*pTOpt).x_seed = ut_alloc_1d_int ((*pTOpt).xqty);
  (*pTOpt).x_var = ut_alloc_1d_int ((*pTOpt).xqty);
  (*pTOpt).seedvar_x =
    ut_alloc_2d_int ((*pTOpt).SSet.N + 1, (*pTOpt).Dim + 2);
  ut_array_2d_int_set ((*pTOpt).seedvar_x + 1, (*pTOpt).SSet.N,
                       (*pTOpt).Dim + 2, -1);

  k = 0;
  for (j = 0; j < (*pTOpt).seedoptiqty; j++)
  {
    seed = (*pTOpt).seedopti[j];
    for (i = 0; i < qty; i++)
    {
      if (!strcmp (parts[i], "r1") || !strcmp (parts[i], "r2")
          || !strcmp (parts[i], "r3"))
      {
        dim = parts[i][1] - '1';
        (*pTOpt).x_seed[k] = seed;
        (*pTOpt).x_var[k] = dim;
        (*pTOpt).x_pvar[k] = &((*pTOpt).SSet.SeedOriR[seed][dim]);
        (*pTOpt).x[k] = (*pTOpt).SSet.SeedOriR[seed][dim];
        (*pTOpt).seedvar_x[seed][dim] = k;
      }
      else if (!strcmp (parts[i], "rt"))
      {
        (*pTOpt).x_seed[k] = seed;
        (*pTOpt).x_var[k] = (*pTOpt).SSet.Dim;
        (*pTOpt).x_pvar[k] = &((*pTOpt).SSet.SeedOriTheta[seed]);
        (*pTOpt).x[k] = (*pTOpt).SSet.SeedOriTheta[seed];
        (*pTOpt).seedvar_x[seed][(*pTOpt).SSet.Dim] = k;
      }
      else if (!strcmp (parts[i], "rw"))
      {
        (*pTOpt).x_seed[k] = seed;
        (*pTOpt).x_var[k] = (*pTOpt).SSet.Dim + 1;
        (*pTOpt).x_pvar[k] = &((*pTOpt).SSet.SeedOriWeight[seed]);
        (*pTOpt).x[k] = (*pTOpt).SSet.SeedOriWeight[seed];
        (*pTOpt).seedvar_x[seed][(*pTOpt).SSet.Dim + 1] = k;
      }
      else
        abort ();

      k++;
    }
  }

  if ((*pTOpt).iter == 0)
    for (k = 0; k < qty * (*pTOpt).seedoptiqty; k++)
      if ((*pTOpt).x[k] < (*pTOpt).boundl[k]
          || (*pTOpt).x[k] > (*pTOpt).boundu[k])
        ut_print_message (2, 3, "Initial solution out of bounds.\n");

  ut_free_2d_char (&parts, qty);

  return;
}
