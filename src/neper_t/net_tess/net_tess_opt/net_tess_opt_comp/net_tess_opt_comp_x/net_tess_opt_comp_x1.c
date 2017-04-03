/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_x_.h"

void
net_tess_opt_comp_x (struct TOPT *pTOpt, double **px)
{
  int i, j, k, seed, dim, qty;
  char **parts = NULL;

  ut_string_separate ((*pTOpt).dof, NEUT_SEP_NODEP, &parts, &qty);

  (*px) = ut_alloc_1d (qty * (*pTOpt).seedoptiqty);

  (*pTOpt).xqty = qty * (*pTOpt).seedoptiqty;

  (*pTOpt).x_pvar = ut_alloc_1d_pdouble ((*pTOpt).xqty);

  (*pTOpt).x_seed = ut_alloc_1d_int ((*pTOpt).xqty);
  (*pTOpt).x_var = ut_alloc_1d_int ((*pTOpt).xqty);

  k = 0;
  for (j = 0; j < (*pTOpt).seedoptiqty; j++)
  {
    seed = (*pTOpt).seedopti[j];
    for (i = 0; i < qty; i++)
    {
      if (!strcmp (parts[i], "x") || !strcmp (parts[i], "y")
	  || !strcmp (parts[i], "z"))
      {
	dim = parts[i][0] - 'x';
	(*pTOpt).x_seed[k] = seed;
	(*pTOpt).x_var[k] = dim;
	(*pTOpt).x_pvar[k] = &((*pTOpt).SSet.SeedCoo0[seed][dim]);
	(*px)[k++] = (*pTOpt).SSet.SeedCoo0[seed][dim];

      }
      else if (!strcmp (parts[i], "w"))
      {
	(*pTOpt).x_seed[k] = seed;
	(*pTOpt).x_var[k] = 3;
	(*pTOpt).x_pvar[k] = &((*pTOpt).SSet.SeedWeight[seed]);
	(*px)[k++] = (*pTOpt).SSet.SeedWeight[seed];
      }
      else
	abort ();
    }
  }

  //  k = 0;
  //  for (i = 0; i < qty; i++)
  //  {
  //    if (!strcmp (parts[i], "x") || !strcmp (parts[i], "y")
  // || !strcmp (parts[i], "z"))
  //    {
  //      dim = parts[i][0] - 'x';
  //      for (j = 0; j < (*pTOpt).seedoptiqty; j++)
  //      {
  // seed = (*pTOpt).seedopti[j];
  // (*px)[k++] = (*pTOpt).SSet.SeedCoo0[seed][dim];
  //      }
  //    }
  //    else if (!strcmp (parts[i], "w"))
  //      for (j = 0; j < (*pTOpt).seedoptiqty; j++)
  //      {
  // seed = (*pTOpt).seedopti[j];
  // (*px)[k++] = (*pTOpt).SSet.SeedWeight[seed];
  //      }
  //    else
  //      abort ();
  //  }

  if ((*pTOpt).iter == 0)
    for (k = 0; k < qty * (*pTOpt).seedoptiqty; k++)
      if ((*px)[k] < (*pTOpt).boundl[k] || (*px)[k] > (*pTOpt).boundu[k])
	ut_print_message (2, 3, "Initial solution out of bounds.\n");

  ut_free_2d_char (parts, qty);

  return;
}
