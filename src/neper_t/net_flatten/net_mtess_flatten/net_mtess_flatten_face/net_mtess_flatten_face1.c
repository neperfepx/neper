/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_face_.h"

void
net_mtess_flatten_face (struct MTESS MTess, struct TESS *Tess, int TessId,
                        int *CTessIds, int CTessQty, int *VTessIds,
                        int VTessQty, struct TESS *pFTess,
                        struct TESSE **pTessE, struct FLATTEN *pFlatten)
{
  int i, id;

  (*pFTess).FaceQty = 0;

  (*pTessE)[TessId].FaceQty = Tess[TessId].FaceQty;
  (*pTessE)[TessId].FaceFFaceQty = ut_alloc_1d_int (Tess[TessId].FaceQty + 1);
  (*pTessE)[TessId].FaceFFaceNb = ut_alloc_1d_pint (Tess[TessId].FaceQty + 1);
  (*pTessE)[TessId].FaceFFaceOri =
    ut_alloc_1d_pint (Tess[TessId].FaceQty + 1);
  for (i = 0; i < CTessQty + VTessQty; i++)
  {
    id = (i < CTessQty) ? CTessIds[i] : VTessIds[i - CTessQty];

    (*pTessE)[id].FaceQty = Tess[id].FaceQty;
    (*pTessE)[id].FaceFFaceQty = ut_alloc_1d_int (Tess[id].FaceQty + 1);
    (*pTessE)[id].FaceFFaceNb = ut_alloc_1d_pint (Tess[id].FaceQty + 1);
    (*pTessE)[id].FaceFFaceOri = ut_alloc_1d_pint (Tess[id].FaceQty + 1);
  }

  net_mtess_flatten_face_body (MTess, Tess, CTessIds, CTessQty, pFTess,
                               pTessE, pFlatten);

  net_mtess_flatten_face_domface (MTess, Tess, TessId, pFTess, pTessE,
                                  pFlatten);

  // the edge faces are recomputed from scratch
  ut_free_2d_int (&(*pFTess).EdgeFaceNb, (*pFTess).EdgeQty + 1);
  ut_free_1d_int (&(*pFTess).EdgeFaceQty);

  neut_tess_init_edgeface (pFTess);

  return;
}
