/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_dom_.h"

void
net_mtess_flatten_dom_gen (struct TESS Tess, struct TESS *pFTess)
{
  ut_string_string (Tess.DomType, &((*pFTess).DomType));

  return;
}

void
net_mtess_flatten_dom_ver (struct TESS *Tess, int TessId, struct TESS *pFTess,
                           struct TESSE *TessE)
{
  int i, ver;

  (*pFTess).DomVerQty = Tess[TessId].DomVerQty;
  (*pFTess).DomVerLabel = ut_alloc_1d_pchar ((*pFTess).DomVerQty + 1);
  for (i = 1; i <= (*pFTess).DomVerQty; i++)
    ut_string_string (Tess[TessId].DomVerLabel[i],
                      &((*pFTess).DomVerLabel[i]));

  (*pFTess).DomVerCoo = ut_alloc_2d ((*pFTess).DomVerQty + 1, 3);
  for (i = 1; i <= (*pFTess).DomVerQty; i++)
    ut_array_1d_memcpy (Tess[TessId].DomVerCoo[i], 3, (*pFTess).DomVerCoo[i]);

  (*pFTess).DomVerEdgeQty = ut_alloc_1d_int ((*pFTess).DomVerQty + 1);
  (*pFTess).DomVerEdgeNb = ut_alloc_1d_pint ((*pFTess).DomVerQty + 1);
  for (i = 1; i <= (*pFTess).DomVerQty; i++)
  {
    (*pFTess).DomVerEdgeQty[i] = Tess[TessId].DomVerEdgeQty[i];
    (*pFTess).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pFTess).DomVerEdgeQty[i]);
    ut_array_1d_int_memcpy (Tess[TessId].DomVerEdgeNb[i],
                            (*pFTess).DomVerEdgeQty[i],
                            (*pFTess).DomVerEdgeNb[i]);
  }

  (*pFTess).DomTessVerNb = ut_alloc_1d_int ((*pFTess).DomVerQty + 1);
  for (i = 1; i <= (*pFTess).DomVerQty; i++)
  {
    ver = TessE[TessId].VerFVer[Tess[TessId].DomTessVerNb[i]];
    (*pFTess).DomTessVerNb[i] = ver;
  }

  return;
}

void
net_mtess_flatten_dom_edge (struct TESS *Tess, int TessId,
                            struct TESS *pFTess, struct TESSE *TessE)
{
  int i, j, edge;

  (*pFTess).DomEdgeQty = Tess[TessId].DomEdgeQty;
  (*pFTess).DomEdgeLabel = ut_alloc_1d_pchar ((*pFTess).DomEdgeQty + 1);
  for (i = 1; i <= (*pFTess).DomEdgeQty; i++)
    ut_string_string (Tess[TessId].DomEdgeLabel[i],
                      &((*pFTess).DomEdgeLabel[i]));

  (*pFTess).DomEdgeVerQty = ut_alloc_1d_int ((*pFTess).DomEdgeQty + 1);
  ut_array_1d_int_set ((*pFTess).DomEdgeVerQty + 1, (*pFTess).DomEdgeQty, 2);

  (*pFTess).DomEdgeVerNb = ut_alloc_2d_int ((*pFTess).DomEdgeQty + 1, 2);
  ut_array_2d_int_memcpy (Tess[TessId].DomEdgeVerNb + 1, (*pFTess).DomEdgeQty,
                          2, (*pFTess).DomEdgeVerNb + 1);

  (*pFTess).DomEdgeFaceNb = ut_alloc_2d_int ((*pFTess).DomEdgeQty + 1, 2);
  ut_array_2d_int_memcpy (Tess[TessId].DomEdgeFaceNb + 1,
                          (*pFTess).DomEdgeQty, 2,
                          (*pFTess).DomEdgeFaceNb + 1);

  (*pFTess).DomTessEdgeQty = ut_alloc_1d_int ((*pFTess).DomEdgeQty + 1);
  (*pFTess).DomTessEdgeNb = ut_alloc_1d_pint ((*pFTess).DomEdgeQty + 1);

  for (i = 1; i <= (*pFTess).DomEdgeQty; i++)
  {
    (*pFTess).DomTessEdgeNb[i] = NULL;
    for (j = 1; j <= Tess[TessId].DomTessEdgeQty[i]; j++)
    {
      edge = Tess[TessId].DomTessEdgeNb[i][j];
      (*pFTess).DomTessEdgeNb[i] =
        ut_realloc_1d_int ((*pFTess).DomTessEdgeNb[i],
                           (*pFTess).DomTessEdgeQty[i] +
                           TessE[TessId].EdgeFEdgeQty[edge] + 1);
      ut_array_1d_int_memcpy (TessE[TessId].EdgeFEdgeNb[edge],
                              TessE[TessId].EdgeFEdgeQty[edge],
                              (*pFTess).DomTessEdgeNb[i] +
                              (*pFTess).DomTessEdgeQty[i] + 1);
      (*pFTess).DomTessEdgeQty[i] += TessE[TessId].EdgeFEdgeQty[edge];
    }
  }

  return;
}

void
net_mtess_flatten_dom_face (struct TESS *Tess, int TessId,
                            struct TESS *pFTess, struct TESSE *TessE)
{
  int i, j, face;

