/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_edges_merge_.h"

int
neut_tess_edges_merge (struct TESS *pTess, int *edges, int edgeqty)
{
  int i, j, edge, ver, keepedge, face;
  int deledgeqty, *deledges = NULL;
  int *keepvers = NULL;
  int delverqty, *delvers = NULL;
  int *boundedges = NULL;
  int faceqty, *faces = NULL;
  int qty, *tmp = NULL;

  if (edgeqty == 1)
    return edges[0];

  boundedges = ut_alloc_1d_int (2);

  neut_tess_edges_merge_edgelists (pTess, edges, edgeqty, &keepedge,
                                   &deledges, &deledgeqty);

  neut_tess_edges_merge_verlists (pTess, edges, edgeqty, keepedge, &keepvers,
                                  &delvers, &delverqty);

  for (i = 0; i < 2; i++)
  {
    ver = keepvers[i];

    for (j = 0; j < (*pTess).VerEdgeQty[ver]; j++)
    {
      edge = (*pTess).VerEdgeNb[ver][j];

      if (ut_array_1d_int_eltpos (edges, edgeqty, edge) != -1)
      {
        boundedges[i] = edge;
        break;
      }
    }
  }

  faceqty = (*pTess).EdgeFaceQty[keepedge];
  faces = ut_alloc_1d_int (faceqty);
  int pos, keepedgeori;

  ut_array_1d_int_memcpy ((*pTess).EdgeFaceNb[keepedge], faceqty, faces);

  ut_array_1d_int_memcpy (keepvers, 2, (*pTess).EdgeVerNb[keepedge]);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    pos =
      ut_array_1d_int_eltpos ((*pTess).FaceEdgeNb[face] + 1,
                              (*pTess).FaceVerQty[face], keepedge);
    if (pos == -1)
      abort ();

    keepedgeori = (*pTess).FaceEdgeOri[face][pos + 1];

    qty = 0;
    ut_array_1d_int_list_addval (&tmp, &qty, keepedge);

    for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
    {
      edge = (*pTess).FaceEdgeNb[face][j];
      if (ut_array_1d_int_eltpos (deledges, deledgeqty, edge) == -1)
        ut_array_1d_int_list_addval (&tmp, &qty, edge);
    }

    neut_tess_face_setedges (pTess, face, tmp, qty, keepedgeori);
  }

  for (i = 0; i < 2; i++)
  {
    ver = keepvers[i];
    for (j = 0; j < 2; j++)
      ut_array_1d_int_findnreplace ((*pTess).VerEdgeNb[ver],
                                    (*pTess).VerEdgeQty[ver], boundedges[j],
                                    keepedge);
  }

  for (i = 0; i < deledgeqty; i++)
  {
    edge = deledges[i];
    (*pTess).EdgeState[edge] = -1;
  }

  for (i = 0; i < delverqty; i++)
  {
    ver = delvers[i];
    (*pTess).VerState[ver] = -1;
  }

  neut_tess_edges_merge_checkfaces (pTess, keepedge);

  ut_free_1d_int (&deledges);
  ut_free_1d_int (&keepvers);
  ut_free_1d_int (&delvers);
  ut_free_1d_int (&boundedges);
  ut_free_1d_int (&faces);
  ut_free_1d_int (&tmp);

  return keepedge;
}
