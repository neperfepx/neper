/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_post_.h"

void
net_tess_opt_post (struct MTESS *pMTess, struct TESS *Tess, int dtess,
                   int dcell, int tessid, struct TOPT TOpt,
                   struct SEEDSET *SSet)
{
  int i;

  if (!strcmp (TOpt.optitype, "morpho"))
    net_tess_opt_post_tess (pMTess, Tess, dtess, dcell, tessid, TOpt, SSet);

  else if (!strcmp (TOpt.optitype, "ori"))
  {
    neut_seedset_memcpy (TOpt.SSet, SSet + tessid);

    Tess[tessid].CellQty = SSet[tessid].N;
    Tess[tessid].CellOri = ut_alloc_2d (Tess[tessid].CellQty + 1, 4);
    ut_array_2d_memcpy (SSet[tessid].SeedOri + 1, Tess[tessid].CellQty, 4,
                        Tess[tessid].CellOri + 1);

    if (SSet[tessid].SeedOriWeight)
    {
      Tess[tessid].CellWeight = ut_alloc_1d (Tess[tessid].CellQty + 1);
      ut_array_1d_memcpy (SSet[tessid].SeedOriWeight + 1, Tess[tessid].CellQty,
                          Tess[tessid].CellWeight + 1);
      ut_array_1d_scale (Tess[tessid].CellWeight + 1, Tess[tessid].CellQty,
                         1. / ut_array_1d_mean (Tess[tessid].CellWeight + 1, Tess[tessid].CellQty));
    }

    if (SSet[tessid].SeedOriTheta)
    {
      char *tmp = ut_alloc_1d_char (1000);

      Tess[tessid].CellOriDistrib = ut_alloc_1d_pchar (Tess[tessid].CellQty + 1);
      for (i = 1; i <= Tess[tessid].CellQty; i++)
      {
        sprintf (tmp, "normal(%.12f)", SSet[tessid].SeedOriTheta[i] * 180 / M_PI);
        ut_string_string (tmp, Tess[tessid].CellOriDistrib + i);
      }
      ut_free_1d_char (&tmp);
    }
  }

  return;
}
