/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_.h"

void
net_tess_opt_init_sset (struct IN_T In, int level, struct MTESS MTess,
                        struct TESS *Tess, int dtess, int dcell,
                        int TessId, struct SEEDSET *SSet, struct TOPT *pTOpt)
{
  int pos;
  int *qty = ut_alloc_1d_int ((*pTOpt).CellQty + 1);
  double *rad = ut_alloc_1d ((*pTOpt).CellQty + 1);
  char *var = NULL, *cooexpr = NULL, *weightexpr = NULL;

  if (!strcmp ((*pTOpt).optitype, "morpho"))
    ut_print_message (0, 2, "Setting seeds... ");

  net_tess_opt_init_sset_pre (In, level, MTess, Tess, dtess, dcell, SSet, &var,
                              &pos, &weightexpr, &cooexpr, pTOpt);

  if (!strcmp ((*pTOpt).optitype, "morpho"))
    net_tess_opt_init_sset_weight (MTess, Tess, dtess, dcell, *pTOpt, var, pos,
                                   weightexpr, rad);
  else
    ut_array_1d_set (rad + 1, (*pTOpt).CellQty, 1);

  net_tess_opt_init_sset_multiseed (In, level, *pTOpt, var, rad, qty);

  if (!strcmp ((*pTOpt).optitype, "morpho"))
    net_tess_opt_init_sset_coo (MTess, Tess, dtess, dcell, pTOpt, var, pos,
                                cooexpr, rad, qty);

  if (!strcmp ((*pTOpt).optitype, "ori"))
    net_tess_opt_init_sset_ori (TessId, SSet, pTOpt);

  net_tess_opt_init_sset_post (In, level, Tess[dtess], dcell, pTOpt);

  ut_free_1d_int (&qty);
  ut_free_1d (&rad);
  ut_free_1d_char (&var);
  ut_free_1d_char (&cooexpr);
  ut_free_1d_char (&weightexpr);

  return;
}
