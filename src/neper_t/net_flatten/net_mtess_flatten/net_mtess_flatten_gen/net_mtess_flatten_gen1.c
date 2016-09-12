/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_gen_.h"

void
net_mtess_flatten_gen (struct TESS *Tess, int TessId, struct TESS *pFTess,
		       struct FLATTEN *pFlatten)
{
  (*pFTess).Dim = 3;
  (*pFTess).Level = Tess[TessId].Level;
  (*pFTess).TessId = TessId;
  ut_string_string ("standard", &(*pFTess).Type);

  (*pFlatten).DVerQty = Tess[TessId].VerQty;
  (*pFlatten).DEdgeQty = Tess[TessId].EdgeQty;
  (*pFlatten).DFaceQty = Tess[TessId].FaceQty;
  (*pFlatten).DPolyQty = Tess[TessId].PolyQty;

  return;
}
