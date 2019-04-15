/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_poly_topo_.h"

int
neut_poly_isvoid (struct POLY Poly)
{
  return (Poly.VerQty == 0);
}

void
neut_poly_neighpolys (struct POLY Poly, struct SEEDSET SSet, int **pneighs, int *pneighqty)
{
  int i, neigh;

  (*pneighqty) = 0;
  ut_free_1d_int_ (pneighs);

  for (i = 1; i <= Poly.FaceQty; i++)
  {
    neigh = Poly.FacePoly[i];
    if (neigh > 0)
    {
      if (neigh > SSet.N)
        neigh = SSet.PerSeedMaster[neigh];

      ut_array_1d_int_list_addelt (pneighs, pneighqty, neigh);
    }
  }

  return;
}

void
neut_polys_neighpolys (struct POLY *Poly, struct SEEDSET SSet, int *polys, int polyqty,
                       int **pneighs, int *pneighqty)
{
  int poly, i, j, qty = 0, *tmp = NULL;

  ut_free_1d_int (*pneighs);
  *pneighqty = 0;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    neut_poly_neighpolys (Poly[poly], SSet, &tmp, &qty);
    for (j = 0; j < qty; j++)
      ut_array_1d_int_list_addelt (pneighs, pneighqty, tmp[j]);
  }

  return;
}

int
neut_polys_polypair_neigh (struct POLY *Poly,
			   int *PerSeedSlaveQty, int **PerSeedSlaveNb,
			   int polyA, int polyB)
{
  int status;

  if (Poly[polyA].FaceQty == 0 || Poly[polyB].FaceQty == 0)
    status = 0;
  else if (polyA == polyB)
    status = 0;
  else
  {
    status = 0;

    if (ut_array_1d_int_eltpos (Poly[polyA].FacePoly + 1,
	                        Poly[polyA].FaceQty, polyB) != -1)
      status = 1;

    else if (PerSeedSlaveQty)
    {
      ut_array_1d_int_inter (Poly[polyA].FacePoly + 1,
			     Poly[polyA].FaceQty,
			     PerSeedSlaveNb[polyB] + 1,
			     PerSeedSlaveQty[polyB], NULL, &status);
      status = (status > 0);
    }
  }

  return status;
}

int
neut_polys_contiguous (struct POLY *Poly,
                       int *PerSeedSlaveQty, int **PerSeedSlaveNb,
		       int* polys, int polyqty,
		       int ***ppolys, int** ppolyqty, int *pqty)
{
  int i, j, k, poly, id, idinv, set;
  int **neighs = NULL, *neighqty = NULL;
  int *polyinv = NULL, polymax;
  int *assigned = NULL;

  ut_array_1d_int_inv (polys, polyqty, &polyinv, &polymax);

  // for each poly, recording neighbours
  neighs = ut_alloc_1d_pint (polyqty);
  neighqty = ut_alloc_1d_int (polyqty);
  for (i = 0; i < polyqty; i++)
    for (j = 0; j < polyqty; j++)
      if (i != j)
	if (neut_polys_polypair_neigh (Poly, PerSeedSlaveQty,
				       PerSeedSlaveNb, polys[i], polys[j]))
	  ut_array_1d_int_list_addelt (neighs + i, neighqty + i, polys[j]);

  assigned = ut_alloc_1d_int (polyqty);

  (*pqty) = 0;
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    if (!neut_poly_isvoid (Poly[poly]) && !assigned[i])
    {
      (*pqty)++;
      (*ppolys) = ut_realloc_1d_pint (*ppolys, *pqty);
      (*ppolyqty) = ut_realloc_1d_int (*ppolyqty, *pqty);
      set = (*pqty) - 1;
      (*ppolys)[set] = NULL;
      (*ppolyqty)[set] = 0;
      ut_array_1d_int_list_addelt (&((*ppolys)[set]),
				   &((*ppolyqty)[set]), poly);
      for (j = 0; j < (*ppolyqty)[set]; j++)
      {
	id = (*ppolys)[set][j];
	idinv = polyinv[id];
	for (k = 0; k < neighqty[idinv]; k++)
	{
	  ut_array_1d_int_list_addelt (&((*ppolys)[set]),
	                               &((*ppolyqty)[set]),
				       neighs[idinv][k]);
	  assigned[idinv] = 1;
	}
      }
    }
  }

  // adding void polys to first set
  for (i = 0; i < polyqty; i++)
  {
    poly = polys[i];
    if (neut_poly_isvoid (Poly[poly]))
      ut_array_1d_int_list_addelt (&((*ppolys)[0]),
				   &((*ppolyqty)[0]), poly);
  }

  ut_free_1d_int (polyinv);
  ut_free_1d_int (assigned);
  ut_free_2d_int (neighs, polyqty);
  ut_free_1d_int (neighqty);

  return (*pqty == 1) ? 1 : 0;
}

