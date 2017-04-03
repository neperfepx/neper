/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_merge_edges_.h"

void
neut_tess_merge_edges (struct TESS *pTess, int *edges, int edgeqty)
{
  int i, j, edge, ver, newedge, face;
  int deledgeqty, *deledges = NULL;
  int verqty, *vers = NULL;
  int keepverqty, *keepvers = NULL;
  int delverqty, *delvers = NULL;
  int veredgeqty;
  int *boundedges = ut_alloc_1d_int (2);
  int faceqty, *faces = NULL;
  int qty, *tmp = NULL;

  // recording edges to del
  newedge = edges[0];
  newedge = newedge;
  deledgeqty = edgeqty - 1;
  deledges = ut_alloc_1d_int (deledgeqty);
  ut_array_1d_int_memcpy (deledges, deledgeqty, edges + 1);

  // finding vertices to keep and vertices to del
  keepverqty = 0;
  delverqty = 0;
  neut_tess_edges_vers (*pTess, edges, edgeqty, &vers, &verqty);

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];
    veredgeqty = 0;
    for (j = 0; j < (*pTess).VerEdgeQty[ver]; j++)
    {
      edge = (*pTess).VerEdgeNb[ver][j];
      if (ut_array_1d_int_eltpos (edges, edgeqty, edge) != -1)
	veredgeqty++;
    }

    if (veredgeqty == 1)
      ut_array_1d_int_list_addelt (&keepvers, &keepverqty, ver);
    else if (veredgeqty == 2)
      ut_array_1d_int_list_addelt (&delvers, &delverqty, ver);
    else
      ut_error_reportbug ();
  }

  if (keepverqty != 2)
    ut_error_reportbug ();

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

  faceqty = (*pTess).EdgeFaceQty[newedge];
  faces = ut_alloc_1d_int (faceqty);

  ut_array_1d_int_memcpy (faces, faceqty, (*pTess).EdgeFaceNb[newedge]);

  ut_array_1d_int_memcpy ((*pTess).EdgeVerNb[newedge], 2, keepvers);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    qty = 0;
    ut_array_1d_int_list_addelt (&tmp, &qty, newedge);

    for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
    {
      edge = (*pTess).FaceEdgeNb[face][j];
      if (ut_array_1d_int_eltpos (deledges, deledgeqty, edge) == -1)
	ut_array_1d_int_list_addelt (&tmp, &qty, edge);
    }

    neut_tess_face_setedges (pTess, face, tmp, qty);
  }

  for (i = 0; i < keepverqty; i++)
  {
    ver = keepvers[i];
    for (j = 0; j < 2; j++)
      ut_array_1d_int_findnreplace ((*pTess).VerEdgeNb[ver],
				    (*pTess).VerEdgeQty[ver], boundedges[j],
				    newedge);
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

  ut_free_1d_int (deledges);
  ut_free_1d_int (vers);
  ut_free_1d_int (keepvers);
  ut_free_1d_int (delvers);
  ut_free_1d_int (boundedges);
  ut_free_1d_int (faces);
  ut_free_1d_int (tmp);

  return;
}