  (*pFTess).DomFaceQty = Tess[TessId].DomFaceQty;

  (*pFTess).DomFaceLabel = ut_alloc_1d_pchar ((*pFTess).DomFaceQty + 1);
  for (i = 1; i <= (*pFTess).DomFaceQty; i++)
    ut_string_string (Tess[TessId].DomFaceLabel[i],
                      &((*pFTess).DomFaceLabel[i]));

  (*pFTess).DomFaceType = ut_alloc_1d_pchar ((*pFTess).DomFaceQty + 1);
  for (i = 1; i <= (*pFTess).DomFaceQty; i++)
    ut_string_string (Tess[TessId].DomFaceType[i],
                      &((*pFTess).DomFaceType[i]));

  (*pFTess).DomFaceParmQty = ut_alloc_1d_int ((*pFTess).DomFaceQty + 1);
  ut_array_1d_int_memcpy (Tess[TessId].DomFaceParmQty + 1,
                          (*pFTess).DomFaceQty, (*pFTess).DomFaceParmQty + 1);

  (*pFTess).DomFaceParms = ut_alloc_1d_pdouble ((*pFTess).DomFaceQty + 1);
  for (i = 1; i <= (*pFTess).DomFaceQty; i++)
  {
    (*pFTess).DomFaceParms[i] = ut_alloc_1d ((*pFTess).DomFaceParmQty[i]);
    ut_array_1d_memcpy (Tess[TessId].DomFaceParms[i],
                        (*pFTess).DomFaceParmQty[i],
                        (*pFTess).DomFaceParms[i]);
  }

  for (i = 1; i <= (*pFTess).DomFaceQty; i++)
    ut_string_string (Tess[TessId].DomFaceType[i],
                      &((*pFTess).DomFaceType[i]));

  (*pFTess).DomFaceType = ut_alloc_1d_pchar ((*pFTess).DomFaceQty + 1);
  for (i = 1; i <= (*pFTess).DomFaceQty; i++)
    ut_string_string (Tess[TessId].DomFaceType[i],
                      &((*pFTess).DomFaceType[i]));

  (*pFTess).DomFaceEq = ut_alloc_2d ((*pFTess).DomFaceQty + 1, 4);
  ut_array_2d_memcpy (Tess[TessId].DomFaceEq + 1, (*pFTess).DomFaceQty, 4,
                      (*pFTess).DomFaceEq + 1);

  (*pFTess).DomFaceVerQty = ut_alloc_1d_int ((*pFTess).DomFaceQty + 1);
  (*pFTess).DomFaceVerNb = ut_alloc_1d_pint ((*pFTess).DomFaceQty + 1);
  (*pFTess).DomFaceEdgeQty = ut_alloc_1d_int ((*pFTess).DomFaceQty + 1);
  (*pFTess).DomFaceEdgeNb = ut_alloc_1d_pint ((*pFTess).DomFaceQty + 1);
  for (i = 1; i <= (*pFTess).DomFaceQty; i++)
  {
    (*pFTess).DomFaceVerQty[i] = Tess[TessId].DomFaceVerQty[i];
    (*pFTess).DomFaceVerNb[i] =
      ut_alloc_1d_int ((*pFTess).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (Tess[TessId].DomFaceVerNb[i] + 1,
                            (*pFTess).DomFaceVerQty[i],
                            (*pFTess).DomFaceVerNb[i] + 1);
    (*pFTess).DomFaceEdgeQty[i] = Tess[TessId].DomFaceEdgeQty[i];
    (*pFTess).DomFaceEdgeNb[i] =
      ut_alloc_1d_int ((*pFTess).DomFaceEdgeQty[i] + 1);
    ut_array_1d_int_memcpy (Tess[TessId].DomFaceEdgeNb[i] + 1,
                            (*pFTess).DomFaceEdgeQty[i],
                            (*pFTess).DomFaceEdgeNb[i] + 1);
  }

  (*pFTess).DomTessFaceQty = ut_alloc_1d_int ((*pFTess).DomFaceQty + 1);
  (*pFTess).DomTessFaceNb = ut_alloc_1d_pint ((*pFTess).DomFaceQty + 1);

  for (i = 1; i <= (*pFTess).DomFaceQty; i++)
  {
    (*pFTess).DomTessFaceNb[i] = NULL;
    for (j = 1; j <= Tess[TessId].DomTessFaceQty[i]; j++)
    {
      face = Tess[TessId].DomTessFaceNb[i][j];
      (*pFTess).DomTessFaceNb[i] =
        ut_realloc_1d_int ((*pFTess).DomTessFaceNb[i],
                           (*pFTess).DomTessFaceQty[i] +
                           TessE[TessId].FaceFFaceQty[face] + 1);
      ut_array_1d_int_memcpy (TessE[TessId].FaceFFaceNb[face],
                              TessE[TessId].FaceFFaceQty[face],
                              (*pFTess).DomTessFaceNb[i] +
                              (*pFTess).DomTessFaceQty[i] + 1);
      (*pFTess).DomTessFaceQty[i] += TessE[TessId].FaceFFaceQty[face];
    }
  }

  return;
}
