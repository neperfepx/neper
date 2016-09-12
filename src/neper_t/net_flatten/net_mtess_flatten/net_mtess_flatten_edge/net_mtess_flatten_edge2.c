/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_edge_.h"

void
net_mtess_flatten_edge_body (struct MTESS MTess, struct TESS *Tess,
			     int *CTessIds, int CTessQty,
			     struct TESS *pFTess, struct TESSE *TessE,
			     struct FLATTEN *pFlatten)
{
  int i, j, id;
  int *dom = ut_alloc_1d_int (2);

  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    ut_array_1d_int_set_2 (dom, 3, MTess.TessDom[id][1]);
    // Body edges
    for (j = 1; j <= Tess[id].EdgeQty; j++)
      if (Tess[id].EdgeDom[j][0] == -1)
      {
	neut_tess_addedge (pFTess, TessE[id].VerFVer[Tess[id].EdgeVerNb[j][0]],
			   TessE[id].VerFVer[Tess[id].EdgeVerNb[j][1]]);
	neut_flatten_addedge (pFlatten, dom);

	net_mtess_flatten_edge_domedge_linkedge ((*pFTess).EdgeQty, TessE, id, j, 1);
      }
  }

  ut_free_1d_int (dom);

  return;
}

void
net_mtess_flatten_edge_domface (struct MTESS MTess, struct TESS *Tess,
			  int TessId, struct TESS *pFTess,
			  struct TESSE *TessE, struct FLATTEN *pFlatten)
{
  int i, j, t;
  int VerQty_ini, VerQty = 0;
  double **VerCoo = NULL;
  int TessQty;
  int *TessNb = NULL;
  int *EdgeQty = NULL;
  int **EdgeNb = NULL;
  int **EdgeVerQty = NULL;
  int ***EdgeVerNb = NULL;
  int *dom = ut_alloc_1d_int (2);

  for (i = 1; i <= Tess[TessId].FaceQty; i++)
  {
    // looking for edges (with intersections since 2 tesses)
    net_mtess_flatten_edge_domface_edges (MTess, Tess, TessId, i,
				    &VerQty, &VerCoo, &TessNb,
				    &TessQty, &EdgeNb, &EdgeQty,
				    &EdgeVerNb, &EdgeVerQty);

    // adding vertices
    VerQty_ini = (*pFTess).VerQty;
    ut_array_1d_int_set_2 (dom, 2, i);
    for (j = 0; j < VerQty; j++)
    {
      neut_tess_addver (pFTess, VerCoo[j]);
      neut_flatten_addver (pFlatten, dom);
    }

    // renumbering vertices in edges
    for (t = 0; t < TessQty; t++)
      for (j = 0; j < EdgeQty[t]; j++)
	ut_array_1d_int_addval (EdgeVerNb[t][j], EdgeVerQty[t][j],
				VerQty_ini, EdgeVerNb[t][j]);

    // adding edges
    for (t = 0; t < TessQty; t++)
      for (j = 0; j < EdgeQty[t]; j++)
	if (EdgeVerQty[t][j] == 0)
	  net_mtess_flatten_edge_domface_addedge (pFTess, Tess[TessNb[t]],
					    &(TessE[TessNb[t]]),
					    EdgeNb[t][j], dom, pFlatten);
	else
	  net_mtess_flatten_edge_domface_addedge_ivers (pFTess, Tess[TessNb[t]],
						  &(TessE[TessNb[t]]),
						  EdgeNb[t][j],
						  EdgeVerNb[t][j],
						  EdgeVerQty[t][j],
						  dom, pFlatten);

    // free'ing memory
    for (t = 0; t < TessQty; t++)
      for (j = 0; j < EdgeQty[t]; j++)
	ut_free_1d_int_ (&EdgeVerNb[t][j]);
    ut_free_1d_ppint_ (&EdgeVerNb);

    ut_free_2d_int_ (&EdgeVerQty, TessQty);
    ut_free_2d_int_ (&EdgeNb, TessQty);
    ut_free_1d_int_ (&EdgeQty);
    ut_free_1d_int_ (&TessNb);
  }