int
neut_poly_ver_polys (struct POLY Poly, int poly, int ver, int **ppoly, int *ppolyqty)
{
  int i, face, poly2;

  (*ppolyqty) = 0;
  ut_free_1d_int (*ppoly);

  if (poly > 0)
    ut_array_1d_int_list_addelt (ppoly, ppolyqty, poly);

  for (i = 0; i < 3; i++)
  {
    face = Poly.VerFace[ver][i];
    poly2 = Poly.FacePoly[face];
    if (poly2 > 0)
      ut_array_1d_int_list_addelt (ppoly, ppolyqty, poly2);
  }

  return 0;
}

int
neut_poly_verpair_compolys (struct POLY Poly, int ver1, int ver2,
                            int **ppoly, int *ppolyqty)
{
  int i, verqty;
  int *vers = ut_alloc_1d_int (2);
  int **VerPoly = ut_alloc_1d_pint (2);
  int *VerPolyQty = ut_alloc_1d_int (2);

  verqty = 2;
  vers[0] = ver1;
  vers[1] = ver2;

  for (i = 0; i < verqty; i++)
    neut_poly_ver_polys (Poly, -1, vers[i], VerPoly + i, VerPolyQty + i);

  (*ppoly) = ut_alloc_1d_int (ut_array_1d_int_max (VerPolyQty, verqty));
  ut_array_1d_int_inter (VerPoly[0], VerPolyQty[0], VerPoly[1], VerPolyQty[1],
                         (*ppoly), ppolyqty);
  (*ppoly) = ut_realloc_1d_int (*ppoly, *ppolyqty);

  ut_free_2d_int (VerPoly, 2);
  ut_free_1d_int (VerPolyQty);
  ut_free_1d_int (vers);

  return 0;
}

int
neut_poly_edges (struct POLY Poly, int ***pedges, int *pedgeqty)
{
  int i, j, k, status;
  int *tmp = ut_alloc_1d_int (2);

  (*pedgeqty) = 0;
  for (i = 1; i <= Poly.FaceQty; i++)
    for (j = 1; j <= Poly.FaceVerQty[i]; j++)
    {
      tmp[0] = Poly.FaceVerNb[i][j];
      tmp[1] = Poly.FaceVerNb[i][ut_num_rotpos (1, Poly.FaceVerQty[i], j, 1)];
      ut_array_1d_int_sort (tmp, 2);

      status = 0;
      for (k = 0; k < (*pedgeqty); k++)
	if (ut_array_1d_int_equal (tmp, 2, (*pedges)[k], 2))
	{
	  status = 1;
	  break;
	}

      if (!status)
      {
	(*pedgeqty)++;
	(*pedges) = ut_realloc_2d_int_addline (*pedges, *pedgeqty, 2);
	ut_array_1d_int_memcpy ((*pedges)[(*pedgeqty) - 1], 2, tmp);
      }
    }

  ut_free_1d_int (tmp);

  return 0;
}
