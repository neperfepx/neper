/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_faces_merge_.h"

void
neut_tess_faces_merge_facelists (struct TESS *pTess, int *faces, int faceqty,
                                 int *pkeepface, int **pdelfaces,
                                 int *pdelfaceqty)
{
  int i, j;
  int *nfaces = NULL, nfaceqty = 0;

  (*pdelfaceqty) = faceqty;
  (*pdelfaces) = ut_alloc_1d_int (*pdelfaceqty);
  ut_array_1d_int_memcpy (*pdelfaces, *pdelfaceqty, faces);

  // looking for a face which is not surrounded by delfaces; this will be
  // useful to determine the edge orientations.
  (*pkeepface) = -1;
  for (i = 0; i < faceqty; i++)
  {
    neut_tess_face_neighfaces (*pTess, faces[i], &nfaces, &nfaceqty);

    for (j = 0; j < nfaceqty; j++)
      if (ut_array_1d_int_eltpos (faces, faceqty, nfaces[j]) == -1)
      {
        (*pkeepface) = faces[i];
        break;
      }

    ut_free_1d_int_ (&nfaces);

    if (*pkeepface != -1)
      break;
  }

  if ((*pkeepface) == -1)
    abort ();

  ut_array_1d_int_list_rmelt (pdelfaces, pdelfaceqty, *pkeepface);

  return;
}

void
neut_tess_faces_merge_edgelists (struct TESS *pTess, int *faces, int faceqty,
                                 int keepface, int **pkeepedges,
                                 int *pkeepedgeqty, int *pfirstkeepedgeori,
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
    {
      ut_array_1d_int_list_addelt (pkeepedges, pkeepedgeqty, edge);
      ut_array_1d_int_list_addelt (pkeepedges, pkeepedgeqty, edge);
    }
    else
      ut_array_1d_int_list_addelt (pdeledges, pdeledgeqty, edge);
  }

  int pos, found = 0;
  for (i = 0; i < *pkeepedgeqty; i++)
  {
    pos = ut_array_1d_int_eltpos ((*pTess).FaceEdgeNb[keepface] + 1,
                                  (*pTess).FaceVerQty[keepface],
                                  (*pkeepedges)[i]);

    if (pos != -1)
    {
      ut_array_1d_int_switch (*pkeepedges, 0, i);
      (*pfirstkeepedgeori) = (*pTess).FaceEdgeOri[keepface][pos + 1];
      found = 1;
      break;
    }
  }

  if (!found)
    abort ();

  ut_free_1d_int (edges);

  return;
}

void
neut_tess_faces_merge_verlists (struct TESS *pTess,
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
neut_tess_faces_merge_mergefaces (struct TESS *pTess, int newface,
				  int *delfaces, int delfaceqty,
				  int *keepedges, int keepedgeqty,
                                  int firstkeepedgeori)
{
  int i, j, delface, poly;
  double *n = ut_alloc_1d (3);

  neut_tess_face_setedges (pTess, newface, keepedges, keepedgeqty,
                           firstkeepedgeori);

  // we must make sure that the face keeps the same orientation.
  // This is done by recording the old orientation, and later correcting the
  // new orientation so that it points in a similar direction.

  ut_array_1d_memcpy (n, 3, (*pTess).FaceEq[newface] + 1);

  if (!neut_tess_face_iscurved (*pTess, newface))
    neut_tess_face_eq_fromvers (*pTess, newface, (*pTess).FaceEq[newface]);
  else
    neut_tess_faceinter_fromdomface (pTess, newface);

  // flipping normal, if necessary

  if (ut_vector_scalprod ((*pTess).FaceEq[newface] + 1, n) < 0)
    ut_array_1d_scale ((*pTess).FaceEq[newface], 4, -1);

  // removing from domface

  for (i = 0; i < delfaceqty; i++)
  {
    delface = delfaces[i];
    if ((*pTess).FaceDom[delface][0] == 2)
      neut_tess_domface_rmface (pTess, (*pTess).FaceDom[delface][1], delface);
  }

  // removing from poly

  if ((*pTess).Dim == 3)
    for (i = 0; i < delfaceqty; i++)
    {
      delface = delfaces[i];

      for (j = 0; j < 2; j++)
      {
        poly = (*pTess).FacePoly[delface][j];

        if (poly > 0)
          neut_tess_poly_rmface (pTess, poly, delface);
      }
    }

  // updating state

  for (i = 0; i < delfaceqty; i++)
    (*pTess).FaceState[delfaces[i]] = -1;

  ut_free_1d (n);

  return;
}

void
neut_tess_faces_merge_updateedges (struct TESS *pTess, int newface,
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
neut_tess_faces_merge_updatevers (struct TESS *pTess,
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
