/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_periodic_.h"

void
net_tess3d_periodic_face (struct TESS *pTess)
{
  int i, j, seed, masterseed, status,
    slaveseed, slaveseed2, slaveface, masterface, fact;

  if ((*pTess).Dim == 2)
  {
    (*pTess).PerFaceQty = 0;
    return;
  }

  (*pTess).PerFaceMaster = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).PerFaceShift = ut_alloc_2d_int ((*pTess).FaceQty + 1, 3);
  ut_array_1d_int_set ((*pTess).PerFaceMaster, (*pTess).FaceQty + 1, -1);

  (*pTess).PerFaceOri = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).PerFaceMaster[i] == -1)
    {
      if (ut_array_1d_int_max ((*pTess).FacePoly[i], 2) <= (*pTess).CellQty)
	(*pTess).PerFaceMaster[i] = 0;

      else
      {
	masterface = i;
	slaveface = -1;

	seed = ut_array_1d_int_min ((*pTess).FacePoly[i], 2);
	slaveseed = ut_array_1d_int_max ((*pTess).FacePoly[i], 2);
	masterseed = (*pTess).PerSeedMaster[slaveseed];

	slaveseed2 = neut_tess_seed_master_slave (*pTess, seed,
						  (*pTess).
						  PerSeedShift[slaveseed],
						  -1);

	if (seed < 0 || seed > (*pTess).CellQty
	    || masterseed < 0 || masterseed > (*pTess).CellQty)
	  ut_error_reportbug ();

	status = 0;
	for (j = 1; j <= (*pTess).PolyFaceQty[masterseed]; j++)
	{
	  slaveface = (*pTess).PolyFaceNb[masterseed][j];

	  if (ut_array_1d_int_max ((*pTess).FacePoly[slaveface], 2) ==
	      slaveseed2)
	  {
	    status = 1;
	    break;
	  }
	}

	if (status == 0)
	  abort ();

	fact = 1;
	if (slaveface < masterface)
	{
	  ut_num_switch_int (&masterface, &slaveface);
	  fact = -1;
	}

	if ((*pTess).PerFaceMaster[masterface] != -1
	    || (*pTess).PerFaceMaster[slaveface] != -1)
	  abort ();

	(*pTess).PerFaceQty++;
	(*pTess).PerFaceNb = ut_realloc_1d_int ((*pTess).PerFaceNb,
						(*pTess).PerFaceQty + 1);
	(*pTess).PerFaceNb[(*pTess).PerFaceQty] = slaveface;
	(*pTess).PerFaceMaster[masterface] = 0;
	(*pTess).PerFaceMaster[slaveface] = masterface;
	ut_array_1d_int_memcpy ((*pTess).PerFaceShift[slaveface],
				3, (*pTess).PerSeedShift[slaveseed]);
	ut_array_1d_int_scale ((*pTess).PerFaceShift[slaveface], 3, -fact);

	int poly, ori1, ori2;
	poly = ut_array_1d_int_min ((*pTess).FacePoly[masterface], 2);
	neut_tess_poly_face_ori (*pTess, poly, masterface, &ori1);
	poly = ut_array_1d_int_min ((*pTess).FacePoly[slaveface], 2);
	neut_tess_poly_face_ori (*pTess, poly, slaveface, &ori2);

	(*pTess).PerFaceOri[slaveface] = (ori1 != ori2) ? -1 : 1;
      }
    }

  ut_array_1d_int_sort ((*pTess).PerFaceNb + 1, (*pTess).PerFaceQty);

  neut_tess_init_faceslave (pTess);

  return;
}

