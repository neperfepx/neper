/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_ver_.h"

void
net_mtess_flatten_ver_init_fvertessver (struct TESS *Tess, int *CTessIds,
                                        int CTessQty, struct TESSE *TessE,
                                        struct FLATTEN *pFlatten)
{
  int i, j, id, fver;

  // Testing whether TessE[].VerFVer is fully filled
  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    for (j = 1; j <= Tess[id].VerQty; j++)
      if (TessE[id].VerFVer[j] == 0)
        ut_print_neperbug ();
  }

  // Init VerTessVerQty and VerTessVerNb from TessE[].VerFVer
  (*pFlatten).VerTessVerQty = ut_alloc_1d_int ((*pFlatten).VerQty + 1);
  (*pFlatten).VerTessVerNb = ut_alloc_2d_pint ((*pFlatten).VerQty + 1, 1);
  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    for (j = 1; j <= Tess[id].VerQty; j++)
    {
      fver = TessE[id].VerFVer[j];
      (*pFlatten).VerTessVerQty[fver]++;
      (*pFlatten).VerTessVerNb[fver] =
        ut_realloc_2d_int_addline ((*pFlatten).VerTessVerNb[fver],
                                   (*pFlatten).VerTessVerQty[fver], 2);
      (*pFlatten).VerTessVerNb[fver][(*pFlatten).VerTessVerQty[fver] - 1][0] =
        id;
      (*pFlatten).VerTessVerNb[fver][(*pFlatten).VerTessVerQty[fver] - 1][1] =
        j;
    }
  }

  return;
}
