/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_sim_.h"

// setting the raster tessellation data of a simulation
void
neut_tesr_sim (struct TESR Tesr, struct SIM *pSim)
{
  (*pSim).CellQty = Tesr.CellQty;

  if (!(*pSim).OriDes)
    ut_string_string (Tesr.CellOriDes, &((*pSim).OriDes));

  neut_sim_init_memberqty (pSim);

  return;
}