void
net_tess3d_periodic_edge (struct TESS *pTess)
{
  int i, j, k, edge, peredge, masteredge, slaveedge, fact, status;
  int *edgelist = NULL;
  int edgeqty;
  int *perseedlist = ut_alloc_1d_int (3);
  int seed, seedqty, *seedlist = NULL;
  int *shift = ut_alloc_1d_int (3);

  (*pTess).PerEdgeMaster = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).PerEdgeShift = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 3);

  (*pTess).PerEdgeOri = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  if ((*pTess).Dim == 3)
    neut_tess_faces_edges (*pTess, (*pTess).PerFaceNb + 1,
			   (*pTess).PerFaceQty, &edgelist, &edgeqty);
  else
  {
    edgeqty = (*pTess).EdgeQty;
    edgelist = ut_alloc_1d_int (edgeqty);
    ut_array_1d_int_set_id (edgelist, edgeqty);
    ut_array_1d_int_addval (edgelist, edgeqty, 1, edgelist);
  }

  for (i = 0; i < edgeqty; i++)
  {
    edge = edgelist[i];

    neut_tess_edge_seeds (*pTess, edge, &seedlist, &seedqty);

    for (j = 0; j < seedqty; j++)
    {
      seed = seedlist[j];

      if (seed > (*pTess).CellQty)
      {
	ut_array_1d_int_memcpy (shift, 3, (*pTess).PerSeedShift[seed]);
	ut_array_1d_int_scale (shift, 3, -1);

	for (k = 0; k < seedqty; k++)
	  perseedlist[k] =
	    neut_tess_seed_perseed (*pTess, seedlist[k], shift);

	status =
	  neut_tess_seeds_edge (*pTess, perseedlist, seedqty, &peredge);

	if (status != 0)
	  ut_error_reportbug ();

	if (edge < peredge)
	{
	  masteredge = edge;
	  slaveedge = peredge;
	  fact = -1;
	}
	else
	{
	  masteredge = peredge;
	  slaveedge = edge;
	  fact = 1;
	}

	if (ut_array_1d_int_eltpos ((*pTess).PerEdgeNb + 1,
				    (*pTess).PerEdgeQty, slaveedge) == -1)
	{
	  (*pTess).PerEdgeQty++;
	  (*pTess).PerEdgeNb = ut_realloc_1d_int ((*pTess).PerEdgeNb,
						  (*pTess).PerEdgeQty + 1);
	  (*pTess).PerEdgeNb[(*pTess).PerEdgeQty] = slaveedge;

	  (*pTess).PerEdgeMaster[slaveedge] = masteredge;
	  ut_array_1d_int_memcpy ((*pTess).PerEdgeShift[slaveedge], 3, shift);
	  ut_array_1d_int_scale ((*pTess).PerEdgeShift[slaveedge], 3, -fact);
	}
      }
    }
  }

  ut_array_1d_int_sort ((*pTess).PerEdgeNb + 1, (*pTess).PerEdgeQty);

  net_tess3d_periodic_edge_shrinkmaster (pTess);
  net_tess3d_periodic_edge_peredgeori (pTess);
  neut_tess_init_edgeslave (pTess);

  ut_free_1d_int (edgelist);
  ut_free_1d_int (perseedlist);
  ut_free_1d_int (shift);

  return;
}

void
net_tess3d_periodic_ver (struct TESS *pTess)
{
  int i, j, k, ver, perver, masterver, slavever, fact;
  int *verlist = NULL;
  int verqty;
  int *perseedlist = ut_alloc_1d_int (4);
  int seed, seedqty, *seedlist = NULL;
  int *shift = ut_alloc_1d_int (3);

  (*pTess).PerVerMaster = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).PerVerShift = ut_alloc_2d_int ((*pTess).VerQty + 1, 3);

  if ((*pTess).Dim == 3)
    neut_tess_faces_vers (*pTess, (*pTess).PerFaceNb + 1,
			  (*pTess).PerFaceQty, &verlist, &verqty);
  else
  {
    verqty = (*pTess).VerQty;
    verlist = ut_alloc_1d_int (verqty);
    ut_array_1d_int_set_id (verlist, verqty);
    ut_array_1d_int_addval (verlist, verqty, 1, verlist);
  }

  for (i = 0; i < verqty; i++)
  {
    ver = verlist[i];

    neut_tess_ver_seeds (*pTess, ver, &seedlist, &seedqty);

    for (j = 0; j < seedqty; j++)
    {
      seed = seedlist[j];

      if (seed > (*pTess).CellQty)
      {
	ut_array_1d_int_memcpy (shift, 3, (*pTess).PerSeedShift[seed]);
	ut_array_1d_int_scale (shift, 3, -1);

	for (k = 0; k < seedqty; k++)
	  perseedlist[k] =
	    neut_tess_seed_perseed (*pTess, seedlist[k], shift);

	neut_tess_seeds_ver (*pTess, perseedlist, seedqty, &perver);

	if (ver < perver)
	{
	  masterver = ver;
	  slavever = perver;
	  fact = -1;
	}
	else
	{
	  masterver = perver;
	  slavever = ver;
	  fact = 1;
	}

	if (ut_array_1d_int_eltpos ((*pTess).PerVerNb + 1,
				    (*pTess).PerVerQty, slavever) == -1)
	{
	  (*pTess).PerVerQty++;
	  (*pTess).PerVerNb = ut_realloc_1d_int ((*pTess).PerVerNb,
						 (*pTess).PerVerQty + 1);
	  (*pTess).PerVerNb[(*pTess).PerVerQty] = slavever;

	  (*pTess).PerVerMaster[slavever] = masterver;
	  ut_array_1d_int_memcpy ((*pTess).PerVerShift[slavever], 3, shift);
	  ut_array_1d_int_scale ((*pTess).PerVerShift[slavever], 3, -fact);
	}
      }
    }
  }

  net_tess3d_periodic_ver_shrinkmaster (pTess);

  ut_array_1d_int_sort ((*pTess).PerVerNb + 1, (*pTess).PerVerQty);

  neut_tess_init_verslave (pTess);

  ut_free_1d_int (verlist);
  ut_free_1d_int (perseedlist);
  ut_free_1d_int (shift);

  return;
}
