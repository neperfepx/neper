/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_polys_merge_.h"

void
neut_tess_polys_merge_polylists (struct TESS *pTess, int *polys, int polyqty,
                                 int *pkeeppoly, int **pdelpolys,
                                 int *pdelpolyqty)
{
  (void) pTess;
  (*pkeeppoly) = polys[0];
  (*pdelpolyqty) = polyqty - 1;
  (*pdelpolys) = ut_alloc_1d_int (*pdelpolyqty);
  ut_array_1d_int_memcpy (polys + 1, *pdelpolyqty, *pdelpolys);

  return;
}

void
neut_tess_polys_merge_facelists (struct TESS *pTess, int *polys, int polyqty,
                                 int **pkeepfaces, int *pkeepfaceqty,
                                 int **pdelfaces, int *pdelfaceqty)
{
  int i, j, del, faceqty = 0, *faces = NULL, face;

  (*pkeepfaceqty) = 0;
  ut_free_1d_int (pkeepfaces);
  (*pdelfaceqty) = 0;
  ut_free_1d_int (pdelfaces);

  neut_tess_polys_faces (*pTess, polys, polyqty, &faces, &faceqty);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];
    del = 0;
    if ((*pTess).FaceDom[face][0] != 2)
    {
      del = 1;
      for (j = 0; j < 2; j++)
        if (ut_array_1d_int_eltpos
            (polys, polyqty, (*pTess).FacePoly[face][j]) == -1)
        {
          del = 0;
          break;
        }
    }

    if (!del)
      ut_array_1d_int_list_addval (pkeepfaces, pkeepfaceqty, face);
    else
      ut_array_1d_int_list_addval (pdelfaces, pdelfaceqty, face);
  }

  ut_free_1d_int (&faces);

  return;
}

void
neut_tess_polys_merge_edgelists (struct TESS *pTess, int *delfaces,
                                 int delfaceqty, int **pkeepedges,
                                 int *pkeepedgeqty, int **pdeledges,
                                 int *pdeledgeqty)
{
  int i, j, del, edgeqty = 0, *edges = NULL, edge;

  (*pkeepedgeqty) = 0;
  ut_free_1d_int (pkeepedges);
  (*pdeledgeqty) = 0;
  ut_free_1d_int (pdeledges);

  neut_tess_faces_edges (*pTess, delfaces, delfaceqty, &edges, &edgeqty);

  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];
    del = 0;
    if ((*pTess).EdgeDom[edge][0] != 1)
    {
      del = 1;
      for (j = 0; j < (*pTess).EdgeFaceQty[edge]; j++)
        if (ut_array_1d_int_eltpos
            (delfaces, delfaceqty, (*pTess).EdgeFaceNb[edge][j]) == -1)
        {
          del = 0;
          break;
        }
    }

    if (!del)
      ut_array_1d_int_list_addval (pkeepedges, pkeepedgeqty, edge);
    else
      ut_array_1d_int_list_addval (pdeledges, pdeledgeqty, edge);
  }

  ut_free_1d_int (&edges);

  return;
}

void
neut_tess_polys_merge_mergepolys (struct TESS *pTess, int newpoly,
                                  int *delpolys, int delpolyqty,
                                  int *keepfaces, int keepfaceqty)
{
  int i, j, pos, face, faceori, oldpoly;
  int polyqty, *polys = NULL;
  int PolyFaceQty_old = (*pTess).PolyFaceQty[newpoly];
  int *PolyFaceNb_old = ut_alloc_1d_int (PolyFaceQty_old + 1);
  int *PolyFaceOri_old = ut_alloc_1d_int (PolyFaceQty_old + 1);
  ut_array_1d_int_memcpy ((*pTess).PolyFaceNb[newpoly] + 1, PolyFaceQty_old,
                          PolyFaceNb_old + 1);
  ut_array_1d_int_memcpy ((*pTess).PolyFaceOri[newpoly] + 1, PolyFaceQty_old,
                          PolyFaceOri_old + 1);

  (*pTess).PolyFaceQty[newpoly] = 0;

  for (i = 0; i < keepfaceqty; i++)
  {
    face = keepfaces[i];

    neut_tess_face_polys (*pTess, face, &polys, &polyqty);

    if (ut_array_1d_int_eltpos (polys, polyqty, newpoly) != -1)
    {
      oldpoly = newpoly;

      pos =
        ut_array_1d_int_eltpos (PolyFaceNb_old + 1, PolyFaceQty_old + 1,
                                face);
      if (pos == -1)
        ut_print_neperbug ();

      faceori = PolyFaceOri_old[++pos];
    }

    else
    {
      faceori = 0;
      oldpoly = -1;
      for (j = 0; j < polyqty; j++)
      {
        oldpoly = polys[j];
        if (ut_array_1d_int_eltpos (delpolys, delpolyqty, oldpoly) != -1)
        {
          neut_tess_poly_face_ori (*pTess, oldpoly, face, &faceori);
          break;
        }
      }

      if (oldpoly == -1 || faceori == 0)
        ut_print_neperbug ();
    }

    ut_array_1d_int_findnreplace ((*pTess).FacePoly[face], 2, oldpoly, 0);
    neut_tess_poly_addface (pTess, newpoly, face, faceori);
  }

  for (i = 0; i < delpolyqty; i++)
    (*pTess).PolyState[delpolys[i]] = -1;

  ut_free_1d_int (&PolyFaceNb_old);
  ut_free_1d_int (&PolyFaceOri_old);
  ut_free_1d_int (&polys);

  return;
}

void
neut_tess_polys_merge_updatefaces (struct TESS *pTess, int *delfaces,
                                   int delfaceqty)
{
  int i;

  for (i = 0; i < delfaceqty; i++)
    (*pTess).FaceState[delfaces[i]] = -1;

  return;
}

void
neut_tess_polys_merge_updateedges (struct TESS *pTess, int *delfaces,
                                   int delfaceqty, int *keepedges,
                                   int keepedgeqty, int *deledges,
                                   int deledgeqty)
{
  int i, j, edge, face;

  // changing edges
  for (i = 0; i < keepedgeqty; i++)
  {
    edge = keepedges[i];
    for (j = 0; j < (*pTess).EdgeFaceQty[edge]; j++)
    {
      face = (*pTess).EdgeFaceNb[edge][j];

      if (ut_array_1d_int_eltpos (delfaces, delfaceqty, face) != -1)
        ut_array_1d_int_list_rmval ((*pTess).EdgeFaceNb + edge,
                                    (*pTess).EdgeFaceQty + edge, face);
    }
  }

  for (i = 0; i < deledgeqty; i++)
    (*pTess).EdgeState[deledges[i]] = -1;

  return;
}
