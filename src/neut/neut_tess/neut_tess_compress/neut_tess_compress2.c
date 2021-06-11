/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_compress_.h"

/* This function deletes the dummy polys in a Tess */
void
neut_tess_compress_polys (struct TESS *pTess)
{
  int i, pos;

  pos = 0;
  for (i = 1; i <= (*pTess).PolyQty; i++)
    if ((*pTess).PolyState[i] != -1)
      if (++pos < i)
        neut_tess_compress_movepoly (pTess, i, pos);

  // realloc'ing
  (*pTess).PolyFaceQty = ut_realloc_1d_int ((*pTess).PolyFaceQty, pos + 1);
  (*pTess).PolyFaceNb =
    ut_realloc_2d_int_delline ((*pTess).PolyFaceNb, (*pTess).PolyQty + 1,
                               pos + 1);
  (*pTess).PolyFaceOri =
    ut_realloc_2d_int_delline ((*pTess).PolyFaceOri, (*pTess).PolyQty + 1,
                               pos + 1);
  (*pTess).PolyState = ut_realloc_1d_int ((*pTess).PolyState, pos + 1);

  (*pTess).PolyQty = pos;
  (*pTess).CellQty = pos;
  (*pTess).SeedQty = pos;

  return;
}

/* This function deletes the dummy faces in a Tess */
void
neut_tess_compress_faces (struct TESS *pTess)
{
  int i, pos;

  pos = 0;
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FaceState[i] != -1)
      if (++pos < i)
        neut_tess_compress_moveface (pTess, i, pos);

  // realloc'ing
  if ((*pTess).Dim == 3)
    (*pTess).FacePoly =
      ut_realloc_2d_int_delline ((*pTess).FacePoly, (*pTess).FaceQty + 1,
                                 pos + 1);
  (*pTess).FaceEq =
    ut_realloc_2d_delline ((*pTess).FaceEq, (*pTess).FaceQty + 1, pos + 1);
  (*pTess).FaceVerQty = ut_realloc_1d_int ((*pTess).FaceVerQty, pos + 1);
  (*pTess).FaceVerNb =
    ut_realloc_2d_int_delline ((*pTess).FaceVerNb, (*pTess).FaceQty + 1,
                               pos + 1);
  (*pTess).FaceEdgeNb =
    ut_realloc_2d_int_delline ((*pTess).FaceEdgeNb, (*pTess).FaceQty + 1,
                               pos + 1);
  (*pTess).FaceEdgeOri =
    ut_realloc_2d_int_delline ((*pTess).FaceEdgeOri, (*pTess).FaceQty + 1,
                               pos + 1);
  (*pTess).FaceState = ut_realloc_1d_int ((*pTess).FaceState, pos + 1);
  (*pTess).FacePt = ut_realloc_1d_int ((*pTess).FacePt, pos + 1);
  (*pTess).FacePtCoo =
    ut_realloc_2d_delline ((*pTess).FacePtCoo, (*pTess).FaceQty + 1, pos + 1);

  if ((*pTess).Dim == 3)
    (*pTess).FaceDom =
      ut_realloc_2d_int_delline ((*pTess).FaceDom, (*pTess).FaceQty + 1,
                                 pos + 1);

  (*pTess).FaceQty = pos;
  if ((*pTess).Dim == 2)
  {
    (*pTess).CellQty = pos;
    (*pTess).SeedQty = pos;
  }

  if ((*pTess).DomFaceQty > 0)
    neut_tess_init_domtessface (pTess);

  return;
}

/* This function deletes the dummy edges in a Tess */
void
neut_tess_compress_edges (struct TESS *pTess)
{
  int i, pos;

  pos = 0;
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if ((*pTess).EdgeState[i] == 0)
      if (++pos < i)
        neut_tess_compress_moveedge (pTess, i, pos);

  // realloc'ing
  (*pTess).EdgeVerNb =
    ut_realloc_2d_int_delline ((*pTess).EdgeVerNb, (*pTess).EdgeQty + 1,
                               pos + 1);
  (*pTess).EdgeFaceQty = ut_realloc_1d_int ((*pTess).EdgeFaceQty, pos + 1);
  (*pTess).EdgeFaceNb =
    ut_realloc_2d_int_delline ((*pTess).EdgeFaceNb, (*pTess).EdgeQty + 1,
                               pos + 1);
  (*pTess).EdgeLength = ut_realloc_1d ((*pTess).EdgeLength, pos + 1);
  (*pTess).EdgeState = ut_realloc_1d_int ((*pTess).EdgeState, pos + 1);
  (*pTess).EdgeDel = ut_realloc_1d_int ((*pTess).EdgeDel, pos + 1);
  (*pTess).EdgeDom =
    ut_realloc_2d_int_delline ((*pTess).EdgeDom, (*pTess).EdgeQty + 1,
                               pos + 1);

  (*pTess).EdgeQty = pos;

  if ((*pTess).DomEdgeQty > 0)
    neut_tess_init_domtessedge (pTess);

  return;
}

