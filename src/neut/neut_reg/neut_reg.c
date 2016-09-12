/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_reg_.h"

void
net_reg_set_zero (struct REG *pReg)
{
  (*pReg).mloop = 2;
  (*pReg).maxedgedelqty = INT_MAX;
  (*pReg).fmax = 20;
  (*pReg).seltype = NULL;
  (*pReg).PolySel = NULL;
  (*pReg).FaceSel = NULL;
  (*pReg).EdgeSel = NULL;

  return;
}

void
net_reg_free (struct REG Reg)
{
  ut_free_1d_char (Reg.seltype);
  ut_free_1d (Reg.PolySel);
  ut_free_1d (Reg.FaceSel);
  ut_free_1d (Reg.EdgeSel);

  return;
}
