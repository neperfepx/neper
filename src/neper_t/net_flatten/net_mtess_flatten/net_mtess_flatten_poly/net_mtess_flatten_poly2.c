/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_poly_.h"

void
net_mtess_flatten_cell_body (struct MTESS MTess, struct TESS *Tess,
                             int *CTessIds, int CTessQty, struct TESS *pFTess,
                             struct TESSE **pTessE, struct FLATTEN *pFlatten)
{
  int i, j, k, l, id, face, poly, cell;
  int *dom = ut_alloc_1d_int (2);

  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    if (i == 0)
    {
      if (Tess[id].CellId)
        (*pFTess).CellId = ut_alloc_1d_int (1);
      /*
      if (Tess[id].CellBody)
        (*pFTess).CellBody = ut_alloc_1d_int (1);
        */
    }

    ut_array_1d_int_set_2 (dom, 3, MTess.TessDom[id][1]);

    for (j = 1; j <= Tess[id].PolyQty; j++)
    {
      poly = neut_tess_addpoly_alloc (pFTess);
      cell = neut_tess_addcell_alloc (pFTess);

      neut_flatten_addpoly (pFlatten, dom);

      neut_tess_cell_addcentre (pFTess, cell, Tess[id].SeedCoo[j],
                                Tess[id].SeedWeight[j]);

      /* moved to net_mtess_flatten_cell
      if (Tess[id].CellOri)
        ut_array_1d_memcpy (Tess[id].CellOri[j], 4, (*pFTess).CellOri[cell]);
      */

      for (k = 1; k <= Tess[id].PolyFaceQty[j]; k++)
      {
        face = Tess[id].PolyFaceNb[j][k];
        for (l = 0; l < (*pTessE)[id].FaceFFaceQty[face]; l++)
        {
          neut_tess_poly_addface (pFTess, poly,
                                  (*pTessE)[id].FaceFFaceNb[face][l],
                                  Tess[id].PolyFaceOri[j][k] *
                                  (*pTessE)[id].FaceFFaceOri[face][l]);
        }
      }

      /*
      int domtess = MTess.TessDom[id][0];
      int dompoly = MTess.TessDom[id][1];
      if (Tess[domtess].CellBody)
        (*pFTess).CellBody[poly] = Tess[domtess].CellBody[dompoly];
      */

      neut_tesse_poly_addpoly (&(*pTessE)[id], j, poly);
    }
  }

  neut_tess_init_facepoly (pFTess);

  ut_free_1d_int (&dom);

  return;
}
