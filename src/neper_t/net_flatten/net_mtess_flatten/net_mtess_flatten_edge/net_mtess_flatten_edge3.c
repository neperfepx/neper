/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_edge_.h"

void
net_mtess_flatten_edge_domface_edges (struct MTESS MTess, struct TESS *Tess,
				int tessid, int face, int *pVerQty,
				double ***pVerCoo, int **pTessNb,
				int *pTessQty, int ***pEdgeNb, int **pEdgeQty,
				int ****pEdgeVerNb, int ***pEdgeVerQty)
{
  int t, i, j, status, ver, edgeinv, poly;
  double *I = ut_alloc_1d (3);
  int **edgever = NULL;
  int *edge = NULL;
  int **EdgeNbInv = NULL;
  int *EdgeNbInvQty = NULL;

  (*pVerQty) = 0;
  (*pVerCoo) = ut_alloc_1d_pdouble (1);

  if (Tess[tessid].FaceDom[face][0] == -1)
  {
    (*pTessQty) = 2;
    (*pTessNb) = ut_alloc_1d_int (2);
    for (t = 0; t < 2; t++)
      (*pTessNb)[t] = MTess.DomTess[tessid][Tess[tessid].FacePoly[face][t]];
  }
  else
  {
    (*pTessQty) = 1;
    (*pTessNb) = ut_alloc_1d_int (1);
    poly = ut_array_1d_int_max (Tess[tessid].FacePoly[face], 2);
    (*pTessNb)[0] = MTess.DomTess[tessid][poly];
  }

  edgever = ut_alloc_1d_pint (*pTessQty);
  edge = ut_alloc_1d_int (*pTessQty);
  EdgeNbInv = ut_alloc_1d_pint (*pTessQty);
  EdgeNbInvQty = ut_alloc_1d_int (*pTessQty);

  (*pEdgeVerQty) = ut_alloc_1d_pint (*pTessQty);
  (*pEdgeVerNb) = ut_alloc_1d_ppint (*pTessQty);
  (*pEdgeNb) = ut_alloc_1d_pint (*pTessQty);
  (*pEdgeQty) = ut_alloc_1d_int (*pTessQty);

  for (t = 0; t < *pTessQty; t++)
  {
    neut_tess_domface_body_edges (Tess[(*pTessNb)[t]],
				  MTess.DomTessFaceNb[tessid][face][t],
				  &((*pEdgeNb)[t]), &((*pEdgeQty)[t]));

    if ((*pEdgeQty)[t] > 0)
      ut_array_1d_int_inv ((*pEdgeNb)[t], (*pEdgeQty)[t], &(EdgeNbInv[t]),
			   &(EdgeNbInvQty[t]));

    (*pEdgeVerQty)[t] = ut_alloc_1d_int ((*pEdgeQty)[t]);
    (*pEdgeVerNb)[t] = ut_alloc_1d_pint ((*pEdgeQty)[t]);
  }

  if (*pTessQty == 2)
  {
    for (i = 0; i < (*pEdgeQty)[0]; i++)
    {
      edge[0] = (*pEdgeNb)[0][i];
      edgever[0] = Tess[(*pTessNb)[0]].EdgeVerNb[edge[0]];

      for (j = 0; j < (*pEdgeQty)[1]; j++)
      {
	edge[1] = (*pEdgeNb)[1][j];
	edgever[1] = Tess[(*pTessNb)[1]].EdgeVerNb[edge[1]];

	status
	  =
	  ut_space_segments_intersectpt (Tess[(*pTessNb)[0]].VerCoo
					 [edgever[0][0]],
					 Tess[(*pTessNb)[0]].VerCoo[edgever[0]
								    [1]],
					 Tess[(*pTessNb)[1]].VerCoo[edgever[1]
								    [0]],
					 Tess[(*pTessNb)[1]].VerCoo[edgever[1]
								    [1]], I);

	if (status == 1)
	{
	  ver = ++(*pVerQty);
	  (*pVerCoo) = ut_realloc_2d_addline (*pVerCoo, ver, 3);
	  ut_array_1d_memcpy ((*pVerCoo)[ver - 1], 3, I);

	  int verqty;
	  for (t = 0; t < 2; t++)
	  {
	    edgeinv = EdgeNbInv[t][edge[t]];
	    if (edgeinv == -1)
	    {
	      printf ("EdgeNbInv[%d][%d] = -1\n", t, edge[t]);
	      abort ();
	    }
	    verqty = ++(*pEdgeVerQty)[t][edgeinv];
	    (*pEdgeVerNb)[t][edgeinv] = ut_realloc_1d_int
	      ((*pEdgeVerNb)[t][edgeinv], verqty);
	    (*pEdgeVerNb)[t][edgeinv][verqty - 1] = ver;
	  }
	}
      }
    }

    // if more than 1 intersection ver, sorting them so that they go
    // from the 1st tessedge vertex to the second.

    int id;
    double *dist = NULL;
    for (t = 0; t < 2; t++)
    {
      int tess = (*pTessNb)[t];
      for (i = 0; i < (*pEdgeQty)[t]; i++)
      {
	ver = Tess[tess].EdgeVerNb[(*pEdgeNb)[t][i]][0];
	if ((*pEdgeVerQty)[t][i] > 1)
	{
	  dist = ut_alloc_1d ((*pEdgeVerQty)[t][i]);

	  for (j = 0; j < (*pEdgeVerQty)[t][i]; j++)
	  {
	    id = (*pEdgeVerNb)[t][i][j];
	    dist[j] =
	      ut_space_dist (Tess[tess].VerCoo[ver], (*pVerCoo)[id - 1]);
	  }
	  ut_array_1d_sort_permarray_int (dist,
					  (*pEdgeVerQty)[t][i],
					  (*pEdgeVerNb)[t][i]);

	  ut_free_1d (dist);
	  dist = NULL;
	}
      }
    }
  }

  ut_free_1d (I);
  ut_free_1d_pint (edgever);
  ut_free_1d_int (edge);
  ut_free_2d_int (EdgeNbInv, *pTessQty);
  ut_free_1d_int (EdgeNbInvQty);

  return;
}

