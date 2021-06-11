/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_.h"

void
net_reg (struct IN_T In, struct TESS *pTess, struct REG *pReg)
{
  int DelId = 0;

  if ((*pTess).CellQty == 0)
    return;

  net_reg_init (In, *pTess, pReg);

  ut_print_message (0, 2, "");

  /***********************************************************************
   * if fmax > 0 & mloop >= 1, small edge deletion */

  if ((*pReg).fmax > 1e-15 && (*pReg).mloop >= 1)
    net_reg_merge (pTess, *pReg, &DelId);

  /*----------------------------------------------------------------------
   * compressing */
  neut_tess_compress (pTess);

  if ((*pTess).Dim == 3)
    neut_tess_init_domtessface (pTess);

  neut_tess_init_domtessedge (pTess);
  neut_tess_init_domtessver (pTess);

  /***********************************************************************
   * testing full tessellation */

#ifdef DEVEL_DEBUGGING_TEST
  if (neut_tess_test (*pTess, 0) != 0 || neut_tess_test_dom (*pTess, 0) != 0)
    ut_print_message (2, 2, "The tessellation is not valid.\n");
#endif

  printf ("\n");

  return;
}
