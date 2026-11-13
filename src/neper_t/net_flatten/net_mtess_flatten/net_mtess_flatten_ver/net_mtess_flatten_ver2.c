/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_ver_.h"

void
net_mtess_flatten_ver_init_fvertessver (struct TESS *Tess, int *CTessIds,
                                        int CTessQty, int *VTessIds,
                                        int VTessQty, struct TESSE *TessE,
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

  // Init VerTessVerQty and VerTessVerNb from TessE[].VerFVer (the
  // arrays allocated by neut_flatten_addver have NULL lines)
  ut_free_1d_int (&(*pFlatten).VerTessVerQty);
  ut_free_1d_ppint (&(*pFlatten).VerTessVerNb);
  (*pFlatten).VerTessVerQty = ut_alloc_1d_int ((*pFlatten).VerQty + 1);
  (*pFlatten).VerTessVerNb = ut_alloc_2d_pint ((*pFlatten).VerQty + 1, 1);
  // (the virtual tessellations have unflattened vertices)
  for (i = 0; i < CTessQty + VTessQty; i++)
  {
    id = (i < CTessQty) ? CTessIds[i] : VTessIds[i - CTessQty];

    for (j = 1; j <= Tess[id].VerQty; j++)
    {
      fver = TessE[id].VerFVer[j];
      if (fver == 0)
        continue;
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
