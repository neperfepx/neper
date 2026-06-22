/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_tess_tocta_seed_.h"

int
net_tess_tocta_seed_readargs (char *morpho, int *n)
{
  int i, varqty;
  char **vars = NULL, **vals = NULL;

  ut_string_function (morpho, NULL, &vars, &vals, &varqty);

  if (varqty == 1)
  {
    ut_math_eval_int (vals[0], 0, NULL, NULL, n);
    n[1] = n[0];
    n[2] = n[0];
  }
  else if (varqty == 3)
    for (i = 0; i < varqty; i++)
      ut_math_eval_int (vals[i], 0, NULL, NULL, n + i);

  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);

  return 0;
}

int
net_tess_tocta_seed_set (struct IN_T In, struct MTESS MTess,
                         struct TESS *Tess, int dtess, int dcell,
                         struct TESS Dom, int *n, struct SEEDSET *SSet,
                         struct SEEDSET *pSSet)
{
  char *ori = NULL, *crysym = NULL;

  ut_print_message (0, 2, "Setting seeds...\n");

  ut_string_string ("standard", &(*pSSet).Type);

  struct TESS Cell;
  neut_tess_set_zero (&Cell);
  neut_tess_poly_tess (Tess[dtess], dcell, &Cell);
  (*pSSet).Size = ut_alloc_2d (3, 2);
  neut_tess_bbox (Cell, (*pSSet).Size);
  neut_tess_free (&Cell);

  net_ori_mtess_id (In, MTess, Tess, 0, 1, pSSet);

  net_ori_mtess_randseed (MTess, Tess, 0, 1, SSet, 1, pSSet);

  neut_seedset_bcc (Dom, n, In.periodic, pSSet);

  if (!ut_array_1d_int_sum (In.periodic, 3))
    net_tess_tocta_seed_set_finalize (pSSet);

  ut_free_1d_char (&ori);
  ut_free_1d_char (&crysym);

  return 0;
}
