/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_sim_.h"

// setting the tessellation data of a simulation
void
neut_tess_sim (struct TESS Tess, struct SIM *pSim)
{
  (*pSim).CellQty = Tess.CellQty;

  if (!(*pSim).OriDes)
    ut_string_string (Tess.CellOriDes, &((*pSim).OriDes));

  neut_sim_init_memberqty (pSim);

  return;
}
