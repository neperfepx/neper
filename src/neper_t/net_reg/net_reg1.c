/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_.h"

void
net_reg (struct IN_T In, struct TESS *pTess, struct REG *pReg)
{
  int DelId = 0, cellqty = (*pTess).CellQty;

  if ((*pTess).CellQty == 0)
    return;

  net_reg_init (In, pTess, pReg);

  ut_print_message (0, 2, "");

  /***********************************************************************
   * if fmax > 0 & mloop >= 1, small edge deletion */

  if ((*pReg).fmax > 1e-15 && (*pReg).mloop >= 1)
    net_reg_merge (pTess, *pReg, &DelId);

  /*----------------------------------------------------------------------
   * updating face equations */
  if ((*pTess).Dim == 3)
    net_reg_faceeq (pTess);

  /*----------------------------------------------------------------------
   * updating periodicity */
  if (!strcmp ((*pTess).Type, "periodic"))
    net_reg_periodic (pTess);

  /*----------------------------------------------------------------------
   * compressing */
  neut_tess_compress (pTess);

  if ((*pTess).CellQty < cellqty)
  {
    printf ("\n");
    ut_print_message (0, 2, "%d cell%s deleted.", cellqty - (*pTess).CellQty,
                      cellqty - (*pTess).CellQty > 1 ? "s" : "");
  }

  if ((*pTess).Dim == 3)
    neut_tess_init_domtessface (pTess);

  neut_tess_init_domtessedge (pTess);
  neut_tess_init_domtessver (pTess);

  /***********************************************************************
   * testing full tessellation */

#ifdef DEVEL_DEBUGGING_TEST
  if (neut_tess_test (*pTess, 0) != 0 || neut_tess_test_dom (*pTess, 0) != 0
      || (!strcmp ((*pTess).Type, "periodic")
          && neut_tess_test_per (*pTess, 0) != 0))
    ut_print_message (2, 2, "The tessellation is not valid.\n");
#endif

  printf ("\n");

  return;
}
