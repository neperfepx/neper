/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_tess_.h"

/* neut_tess_tess initializing the properties of the vertices ******************* */

void
neut_tess_tess_seedcoo (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  if (TessA.SeedCoo)
  {
    (*pTessB).SeedCoo = ut_alloc_2d ((*pTessB).SeedQty + 1, 3);

    for (i = 1; i <= (*pTessB).SeedQty; i++)
      for (j = 0; j < (*pTessB).Dim; j++)
        (*pTessB).SeedCoo[i][j] = TessA.SeedCoo[i][j];
  }

  return;
}

void
neut_tess_tess_seedweight (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  if (TessA.SeedWeight)
  {
    (*pTessB).SeedWeight = ut_alloc_1d ((*pTessB).SeedQty + 1);

    for (i = 1; i <= (*pTessB).SeedQty; i++)
      (*pTessB).SeedWeight[i] = TessA.SeedWeight[i];
  }

  return;
}

void
neut_tess_tess_verqty (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).VerQty = TessA.VerQty;

  return;
}

void
neut_tess_tess_vercoo (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).VerCoo = ut_alloc_2d ((*pTessB).VerQty + 1, 3);

  for (i = 1; i <= (*pTessB).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTessB).VerCoo[i][j] = TessA.VerCoo[i][j];

  return;
}

void
neut_tess_tess_veredge (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).VerEdgeQty = ut_alloc_1d_int ((*pTessB).VerQty + 1);

  (*pTessB).VerEdgeNb = ut_alloc_1d_pint ((*pTessB).VerQty + 1);

  for (i = 1; i <= (*pTessB).VerQty; i++)
  {
    (*pTessB).VerEdgeQty[i] = TessA.VerEdgeQty[i];

    (*pTessB).VerEdgeNb[i] = ut_alloc_1d_int ((*pTessB).VerEdgeQty[i]);

    for (j = 0; j < (*pTessB).VerEdgeQty[i]; j++)
      (*pTessB).VerEdgeNb[i][j] = TessA.VerEdgeNb[i][j];
  }

  return;
}

void
neut_tess_tess_verbound (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).VerDom = ut_alloc_2d_int ((*pTessB).VerQty + 1, 2);
  ut_array_2d_int_memcpy (TessA.VerDom + 1, (*pTessB).VerQty, 2,
                          (*pTessB).VerDom + 1);

  return;
}

void
neut_tess_tess_verstate (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).VerState = ut_alloc_1d_int ((*pTessB).VerQty + 1);

  for (i = 1; i <= (*pTessB).VerQty; i++)
    (*pTessB).VerState[i] = TessA.VerState[i];

  return;
}

/* Initializing the properties of the edges ******************* */

void
neut_tess_tess_edgeqty (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).EdgeQty = TessA.EdgeQty;

  return;
}

void
neut_tess_tess_edgevernb (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).EdgeVerNb = ut_alloc_2d_int ((*pTessB).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTessB).EdgeQty; i++)
    for (j = 0; j < 2; j++)
      (*pTessB).EdgeVerNb[i][j] = TessA.EdgeVerNb[i][j];

  return;
}

void
neut_tess_tess_edgeface (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).EdgeFaceQty = ut_alloc_1d_int ((*pTessB).EdgeQty + 1);
  (*pTessB).EdgeFaceNb = ut_alloc_1d_pint ((*pTessB).EdgeQty + 1);

  for (i = 1; i <= TessA.EdgeQty; i++)
  {
    (*pTessB).EdgeFaceQty[i] = TessA.EdgeFaceQty[i];

    (*pTessB).EdgeFaceNb[i] = ut_alloc_1d_int ((*pTessB).EdgeFaceQty[i] + 1);

    for (j = 0; j < (*pTessB).EdgeFaceQty[i]; j++)
      (*pTessB).EdgeFaceNb[i][j] = TessA.EdgeFaceNb[i][j];
  }

  return;
}

void
neut_tess_tess_edgelength (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  if (TessA.EdgeLength)
  {
    (*pTessB).EdgeLength = ut_alloc_1d ((*pTessB).EdgeQty + 1);

    for (i = 1; i <= (*pTessB).EdgeQty; i++)
      (*pTessB).EdgeLength[i] = TessA.EdgeLength[i];
  }

  return;
}

