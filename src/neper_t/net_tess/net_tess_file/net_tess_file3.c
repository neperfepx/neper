/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_file_.h"

int
net_tess_file_mtess_vers (struct MTESS *pMTess, struct TESS *Tess, int dtess,
                          int dcell, int TessId)
{
  int i, status, *pos = NULL, *vers = NULL, verqty;
  double dist, **vercoos = NULL;

  neut_tess_poly_vercoos (Tess[dtess], dcell, &vers, &vercoos, &verqty);

  pos = ut_alloc_1d_int (verqty);
  status = ut_array_2d_pair (Tess[TessId].DomVerCoo + 1, Tess[TessId].DomVerQty, 3,
                             vercoos, verqty, 3, pos, &dist);
  if (status || dist > 1e-6)
    abort ();

  (*pMTess).TessDomVerNb
    = ut_realloc_2d_int_addline ((*pMTess).TessDomVerNb,
                                 (*pMTess).TessQty + 1,
                                 Tess[TessId].DomVerQty + 1);
  for (i = 0; i < verqty; i++)
    (*pMTess).TessDomVerNb[TessId][i + 1] = vers[pos[i]];

  ut_free_1d_int (pos);
  ut_free_2d (vercoos, verqty);

  return 0;
}

int
net_tess_file_mtess_edges (struct MTESS *pMTess, struct TESS *Tess, int dtess,
                           int dcell, int TessId)
{
  int i, j, status, *pos = NULL, *edges = NULL, **edgevers = NULL, edgeqty;

  neut_tess_poly_edgevers (Tess[dtess], dcell, &edges, &edgevers, &edgeqty);

  int **tess_edgevers = ut_alloc_2d_int (Tess[TessId].DomEdgeQty, 2);
  for (i = 0; i < Tess[TessId].DomEdgeQty; i++)
    for (j = 0; j < 2; j++)
      tess_edgevers[i][j] = (*pMTess).TessDomVerNb[TessId][Tess[TessId].DomEdgeVerNb[i + 1][j]];

  pos = ut_alloc_1d_int (edgeqty);
  status = ut_array_2d_int_list_pair (tess_edgevers,
                                      Tess[TessId].DomEdgeQty, 2,
                                      edgevers, edgeqty, 2, pos);
  if (status)
    abort ();

  (*pMTess).TessDomEdgeNb
    = ut_realloc_2d_int_addline ((*pMTess).TessDomEdgeNb,
                                 (*pMTess).TessQty + 1,
                                 Tess[TessId].DomEdgeQty + 1);
  for (i = 0; i < edgeqty; i++)
    (*pMTess).TessDomEdgeNb[TessId][i + 1] = edges[pos[i]];

  ut_free_1d_int (edges);
  ut_free_2d_int (edgevers, edgeqty);
  ut_free_2d_int (tess_edgevers, edgeqty);
  ut_free_1d_int (pos);

  return 0;
}

int
net_tess_file_mtess_faces (struct MTESS *pMTess, struct TESS *Tess, int dtess,
                           int dcell, int TessId)
{
  int i, j, *pos = NULL, *faces = NULL, **faceedges = NULL, *faceedgeqty = NULL, faceqty;
  int status;
  int pos2, face;

  neut_tess_poly_faceedges (Tess[dtess], dcell, &faces, &faceedges, &faceedgeqty, &faceqty);

  int **tess_faceedges = ut_alloc_1d_pint (Tess[TessId].DomFaceQty);
  int *tess_faceedgeqty = ut_alloc_1d_int (Tess[TessId].DomFaceQty);
  for (i = 0; i < Tess[TessId].DomFaceQty; i++)
  {
    tess_faceedgeqty[i] = Tess[TessId].DomFaceEdgeQty[i + 1];
    tess_faceedges[i] = ut_alloc_1d_int (tess_faceedgeqty[i]);
    for (j = 0; j < tess_faceedgeqty[i]; j++)
      tess_faceedges[i][j] = (*pMTess).TessDomEdgeNb[TessId][Tess[TessId].DomFaceEdgeNb[i + 1][j + 1]];
  }

  pos = ut_alloc_1d_int (faceqty);
  status = ut_array_2d_int_list_pair_2 (tess_faceedges,
                                      Tess[TessId].DomFaceQty, Tess[TessId].DomFaceEdgeQty + 1,
                                      faceedges, faceqty, faceedgeqty, pos);
  if (status)
    abort ();

  (*pMTess).TessDomFaceNb
    = ut_realloc_2d_int_addline ((*pMTess).TessDomFaceNb,
                                 (*pMTess).TessQty + 1,
                                 Tess[TessId].DomFaceQty + 1);

  for (i = 0; i < faceqty; i++)
    (*pMTess).TessDomFaceNb[TessId][i + 1] = faces[pos[i]];

  for (i = 1; i <= Tess[TessId].DomFaceQty; i++)
  {
    face = (*pMTess).TessDomFaceNb[TessId][i];

    pos2 = ut_array_1d_int_eltpos (Tess[dtess].FacePoly[face], 2, dcell);
    if (pos2 == -1)
      abort ();
    (*pMTess).DomTessFaceNb[dtess][face][pos2] = i;
  }

  ut_free_1d_int (faces);
  ut_free_2d_int (faceedges, faceqty);
  ut_free_2d_int (tess_faceedges, Tess[TessId].DomFaceQty);
  ut_free_1d_int (tess_faceedgeqty);

  return 0;
}
