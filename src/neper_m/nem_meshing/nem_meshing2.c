/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_.h"

void
nem_meshing_gen (struct TESS Tess, struct NODES *pNodes)
{
  if (Tess.Periodic)
  {
    (*pNodes).Periodic = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy ((*pNodes).Periodic, 3, Tess.Periodic);
    (*pNodes).PeriodicDist = ut_alloc_1d (3);
    ut_array_1d_memcpy ((*pNodes).PeriodicDist, 3, Tess.PeriodicDist);
  }

  return;
}