void
neut_tess_tess_edgebound (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).EdgeDom = ut_alloc_2d_int ((*pTessB).EdgeQty + 1, 2);

  ut_array_2d_int_memcpy (TessA.EdgeDom + 1, (*pTessB).EdgeQty, 2,
                          (*pTessB).EdgeDom + 1);

  return;
}

void
neut_tess_tess_edgedel (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).EdgeDel = ut_alloc_1d_int ((*pTessB).EdgeQty + 1);

  for (i = 1; i <= (*pTessB).EdgeQty; i++)
    (*pTessB).EdgeDel[i] = TessA.EdgeDel[i];

  return;
}

void
neut_tess_tess_edgestate (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).EdgeState = ut_alloc_1d_int ((*pTessB).EdgeQty + 1);

  for (i = 1; i <= (*pTessB).EdgeQty; i++)
    (*pTessB).EdgeState[i] = TessA.EdgeState[i];

  return;
}

/* neut_tess_tesstializing the properties of the faces ******************* */

void
neut_tess_tess_faceqty (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).FaceQty = TessA.FaceQty;

  return;
}

void
neut_tess_tess_facepoly (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FacePoly = ut_alloc_2d_int ((*pTessB).FaceQty + 1, 2);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
    for (j = 0; j < 2; j++)
      (*pTessB).FacePoly[i][j] = TessA.FacePoly[i][j];

  return;
}

void
neut_tess_tess_facever (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FaceVerQty = ut_alloc_1d_int ((*pTessB).FaceQty + 1);
  (*pTessB).FaceVerNb = ut_alloc_1d_pint ((*pTessB).FaceQty + 1);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
  {
    (*pTessB).FaceVerQty[i] = TessA.FaceVerQty[i];

    (*pTessB).FaceVerNb[i] = ut_alloc_1d_int ((*pTessB).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTessB).FaceVerQty[i]; j++)
      (*pTessB).FaceVerNb[i][j] = TessA.FaceVerNb[i][j];
  }

  return;
}

/* a preliminar neut_tess_tess_faceVer is needed! */
void
neut_tess_tess_faceedge (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FaceEdgeNb = ut_alloc_1d_pint ((*pTessB).FaceQty + 1);
  (*pTessB).FaceEdgeOri = ut_alloc_1d_pint ((*pTessB).FaceQty + 1);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
  {
    (*pTessB).FaceEdgeNb[i] = ut_alloc_1d_int ((*pTessB).FaceVerQty[i] + 1);
    (*pTessB).FaceEdgeOri[i] = ut_alloc_1d_int ((*pTessB).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTessB).FaceVerQty[i]; j++)
    {
      (*pTessB).FaceEdgeNb[i][j] = TessA.FaceEdgeNb[i][j];
      (*pTessB).FaceEdgeOri[i][j] = TessA.FaceEdgeOri[i][j];
    }
  }

  return;
}

void
neut_tess_tess_faceeq (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FaceEq = ut_alloc_2d ((*pTessB).FaceQty + 1, 4);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
    for (j = 0; j < 4; j++)
      (*pTessB).FaceEq[i][j] = TessA.FaceEq[i][j];

  return;
}

void
neut_tess_tess_facebound (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).FaceDom = ut_alloc_2d_int ((*pTessB).FaceQty + 1, 2);

  ut_array_2d_int_memcpy (TessA.FaceDom + 1, (*pTessB).FaceQty, 2,
                          (*pTessB).FaceDom + 1);

  return;
}

void
neut_tess_tess_facestate (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).FaceState = ut_alloc_1d_int ((*pTessB).FaceQty + 1);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
    (*pTessB).FaceState[i] = TessA.FaceState[i];

  return;
}

void
neut_tess_tess_facept (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FacePt = ut_alloc_1d_int ((*pTessB).FaceQty + 1);
  (*pTessB).FacePtCoo = ut_alloc_2d ((*pTessB).FaceQty + 1, 3);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
  {
    (*pTessB).FacePt[i] = TessA.FacePt[i];
    for (j = 0; j < 3; j++)
      (*pTessB).FacePtCoo[i][j] = TessA.FacePtCoo[i][j];
  }

  return;
}

/* neut_tess_tesstializing the properties of the faces ******************* */

void
neut_tess_tess_polyqty (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).PolyQty = TessA.PolyQty;

  return;
}

