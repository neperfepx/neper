/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_face_.h"

void
net_mtess_flatten_face_domface_dom (struct MTESS MTess, struct TESS *Tess,
			      int TessId, int dface,
			      struct TESS *pFTess, struct TESSE **pTessE,
			      int *dom, struct FLATTEN *pFlatten)
{
  int i, j, k, edge, face, ori, poly, newface, fedge, fori;
  int *tessface = ut_alloc_1d_int (2);

  // poly in tess L1, tess and domain face in tess L2
  poly = Tess[TessId].FacePoly[dface][0];
  tessface[0] = MTess.DomTess[TessId][poly];
  tessface[1] = MTess.DomTessFaceNb[TessId][dface][0];

  // for all faces of the domain face, in tess L2
  for (i = 1; i <= Tess[tessface[0]].DomTessFaceQty[tessface[1]]; i++)
  {
    face = Tess[tessface[0]].DomTessFaceNb[tessface[1]][i];
    newface = neut_tess_addface_alloc (pFTess);
    neut_flatten_addface (pFlatten, dom);

    neut_tess_face_addeq (pFTess, newface, Tess[tessface[0]].FaceEq[face]);
    for (j = 1; j <= Tess[tessface[0]].FaceVerQty[face]; j++)
    {
      edge = Tess[tessface[0]].FaceEdgeNb[face][j];
      ori = Tess[tessface[0]].FaceEdgeOri[face][j];
      for (k = 0; k < (*pTessE)[tessface[0]].EdgeFEdgeQty[edge]; k++)
      {
	fedge = (*pTessE)[tessface[0]].EdgeFEdgeNb[edge][k];
	fori = (*pTessE)[tessface[0]].EdgeFEdgeOri[edge][k];
	neut_tess_face_addedge (pFTess, newface, fedge, ori * fori);
      }
    }

    neut_tesse_face_addface (&((*pTessE)[TessId]), dface, newface, 1);
    neut_tesse_face_addface (&((*pTessE)[tessface[0]]), face, newface, 1);
  }

  ut_free_1d_int (tessface);

  return;
}

void
net_mtess_flatten_face_domface_body (struct MTESS MTess, struct TESS *Tess,
			       int TessId, int dface, struct TESS *pFTess,
			       struct TESSE **pTessE, int *dom,
			       struct FLATTEN *pFlatten)
{
  int i, j, fedge, fedgeinv;
  int edgeqty = 0;
  int *edges = NULL;
  int **tessface = ut_alloc_2d_int (2, 2);
  int *edgesinv = NULL;
  int edgemax;
  int **edgetessfaceqty = NULL;
  int ***edgetessfacenb = NULL;
  double *coo = ut_alloc_1d (3);
  double *coob = ut_alloc_1d (3);

  // Recording the tesses / domfaces of the domface
  net_mtess_flatten_face_domface_body_tessfaces (MTess, Tess, TessId, dface, tessface);

  // Recording all edges of the domface
  neut_flatten_domface_edges (*pFlatten, Tess[TessId], dface, &edges, &edgeqty);
  ut_array_1d_int_inv (edges, edgeqty, &edgesinv, &edgemax);

  // For each edge, recording the faces of the tesses it belongs to
  edgetessfaceqty = ut_alloc_2d_int (edgeqty + 1, 2);
  edgetessfacenb = ut_alloc_2d_pint (edgeqty + 1, 2);

  for (i = 0; i < edgeqty; i++)
  {
    fedge = edges[i];
    fedgeinv = edgesinv[fedge];

    for (j = 0; j < 2; j++)
    {
      neut_tess_edge_centre (*pFTess, fedge, coo);
      net_mtess_flatten_face_domface_edge_tess_faces (*pFlatten, fedge,
						coo, Tess[tessface[j][0]],
						tessface[j][1],
						&(edgetessfacenb[fedgeinv]
						  [j]),
						&(edgetessfaceqty[fedgeinv]
						  [j]));
    }
  }

  int ori;
  int facepairqty = 0;
  int **facepairlist = NULL;
  int *facepair = ut_alloc_1d_int (2);
  int startver, endver, fface;

  int edgepos = 0;
  while (net_mtess_flatten_face_domface_body_nextface (edges, edgeqty,
						 edgesinv,
						 edgetessfacenb,
						 edgetessfaceqty,
						 &facepairlist, &facepairqty,
						 &edgepos, facepair) == 1)
  {
    fedge = edges[edgepos];

    fface = neut_tess_addface_alloc (pFTess);
    neut_flatten_addface (pFlatten, dom);
    neut_tess_face_addeq (pFTess, fface, Tess[TessId].FaceEq[dface]);

    // adding the fedge with ori == 1; recording start ver
    neut_tess_face_addedge (pFTess, fface, fedge, 1);

    startver = (*pFTess).EdgeVerNb[fedge][0];
    endver = (*pFTess).EdgeVerNb[fedge][1];

    int nextedge, nextedgeori, endedge;
    endedge = fedge;
    while (endver != startver)
    {
      // Looking for the next fedge. The fedge is connected to the
      // previous one and has the same tess faces.
      net_mtess_flatten_face_domface_body_nextedge (*pFlatten,
					      Tess[TessId],
					      dface, *pFTess,
					      facepair, endedge,
					      endver,
					      edgesinv,
					      edgetessfacenb,
					      edgetessfaceqty,
					      &nextedge, &nextedgeori);
      if (nextedge == -1)
	ut_error_reportbug ();

      neut_tess_face_addedge (pFTess, fface, nextedge, nextedgeori);

      int pos = (nextedgeori == 1) ? 1 : 0;
      endver = (*pFTess).EdgeVerNb[nextedge][pos];
      endedge = nextedge;
    }

    // setting face normal so that the edges / vers are listed in
    // trigonometric order
    neut_tess_face_fixorifromedges (pFTess, (*pFTess).FaceQty);

    for (j = 0; j < 2; j++)
    {
      ori = ut_num_sgn (ut_vector_scalprod
			((*pFTess).FaceEq[(*pFTess).FaceQty] + 1,
			 Tess[tessface[j][0]].FaceEq[facepair[j]] + 1));
      neut_tesse_face_addface (&((*pTessE)[tessface[j][0]]),
			       facepair[j], (*pFlatten).FaceQty, ori);
    }
    ori = ut_num_sgn (ut_vector_scalprod
		      ((*pFTess).FaceEq[(*pFTess).FaceQty] + 1,
		       Tess[TessId].FaceEq[dface] + 1));
    neut_tesse_face_addface (&((*pTessE)[TessId]), dface,
			     (*pFlatten).FaceQty, ori);
  }

  ut_free_1d_int (facepair);
  ut_free_1d_int (edges);
  ut_free_2d_int (tessface, 2);
  ut_free_2d_int (edgetessfaceqty, edgeqty + 1);
  ut_free_3d_int (edgetessfacenb, edgeqty + 1, 2);
  ut_free_1d (coo);
  ut_free_1d (coob);
  ut_free_1d_int (edgesinv);

  return;
}