/* This function deletes the dummy vertices in a Tess */
void
neut_tess_compress_vers (struct TESS *pTess)
{
  int i, pos;

  pos = 0;
  for (i = 1; i <= (*pTess).VerQty; i++)
    if ((*pTess).VerState[i] != -1)
      if (++pos < i)
        neut_tess_compress_movever (pTess, i, pos);

  // realloc'ing
  (*pTess).VerCoo =
    ut_realloc_2d_delline ((*pTess).VerCoo, (*pTess).VerQty + 1, pos + 1);
  (*pTess).VerEdgeQty = ut_realloc_1d_int ((*pTess).VerEdgeQty, pos + 1);
  (*pTess).VerEdgeNb =
    ut_realloc_2d_int_delline ((*pTess).VerEdgeNb, (*pTess).VerQty + 1,
                               pos + 1);
  (*pTess).VerDom =
    ut_realloc_2d_int_delline ((*pTess).VerDom, (*pTess).VerQty + 1, pos + 1);
  (*pTess).VerState = ut_realloc_1d_int ((*pTess).VerState, pos + 1);

  (*pTess).VerQty = pos;

  if ((*pTess).DomVerQty > 0)
    neut_tess_init_domtessver (pTess);


  return;
}

void
neut_tess_compress_domfaces (struct TESS *pTess)
{
  int i, domfaceqty, *domface_old_new = NULL;

  if ((*pTess).Dim == 2)
    return;

  domface_old_new = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);

  domfaceqty = 0;
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    if ((*pTess).DomTessFaceQty[i] > 0)
      domface_old_new[i] = ++domfaceqty;

  if (domfaceqty < (*pTess).DomFaceQty)
    for (i = 1; i <= (*pTess).DomFaceQty; i++)
      if ((*pTess).DomTessFaceQty[i] > 0)
        neut_tess_compress_movedomface (pTess, i, domface_old_new[i]);

  (*pTess).DomFaceQty = domfaceqty;

  ut_free_1d_int (&domface_old_new);

  return;
}

void
neut_tess_compress_domedges (struct TESS *pTess)
{
  int i, domedgeqty, *domedge_old_new = NULL;

  if ((*pTess).Dim == 2)
    return;

  domedge_old_new = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);

  domedgeqty = 0;
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    if ((*pTess).DomTessEdgeQty[i] > 0)
      domedge_old_new[i] = ++domedgeqty;

  if (domedgeqty < (*pTess).DomEdgeQty)
    for (i = 1; i <= (*pTess).DomEdgeQty; i++)
      if ((*pTess).DomTessEdgeQty[i] > 0)
        neut_tess_compress_movedomedge (pTess, i, domedge_old_new[i]);

  (*pTess).DomEdgeQty = domedgeqty;

  ut_free_1d_int (&domedge_old_new);

  return;
}

void
neut_tess_compress_domvers (struct TESS *pTess)
{
  int i, domverqty, *domver_old_new = NULL;

  if ((*pTess).Dim == 2)
    return;

  domver_old_new = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);

  domverqty = 0;
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    if ((*pTess).DomTessEdgeQty[i] > 0)
      domver_old_new[i] = ++domverqty;

  if (domverqty < (*pTess).DomEdgeQty)
    for (i = 1; i <= (*pTess).DomEdgeQty; i++)
      if ((*pTess).DomTessEdgeQty[i] > 0)
        neut_tess_compress_movedomver (pTess, i, domver_old_new[i]);

  (*pTess).DomEdgeQty = domverqty;

  ut_free_1d_int (&domver_old_new);

  return;
}