void
net_mtess_flatten_edge_domface_addedge (struct TESS *pFTess, struct TESS Tess,
				  struct TESSE *pTessE, int edge,
				  int *dom, struct FLATTEN *pFlatten)
{
  neut_tess_addedge (pFTess, (*pTessE).VerFVer[Tess.EdgeVerNb[edge][0]],
		     (*pTessE).VerFVer[Tess.EdgeVerNb[edge][1]]);

  (*pTessE).EdgeFEdgeQty[edge] = 1;
  (*pTessE).EdgeFEdgeNb[edge] =
    ut_realloc_1d_int ((*pTessE).EdgeFEdgeNb[edge], 1);
  (*pTessE).EdgeFEdgeOri[edge] =
    ut_realloc_1d_int ((*pTessE).EdgeFEdgeOri[edge], 1);
  (*pTessE).EdgeFEdgeNb[edge][0] = (*pFTess).EdgeQty;
  (*pTessE).EdgeFEdgeOri[edge][0] = 1;

  neut_flatten_addedge (pFlatten, dom);

  return;
}

void
net_mtess_flatten_edge_domface_addedge_ivers (struct TESS *pFTess, struct TESS Tess,
					struct TESSE *pTessE, int edge,
					int *EdgeVerNb, int EdgeVerQty,
					int *dom, struct FLATTEN *pFlatten)
{
  int i, ver1, ver2;

  (*pTessE).EdgeFEdgeQty[edge] = EdgeVerQty + 1;
  (*pTessE).EdgeFEdgeNb[edge] = ut_realloc_1d_int
    ((*pTessE).EdgeFEdgeNb[edge], (*pTessE).EdgeFEdgeQty[edge]);
  (*pTessE).EdgeFEdgeOri[edge] = ut_realloc_1d_int
    ((*pTessE).EdgeFEdgeOri[edge], (*pTessE).EdgeFEdgeQty[edge]);

  // adding subedges
  for (i = 0; i < EdgeVerQty + 1; i++)
  {
    ver1 = (i == 0) ? (*pTessE).VerFVer[Tess.EdgeVerNb[edge][0]]
      : EdgeVerNb[i - 1];
    ver2 = (i < EdgeVerQty) ? EdgeVerNb[i]
      : (*pTessE).VerFVer[Tess.EdgeVerNb[edge][1]];

    neut_tess_addedge (pFTess, ver1, ver2);
    neut_flatten_addedge (pFlatten, dom);

    (*pTessE).EdgeFEdgeNb[edge][i] = (*pFTess).EdgeQty;
    (*pTessE).EdgeFEdgeOri[edge][i] = 1;
  }

  return;
}

