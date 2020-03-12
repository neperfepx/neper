/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_file_.h"

void
net_tess_file_tess (int level, char *morpho, struct TESS *Tess, int TessId,
                    struct SEEDSET *SSet)
{
  struct TESS *pTess = Tess + TessId;

  neut_tess_fnscanf (morpho, pTess);

  net_tess_seedset (*pTess, SSet + TessId);

  (*pTess).Level = level;
  (*pTess).TessId = TessId;

  return;
}

void
net_tess_file_updatedomain (struct MTESS *pMTess, struct TESS *Tess,
                            int dtess, int dcell, int TessId)
{
  struct TESS Dom, *pTess = Tess + TessId;

  neut_tess_set_zero (&Dom);
  neut_tess_poly_tess (Tess[dtess], dcell, &Dom);

  // vers
  int i, j, status, *tessdomver_domver =
    ut_alloc_1d_int ((*pTess).DomVerQty + 1);
  double dist;
  status =
    ut_array_2d_pair ((*pTess).DomVerCoo + 1, (*pTess).DomVerQty, 3,
                      Dom.VerCoo + 1, Dom.VerQty, 3, tessdomver_domver + 1,
                      &dist);
  ut_array_1d_int_addval (tessdomver_domver + 1, (*pTess).DomVerQty, 1,
                          tessdomver_domver + 1);
  if (status || dist > 1e-6)
    abort ();

  // edges
  int *tessdomedge_domedge = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);

  int TessDomEdgeQty = (*pTess).DomEdgeQty, DomEdgeQty = Dom.EdgeQty;
  int **TessDomEdgeVerNb = ut_alloc_2d_int (TessDomEdgeQty, 2);
  int **DomEdgeVerNb = ut_alloc_2d_int (Dom.EdgeQty, 2);

  for (i = 0; i < TessDomEdgeQty; i++)
    for (j = 0; j < 2; j++)
      TessDomEdgeVerNb[i][j] =
        tessdomver_domver[(*pTess).DomEdgeVerNb[i + 1][j]];
  ut_array_2d_int_memcpy (Dom.EdgeVerNb + 1, DomEdgeQty, 2, DomEdgeVerNb);

  status =
    ut_array_1d_int_lists_pair_samelength (TessDomEdgeVerNb, 2,
                                           TessDomEdgeQty, DomEdgeVerNb, 2,
                                           DomEdgeQty,
                                           tessdomedge_domedge + 1);
  ut_array_1d_int_addval (tessdomedge_domedge + 1, (*pTess).DomEdgeQty, 1,
                          tessdomedge_domedge + 1);
  if (status)
    abort ();

  // faces
  int *tessdomface_domface = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);

  int TessDomFaceQty = (*pTess).DomFaceQty;
  int *TessDomFaceEdgeQty = ut_alloc_1d_int (TessDomFaceQty);
  int **TessDomFaceEdgeNb = ut_alloc_1d_pint (TessDomFaceQty);
  ut_array_1d_int_memcpy ((*pTess).DomFaceEdgeQty + 1, TessDomFaceQty,
                          TessDomFaceEdgeQty);
  for (i = 0; i < TessDomFaceQty; i++)
  {
    TessDomFaceEdgeNb[i] = ut_alloc_1d_int (TessDomFaceEdgeQty[i]);
    for (j = 0; j < TessDomFaceEdgeQty[i]; j++)
      TessDomFaceEdgeNb[i][j] =
        tessdomedge_domedge[(*pTess).DomFaceEdgeNb[i + 1][j + 1]];
  }

  int DomFaceQty = Dom.FaceQty;
  int *DomFaceEdgeQty = ut_alloc_1d_int (DomFaceQty);
  int **DomFaceEdgeNb = ut_alloc_1d_pint (DomFaceQty);
  ut_array_1d_int_memcpy (Dom.FaceVerQty + 1, DomFaceQty, DomFaceEdgeQty);
  for (i = 0; i < DomFaceQty; i++)
  {
    DomFaceEdgeNb[i] = ut_alloc_1d_int (DomFaceEdgeQty[i]);
    ut_array_1d_int_memcpy (Dom.FaceEdgeNb[i + 1] + 1, DomFaceEdgeQty[i],
                            DomFaceEdgeNb[i]);
  }

  status =
    ut_array_1d_int_lists_pair (TessDomFaceEdgeNb, TessDomFaceEdgeQty,
                                TessDomFaceQty, DomFaceEdgeNb, DomFaceEdgeQty,
                                DomFaceQty, tessdomface_domface + 1);
  ut_array_1d_int_addval (tessdomface_domface + 1, (*pTess).DomFaceQty, 1,
                          tessdomface_domface + 1);
  if (status)
    abort ();

  // we only need to correct FacePoly before we copy the domain
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FacePoly[i][1] < 0)
      (*pTess).FacePoly[i][1] =
        -tessdomface_domface[-(*pTess).FacePoly[i][1]];

  // recording parent dom in tess
  net_tess3d_domain (Dom, 1, TessId, pMTess, pTess);

  neut_tess_free (&Dom);
  ut_free_1d_int (&tessdomver_domver);
  ut_free_1d_int (&tessdomedge_domedge);
  ut_free_1d_int (&tessdomface_domface);

  return;
}

int
net_tess_file_mtess (struct MTESS *pMTess, struct TESS *Tess, int dtess,
                     int dcell, int TessId)
{
  if ((*pMTess).TessQty != TessId)
    abort ();

  net_tess_file_mtess_vers (pMTess, Tess, dtess, dcell, TessId);

  net_tess_file_mtess_edges (pMTess, Tess, dtess, dcell, TessId);

  net_tess_file_mtess_faces (pMTess, Tess, dtess, dcell, TessId);

  return 0;
}