void
neut_tess_tess_polystate (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).PolyState = ut_alloc_1d_int ((*pTessB).PolyQty + 1);

  for (i = 1; i <= (*pTessB).PolyQty; i++)
    (*pTessB).PolyState[i] = TessA.PolyState ? TessA.PolyState[i] : 0;

  return;
}

void
neut_tess_tess_polyface (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).PolyFaceQty = ut_alloc_1d_int ((*pTessB).PolyQty + 1);
  (*pTessB).PolyFaceNb = ut_alloc_1d_pint ((*pTessB).PolyQty + 1);
  (*pTessB).PolyFaceOri = ut_alloc_1d_pint ((*pTessB).PolyQty + 1);

  for (i = 1; i <= (*pTessB).PolyQty; i++)
  {
    (*pTessB).PolyFaceQty[i] = TessA.PolyFaceQty[i];

    (*pTessB).PolyFaceNb[i] = ut_alloc_1d_int ((*pTessB).PolyFaceQty[i] + 1);
    (*pTessB).PolyFaceOri[i] = ut_alloc_1d_int ((*pTessB).PolyFaceQty[i] + 1);

    for (j = 1; j <= (*pTessB).PolyFaceQty[i]; j++)
    {
      (*pTessB).PolyFaceNb[i][j] = TessA.PolyFaceNb[i][j];
      (*pTessB).PolyFaceOri[i][j] = TessA.PolyFaceOri[i][j];
    }
  }

  return;
}

void
neut_tess_tess_celllamid (struct TESS TessA, struct TESS *pTessB)
{
  if (!TessA.CellLamId)
    return;

  (*pTessB).CellLamId = ut_alloc_1d_int (TessA.CellQty + 1);
  ut_array_1d_int_memcpy (TessA.CellLamId + 1, TessA.CellQty,
                          (*pTessB).CellLamId + 1);

  return;
}

void
neut_tess_tess_cellmodeid (struct TESS TessA, struct TESS *pTessB)
{
  if (!TessA.CellModeId)
    return;

  (*pTessB).CellModeId = ut_alloc_1d_int (TessA.CellQty + 1);
  ut_array_1d_int_memcpy (TessA.CellModeId + 1, TessA.CellQty,
                          (*pTessB).CellModeId + 1);

  return;
}

void
neut_tess_tess_cellgroupid (struct TESS TessA, struct TESS *pTessB)
{
  if (!TessA.CellGroup)
    return;

  (*pTessB).CellGroup = ut_alloc_1d_int (TessA.CellQty + 1);
  ut_array_1d_int_memcpy (TessA.CellGroup + 1, TessA.CellQty,
                          (*pTessB).CellGroup + 1);

  return;
}

void
neut_tess_tess_cellbody (struct TESS TessA, struct TESS *pTessB)
{
  if (!TessA.CellBodyQty)
    return;

  (*pTessB).CellBodyQty = TessA.CellBodyQty;

  (*pTessB).CellBody = ut_alloc_2d_int (TessA.CellBodyQty, TessA.CellQty + 1);
  ut_array_2d_int_memcpy (TessA.CellBody, TessA.CellBodyQty, TessA.CellQty + 1,
                          (*pTessB).CellBody);

  return;
}