void
net_mtess_flatten_edge_domedge_edges (struct MTESS MTess, struct TESS *Tess,
				int tessid, int edge,
				int **pTessNb, int *pTessQty,
				int ***pEdgeNb, int ***pEdgeOri,
				int **pEdgeQty)
{
  int t, id, pos;
  int *polys = NULL;
  int polyqty;

  neut_tess_edge_polys (Tess[tessid], edge, &polys, &polyqty);

  (*pTessQty) = polyqty;
  (*pTessNb) = ut_alloc_1d_int (*pTessQty);
  for (t = 0; t < polyqty; t++)
    (*pTessNb)[t] = MTess.DomTess[tessid][polys[t]];

  (*pEdgeQty) = ut_alloc_1d_int (*pTessQty);
  (*pEdgeNb) = ut_alloc_1d_pint (*pTessQty);
  (*pEdgeOri) = ut_alloc_1d_pint (*pTessQty);

  for (t = 0; t < *pTessQty; t++)
  {
    id = (*pTessNb)[t];
    pos = ut_array_1d_int_eltpos
      (MTess.TessDomEdgeNb[id] + 1, Tess[id].DomEdgeQty, edge);
    if (pos == -1)
      ut_error_reportbug ();
    pos++;

    neut_tess_domedge_edges_sorted (Tess[id], pos, &((*pEdgeNb)[t]),
				    &((*pEdgeOri)[t]), &((*pEdgeQty)[t]));
  }

  ut_free_1d_int (polys);

  return;
}