  ut_free_1d_int (dom);

  return;
}

void
net_mtess_flatten_edge_domedge (struct MTESS MTess, struct TESS *Tess,
			  int TessId, struct TESS *pFTess,
			  struct TESSE *TessE, struct FLATTEN *pFlatten)
{
  int i, TessQty;
  int *TessNb = NULL;
  int *EdgeQty = NULL;
  int **EdgeNb = NULL;
  int **EdgeOri = NULL;
  int *dom = ut_alloc_1d_int (2);

  for (i = 1; i <= Tess[TessId].EdgeQty; i++)
  {
    ut_array_1d_int_set_2 (dom, 1, i);

    // looking for edges
    net_mtess_flatten_edge_domedge_edges (MTess, Tess, TessId, i, &TessNb,
				    &TessQty, &EdgeNb, &EdgeOri, &EdgeQty);

    net_mtess_flatten_edge_domedge_addedges (pFTess, TessE, Tess, TessId, i,
				       TessNb, TessQty, EdgeNb, EdgeOri,
				       EdgeQty, pFlatten);

    // free'ing memory
    ut_free_2d_int (EdgeNb, TessQty);
    ut_free_1d_int (EdgeQty);
    ut_free_1d_int (TessNb);
    EdgeQty = NULL;
    EdgeNb = NULL;
    TessNb = NULL;
  }

  ut_free_1d_int (dom);

  return;
}

void
net_mtess_flatten_edge_init_fedgetessedge (struct TESS *Tess,
				     int *CTessIds, int CTessQty,
				     struct TESS *pFTess, struct TESSE *TessE,
				     struct FLATTEN *pFlatten)
{
  int i, j, k, id, fedge;

  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    for (j = 1; j <= Tess[id].EdgeQty; j++)
    {
      if (TessE[id].EdgeFEdgeQty[j] == 0)
	ut_error_reportbug ();
      for (k = 0; k < TessE[id].EdgeFEdgeQty[j]; k++)
      {
	if (TessE[id].EdgeFEdgeNb[j][k] == 0)
	  ut_error_reportbug ();
	if (TessE[id].EdgeFEdgeOri[j][k] != -1
	    && TessE[id].EdgeFEdgeOri[j][k] != 1)
	  ut_error_reportbug ();
      }
    }
  }

  // Init EdgeTessEdgeQty nd EdgeTessEdgeNb from TessE[].EdgeFEdge*
  (*pFlatten).EdgeTessEdgeQty = ut_alloc_1d_int ((*pFTess).EdgeQty + 1);
  (*pFlatten).EdgeTessEdgeNb = ut_alloc_2d_pint ((*pFTess).EdgeQty + 1, 1);
  (*pFlatten).EdgeTessEdgeNb[0][0] = NULL;
  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    for (j = 1; j <= Tess[id].EdgeQty; j++)
      for (k = 0; k < TessE[id].EdgeFEdgeQty[j]; k++)
      {
	fedge = TessE[id].EdgeFEdgeNb[j][k];
	(*pFlatten).EdgeTessEdgeQty[fedge]++;
	(*pFlatten).EdgeTessEdgeNb[fedge]
	  = ut_realloc_2d_int_addline ((*pFlatten).EdgeTessEdgeNb[fedge],
				       (*pFlatten).EdgeTessEdgeQty[fedge], 2);
	(*pFlatten).EdgeTessEdgeNb[fedge][(*pFlatten).EdgeTessEdgeQty[fedge] -
					  1][0] = id;
	(*pFlatten).EdgeTessEdgeNb[fedge][(*pFlatten).EdgeTessEdgeQty[fedge] -
					  1][1] = j;
      }
  }

  return;
}