void
neut_tess_tess_domaindata (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  if (TessA.DomType)
    ut_string_string (TessA.DomType, &((*pTessB).DomType));

  // ver

  (*pTessB).DomVerQty = TessA.DomVerQty;
  (*pTessB).DomVerCoo = ut_alloc_2d ((*pTessB).DomVerQty + 1, 3);
  (*pTessB).DomVerLabel = ut_alloc_1d_pchar ((*pTessB).DomVerQty + 1);
  ut_array_1d_pchar_memcpy (TessA.DomVerLabel + 1, (*pTessB).DomVerQty,
                            (*pTessB).DomVerLabel + 1);
  ut_array_2d_memcpy (TessA.DomVerCoo + 1, (*pTessB).DomVerQty, 3,
                      (*pTessB).DomVerCoo + 1);

  (*pTessB).DomTessVerNb = ut_alloc_1d_int ((*pTessB).DomVerQty + 1);
  ut_array_1d_int_memcpy (TessA.DomTessVerNb + 1, (*pTessB).DomVerQty,
                          (*pTessB).DomTessVerNb + 1);

  (*pTessB).DomVerEdgeQty = ut_alloc_1d_int ((*pTessB).DomVerQty + 1);
  (*pTessB).DomVerEdgeNb = ut_alloc_1d_pint ((*pTessB).DomVerQty + 1);

  if (TessA.Dim >= 2)
  {
    ut_array_1d_int_memcpy (TessA.DomVerEdgeQty + 1, (*pTessB).DomVerQty,
                            (*pTessB).DomVerEdgeQty + 1);

    for (i = 1; i <= (*pTessB).DomVerQty; i++)
      if (TessA.DomVerEdgeQty[i] > 0)
      {
        (*pTessB).DomVerEdgeNb[i] =
          ut_alloc_1d_int ((*pTessB).DomVerEdgeQty[i]);
        ut_array_1d_int_memcpy (TessA.DomVerEdgeNb[i],
                                (*pTessB).DomVerEdgeQty[i],
                                (*pTessB).DomVerEdgeNb[i]);
      }
  }

  // edge

  (*pTessB).DomEdgeQty = TessA.DomEdgeQty;
  (*pTessB).DomEdgeLabel = ut_alloc_1d_pchar ((*pTessB).DomEdgeQty + 1);
  ut_array_1d_pchar_memcpy (TessA.DomEdgeLabel + 1, (*pTessB).DomEdgeQty,
                            (*pTessB).DomEdgeLabel + 1);

  (*pTessB).DomEdgeVerQty = ut_alloc_1d_int ((*pTessB).DomEdgeQty + 1);
  ut_array_1d_int_memcpy (TessA.DomEdgeVerQty + 1, (*pTessB).DomEdgeQty,
                          (*pTessB).DomEdgeVerQty + 1);

  (*pTessB).DomEdgeVerNb = ut_alloc_2d_int ((*pTessB).DomEdgeQty + 1, 2);
  ut_array_2d_int_memcpy (TessA.DomEdgeVerNb + 1, (*pTessB).DomEdgeQty, 2,
                          (*pTessB).DomEdgeVerNb + 1);

  if ((*pTessB).Dim == 3)
  {
    (*pTessB).DomEdgeFaceNb = ut_alloc_2d_int ((*pTessB).DomEdgeQty + 1, 2);
    ut_array_2d_int_memcpy (TessA.DomEdgeFaceNb + 1, (*pTessB).DomEdgeQty, 2,
                            (*pTessB).DomEdgeFaceNb + 1);
  }

  (*pTessB).DomTessEdgeQty = ut_alloc_1d_int ((*pTessB).DomEdgeQty + 1);
  (*pTessB).DomTessEdgeNb = ut_alloc_1d_pint ((*pTessB).DomEdgeQty + 1);

  ut_array_1d_int_memcpy (TessA.DomTessEdgeQty + 1, (*pTessB).DomEdgeQty,
                          (*pTessB).DomTessEdgeQty + 1);

  for (i = 1; i <= (*pTessB).DomEdgeQty; i++)
    if ((*pTessB).DomTessEdgeQty[i] > 0)
    {
      (*pTessB).DomTessEdgeNb[i] =
        ut_alloc_1d_int ((*pTessB).DomTessEdgeQty[i] + 1);
      ut_array_1d_int_memcpy (TessA.DomTessEdgeNb[i] + 1,
                              (*pTessB).DomTessEdgeQty[i],
                              (*pTessB).DomTessEdgeNb[i] + 1);
    }

  (*pTessB).DomEdgeType = ut_alloc_1d_pchar ((*pTessB).DomEdgeQty + 1);
  for (i = 1; i <= (*pTessB).DomEdgeQty; i++)
    ut_string_string (TessA.DomEdgeType[i], (*pTessB).DomEdgeType + i);

  (*pTessB).DomEdgeParmQty = ut_alloc_1d_int ((*pTessB).DomEdgeQty + 1);
  (*pTessB).DomEdgeParms = ut_alloc_1d_pdouble ((*pTessB).DomEdgeQty + 1);
  for (i = 1; i <= (*pTessB).DomEdgeQty; i++)
  {
    (*pTessB).DomEdgeParmQty[i] = TessA.DomEdgeParmQty[i];
    if ((*pTessB).DomEdgeParmQty[i] > 0)
    {
      (*pTessB).DomEdgeParms[i] = ut_alloc_1d ((*pTessB).DomEdgeParmQty[i]);
      ut_array_1d_memcpy (TessA.DomEdgeParms[i], (*pTessB).DomEdgeParmQty[i],
                          (*pTessB).DomEdgeParms[i]);
    }
  }

  // face

  (*pTessB).DomFaceQty = TessA.DomFaceQty;

  (*pTessB).DomFaceEq = ut_alloc_2d ((*pTessB).DomFaceQty + 1, 4);
  ut_array_2d_memcpy (TessA.DomFaceEq + 1, (*pTessB).DomFaceQty, 4,
                      (*pTessB).DomFaceEq + 1);

  (*pTessB).DomFaceType = ut_alloc_1d_pchar ((*pTessB).DomFaceQty + 1);
  for (i = 1; i <= (*pTessB).DomFaceQty; i++)
    ut_string_string (TessA.DomFaceType[i], (*pTessB).DomFaceType + i);

  (*pTessB).DomFaceParmQty = ut_alloc_1d_int ((*pTessB).DomFaceQty + 1);
  (*pTessB).DomFaceParms = ut_alloc_1d_pdouble ((*pTessB).DomFaceQty + 1);
  for (i = 1; i <= (*pTessB).DomFaceQty; i++)
  {
    (*pTessB).DomFaceParmQty[i] = TessA.DomFaceParmQty[i];
    if ((*pTessB).DomFaceParmQty[i] > 0)
    {
      (*pTessB).DomFaceParms[i] = ut_alloc_1d ((*pTessB).DomFaceParmQty[i]);
      ut_array_1d_memcpy (TessA.DomFaceParms[i], (*pTessB).DomFaceParmQty[i],
                          (*pTessB).DomFaceParms[i]);
    }
  }

  (*pTessB).DomFaceLabel = ut_alloc_1d_pchar ((*pTessB).DomFaceQty + 1);
  ut_array_1d_pchar_memcpy (TessA.DomFaceLabel + 1, (*pTessB).DomFaceQty,
                            (*pTessB).DomFaceLabel + 1);

  (*pTessB).DomTessFaceQty = ut_alloc_1d_int ((*pTessB).DomFaceQty + 1);
  ut_array_1d_int_memcpy (TessA.DomTessFaceQty + 1, (*pTessB).DomFaceQty,
                          (*pTessB).DomTessFaceQty + 1);

  (*pTessB).DomTessFaceNb = ut_alloc_1d_pint ((*pTessB).DomFaceQty + 1);

  for (i = 1; i <= (*pTessB).DomFaceQty; i++)
    if ((*pTessB).DomTessFaceQty[i] > 0)
    {
      (*pTessB).DomTessFaceNb[i] =
        ut_alloc_1d_int ((*pTessB).DomTessFaceQty[i] + 1);
      ut_array_1d_int_memcpy (TessA.DomTessFaceNb[i] + 1,
                              (*pTessB).DomTessFaceQty[i],
                              (*pTessB).DomTessFaceNb[i] + 1);
    }

  (*pTessB).DomFaceVerQty = ut_alloc_1d_int ((*pTessB).DomFaceQty + 1);
  (*pTessB).DomFaceVerNb = ut_alloc_1d_pint ((*pTessB).DomFaceQty + 1);
  (*pTessB).DomFaceEdgeQty = ut_alloc_1d_int ((*pTessB).DomFaceQty + 1);
  (*pTessB).DomFaceEdgeNb = ut_alloc_1d_pint ((*pTessB).DomFaceQty + 1);

  ut_array_1d_int_memcpy (TessA.DomFaceVerQty + 1, (*pTessB).DomFaceQty,
                          (*pTessB).DomFaceVerQty + 1);

  ut_array_1d_int_memcpy (TessA.DomFaceEdgeQty + 1, (*pTessB).DomFaceQty,
                          (*pTessB).DomFaceEdgeQty + 1);

  for (i = 1; i <= (*pTessB).DomFaceQty; i++)
  {
    (*pTessB).DomFaceVerNb[i] =
      ut_alloc_1d_int ((*pTessB).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy (TessA.DomFaceVerNb[i] + 1,
                            (*pTessB).DomFaceVerQty[i],
                            (*pTessB).DomFaceVerNb[i] + 1);

    (*pTessB).DomFaceEdgeNb[i] =
      ut_alloc_1d_int ((*pTessB).DomFaceEdgeQty[i] + 1);
    ut_array_1d_int_memcpy (TessA.DomFaceEdgeNb[i] + 1,
                            (*pTessB).DomFaceEdgeQty[i],
                            (*pTessB).DomFaceEdgeNb[i] + 1);
  }

  return;
}