void
net_mtess_flatten_edge_domedge_addedges (struct TESS *pFTess, struct TESSE
				   *TessE, struct TESS *Tess,
				   int domtess, int domedge,
				   int *TessNb, int TessQty,
				   int **EdgeNb, int **EdgeOri,
				   int *EdgeQty, struct FLATTEN *pFlatten)
{
  int i, j, tess, pos, edge, ori;
  int *dom = ut_alloc_1d_int (2);
  ut_array_1d_int_set_2 (dom, 1, domedge);

  // recording the current edge of the tesses while going along the
  // domain edge
  int id, firstver, lastver, begver, endver;
  int *EdgePos = ut_alloc_1d_int (TessQty);
  int *ver = ut_alloc_1d_int (TessQty);
  double *dist = ut_alloc_1d (TessQty);
  double *firstvercoo = NULL;
  double *vercoo = NULL;

  // recording start point of the domain edge
  firstver = TessE[domtess].VerFVer[Tess[domtess].EdgeVerNb[domedge][0]];
  lastver = TessE[domtess].VerFVer[Tess[domtess].EdgeVerNb[domedge][1]];
  firstvercoo = (*pFTess).VerCoo[firstver];
  begver = firstver;

  // is there an intermediate ver?
  int status = 0;
  for (i = 0; i < TessQty; i++)
    if (EdgePos[i] < EdgeQty[i] - 1)
    {
      status = 1;
      break;
    }

  while (status)
  {
    // looking for the next node

    // for each tessellation, looking for the next node and computing
    // distance from the reference (start) point
    for (i = 0; i < TessQty; i++)
    {
      tess = TessNb[i];

      if (EdgePos[i] < EdgeQty[i] - 1)
      {
	// looking for the number of the end node (depends on orientation)
	edge = EdgeNb[i][EdgePos[i]];
	pos = (EdgeOri[i][EdgePos[i]] == 1) ? 1 : 0;
	ver[i] = Tess[tess].EdgeVerNb[edge][pos];
	ver[i] = TessE[tess].VerFVer[ver[i]];
	vercoo = (*pFTess).VerCoo[ver[i]];

	dist[i] = ut_space_dist (firstvercoo, vercoo);
      }
      else
	dist[i] = DBL_MAX;
    }

    // looking for the tessellation / edge which has the next node
    id = ut_array_1d_min_index (dist, TessQty);
    tess = TessNb[id];
    edge = EdgeNb[id][EdgePos[id]];
    endver = ver[id];
    ori = EdgeOri[id][EdgePos[id]];
    if (ori != -1 && ori != 1)
      ut_error_reportbug ();

    neut_tess_addedge (pFTess, begver, endver);
    neut_flatten_addedge (pFlatten, dom);

    net_mtess_flatten_edge_domedge_linkedge ((*pFTess).EdgeQty, TessE,
				       domtess, domedge, 1);

    for (i = 0; i < TessQty; i++)
    {
      tess = TessNb[i];
      edge = EdgeNb[i][EdgePos[i]];
      ori = EdgeOri[i][EdgePos[i]];
      net_mtess_flatten_edge_domedge_linkedge ((*pFTess).EdgeQty, TessE, tess,
					 edge, ori);
    }
    EdgePos[id]++;

    begver = endver;

    // is there an intermediate ver?
    status = 0;
    for (i = 0; i < TessQty; i++)
      if (EdgePos[i] < EdgeQty[i] - 1)
      {
	status = 1;
	break;
      }
  }

  for (i = 0; i < TessQty; i++)
    if (EdgePos[i] != EdgeQty[i] - 1)
      ut_error_reportbug ();

  // recording last edge
  endver = lastver;

  neut_tess_addedge (pFTess, begver, endver);
  neut_flatten_addedge (pFlatten, dom);

  net_mtess_flatten_edge_domedge_linkedge ((*pFTess).EdgeQty, TessE, domtess,
				     domedge, 1);

  for (i = 0; i < TessQty; i++)
  {
    tess = TessNb[i];
    edge = EdgeNb[i][EdgePos[i]];
    ori = EdgeOri[i][EdgePos[i]];
    net_mtess_flatten_edge_domedge_linkedge ((*pFTess).EdgeQty, TessE, tess,
				       edge, ori);
  }

  // Depending on the orientation of the edge in the L2 tessellations
  // wrt the orientation of the edge in the L1 tessellation (namely, if
  // they are opposite), the fedges of the L2 tessellation need to be
  // reversed.
  for (i = 0; i < TessQty; i++)
  {
    tess = TessNb[i];
    for (j = 0; j < EdgeQty[i]; j++)
    {
      edge = EdgeNb[i][j];
      ori = EdgeOri[i][j];

      if (ori == -1)
      {
	ut_array_1d_int_reverseelts (TessE[tess].EdgeFEdgeNb[edge],
				     TessE[tess].EdgeFEdgeQty[edge]);
	ut_array_1d_int_reverseelts (TessE[tess].EdgeFEdgeOri[edge],
				     TessE[tess].EdgeFEdgeQty[edge]);
      }
    }
  }

  ut_free_1d_int (dom);

  return;
}
