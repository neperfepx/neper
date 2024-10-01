/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_sim_.h"

void
neut_tess_sim (char *simdir, struct TESS Tess, struct SIM *pSim)
{
  neut_sim_set_zero (pSim);
  ut_string_string (simdir, &((*pSim).simdir));
  (*pSim).CellQty = Tess.CellQty;
  ut_string_string (Tess.CellOriDes, &((*pSim).OriDes));

  ut_string_string ("simulation.tess", &(*pSim).tess);

  return;
}
