/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_merge_faces_.h"

void
neut_tess_merge_faces_facelists (struct TESS *pTess, int *faces, int faceqty,
				 int *pkeepface,
				 int **pdelfaces, int *pdelfaceqty)
{
  pTess = pTess;
  (*pkeepface) = faces[0];
  (*pdelfaceqty) = faceqty - 1;
  (*pdelfaces) = ut_alloc_1d_int (*pdelfaceqty);
  ut_array_1d_int_memcpy (*pdelfaces, *pdelfaceqty, faces + 1);

  return;
}

void
neut_tess_merge_faces_edgelists (struct TESS *pTess, int *faces, int faceqty,
				 int **pkeepedges, int *pkeepedgeqty,
				 int **pdeledges, int *pdeledgeqty)
{
  int i, j, del, edgeqty = 0, *edges = NULL, edge;

  (*pkeepedgeqty) = 0;
  ut_free_1d_int_ (pkeepedges);
  (*pdeledgeqty) = 0;
  ut_free_1d_int_ (pdeledges);

  neut_tess_faces_edges (*pTess, faces, faceqty, &edges, &edgeqty);

  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];
    del = 0;
    if ((*pTess).EdgeDom[edge][0] != 1)
    {
      del = 1;
      for (j = 0; j < (*pTess).EdgeFaceQty[edge]; j++)
	if (ut_array_1d_int_eltpos (faces, faceqty,
				    (*pTess).EdgeFaceNb[edge][j]) == -1)
	{
	  del = 0;
	  break;
	}
    }

    if (!del)
      ut_array_1d_int_list_addelt (pkeepedges, pkeepedgeqty, edge);
    else
      ut_array_1d_int_list_addelt (pdeledges, pdeledgeqty, edge);
  }

  ut_free_1d_int (edges);

  return;
}

void
neut_tess_merge_faces_verlists (struct TESS *pTess,
				int *deledges, int deledgeqty,
				int **pkeepvers, int *pkeepverqty,
				int **pdelvers, int *pdelverqty)
{
  int i, j, del, verqty = 0, *vers = NULL, ver;

  (*pkeepverqty) = 0;
  ut_free_1d_int_ (pkeepvers);
  (*pdelverqty) = 0;
  ut_free_1d_int_ (pdelvers);

  neut_tess_edges_vers (*pTess, deledges, deledgeqty, &vers, &verqty);

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];
    del = 0;
    if ((*pTess).VerDom[ver][0] != 0)
    {
      del = 1;
      for (j = 0; j < (*pTess).VerEdgeQty[ver]; j++)
	if (ut_array_1d_int_eltpos (deledges, deledgeqty,
				    (*pTess).VerEdgeNb[ver][j]) == -1)
	{
	  del = 0;
	  break;
	}
    }

    if (!del)
      ut_array_1d_int_list_addelt (pkeepvers, pkeepverqty, ver);
    else
      ut_array_1d_int_list_addelt (pdelvers, pdelverqty, ver);
  }

  ut_free_1d_int_ (&vers);

  return;
}

void
neut_tess_merge_faces_mergefaces (struct TESS *pTess, int newface,
				  int *delfaces, int delfaceqty,
				  int *keepedges, int keepedgeqty)
{
  int i, j, face, poly;

  neut_tess_face_setedges (pTess, newface, keepedges, keepedgeqty);

  for (i = 0; i < delfaceqty; i++)
    (*pTess).FaceState[delfaces[i]] = -1;

  for (i = 0; i < delfaceqty; i++)
  {
    face = delfaces[i];

    if ((*pTess).Dim == 3)
      for (j = 0; j < 2; j++)
      {
	poly = (*pTess).FacePoly[face][j];

	if (poly > 0)
	  neut_tess_poly_rmface (pTess, poly, face);
      }

    (*pTess).FaceState[face] = -1;
  }

  return;
}

void
neut_tess_merge_faces_updateedges (struct TESS *pTess, int newface,
				   int *delfaces, int delfaceqty,
				   int *keepedges, int keepedgeqty,
				   int *deledges, int deledgeqty)
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
	(*pTess).EdgeFaceNb[edge][j] = newface;
    }

    ut_array_1d_int_sort_uniq ((*pTess).EdgeFaceNb[edge],
			       (*pTess).EdgeFaceQty[edge],
			       (*pTess).EdgeFaceQty + edge);
  }

  for (i = 0; i < deledgeqty; i++)
    (*pTess).EdgeState[deledges[i]] = -1;

  return;
}

void
neut_tess_merge_faces_updatevers (struct TESS *pTess,
				  int *deledges, int deledgeqty,
				  int *keepvers, int keepverqty,
				  int *delvers, int delverqty)
{
  int i, j, ver, edge;

  // changing vers
  for (i = 0; i < keepverqty; i++)
  {
    ver = keepvers[i];

    for (j = 0; j < (*pTess).VerEdgeQty[ver]; j++)
    {
      edge = (*pTess).VerEdgeNb[ver][j];

      if (ut_array_1d_int_eltpos (deledges, deledgeqty, edge) != -1)
	(*pTess).VerEdgeQty[ver]
	  -= ut_array_1d_int_deletencompress ((*pTess).VerEdgeNb[ver],
					      (*pTess).VerEdgeQty[ver],
					      edge, 1);
    }
  }

  for (i = 0; i < delverqty; i++)
    (*pTess).VerState[delvers[i]] = -1;

  return;
}
