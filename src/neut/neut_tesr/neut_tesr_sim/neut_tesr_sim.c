/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_sim_.h"

void
neut_tesr_sim (char *simdir, struct TESR Tesr, struct SIM *pSim)
{
  neut_sim_set_zero (pSim);
  ut_string_string (simdir, &((*pSim).simdir));
  (*pSim).CellQty = Tesr.CellQty;
  ut_string_string (Tesr.CellOriDes, &((*pSim).OriDes));

  ut_string_string ("simulation.tesr", &(*pSim).tesr);

  return;
}
