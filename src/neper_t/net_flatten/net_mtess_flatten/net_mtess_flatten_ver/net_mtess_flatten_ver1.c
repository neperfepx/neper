/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_ver_.h"

void
net_mtess_flatten_ver (struct MTESS MTess, struct TESS *Tess,
		       int TessId, int *CTessIds, int CTessQty,
		       struct TESS *pFTess,
		       struct TESSE *TessE, struct FLATTEN *pFlatten)
{
  int i, j, id, domver, domverid;
  int *dom = ut_alloc_1d_int (2);

  // Vertices
  (*pFTess).VerQty = 0;
  (*pFTess).VerCoo = ut_alloc_1d_pdouble (1);

  TessE[TessId].VerQty = Tess[TessId].VerQty;
  TessE[TessId].VerFVer = ut_alloc_1d_int (Tess[TessId].VerQty + 1);

  // adding vertices of tessellation Ln
  for (j = 1; j <= Tess[TessId].VerQty; j++)
  {
    ut_array_1d_int_set_2 (dom, 0, j);
    neut_tess_addver (pFTess, Tess[TessId].VerCoo[j]);
    TessE[TessId].VerFVer[j] = (*pFTess).VerQty;
    neut_flatten_addver (pFlatten, dom);
  }

  // adding vertices of tessellations Ln+1
  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    TessE[id].VerFVer = ut_alloc_1d_int (Tess[id].VerQty + 1);

    for (j = 1; j <= Tess[id].VerQty; j++)
      if (Tess[id].VerDom[j][0] != 0)
      {
	dom[0] = Tess[id].VerDom[j][0];
	if (dom[0] == 1)
	  dom[1] = MTess.TessDomEdgeNb[id][Tess[id].VerDom[j][1]];
	else if (dom[0] == 2)
	  dom[1] = MTess.TessDomFaceNb[id][Tess[id].VerDom[j][1]];
	else if (dom[0] == -1)
	{
	  dom[0] = 3;
	  dom[1] = MTess.TessDom[id][1];
	}
	else
	  abort ();

	neut_tess_addver (pFTess, Tess[id].VerCoo[j]);
	TessE[id].VerFVer[j] = (*pFTess).VerQty;
	neut_flatten_addver (pFlatten, dom);
      }
      else
      {
	domver = Tess[id].VerDom[j][1];
	domverid = MTess.TessDomVerNb[id][domver];
	TessE[id].VerFVer[j] = TessE[TessId].VerFVer[domverid];
      }
  }

  // filling pFVerTessVerQty and pFVerTessVerNb
  net_mtess_flatten_ver_init_fvertessver (Tess, CTessIds,
					  CTessQty, TessE, pFlatten);

  ut_free_1d_int (dom);

  return;
}
