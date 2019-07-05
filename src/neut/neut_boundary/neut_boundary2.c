/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_boundary_.h"

void
neut_boundary_set_zero (struct BOUNDARY *pBound)
{
  (*pBound).BoundQty = 0;
  (*pBound).BoundNames = NULL;
  (*pBound).BoundSeed = NULL;
  (*pBound).BoundEltQty = NULL;
  (*pBound).BoundElts = NULL;
  (*pBound).BoundEltFacets = NULL;
  (*pBound).BoundNodeQty = NULL;
  (*pBound).BoundNodes = NULL;

  return;
}

void
neut_boundary_free (struct BOUNDARY *pBound)
{
  ut_free_3d_char ((*pBound).BoundNames, (*pBound).BoundQty + 1, 2);
  ut_free_2d_int ((*pBound).BoundDom, (*pBound).BoundQty + 1);
  ut_free_2d_int ((*pBound).BoundSeed, (*pBound).BoundQty + 1);
  ut_free_1d_int ((*pBound).BoundEltQty);
  ut_free_3d_int ((*pBound).BoundElts, (*pBound).BoundQty + 1, 2);
  ut_free_3d_int ((*pBound).BoundEltFacets, (*pBound).BoundQty + 1, 2);
  ut_free_1d_int ((*pBound).BoundNodeQty);
  ut_free_3d_int ((*pBound).BoundNodes, (*pBound).BoundQty + 1, 2);
  (*pBound).BoundQty = 0;

  return;
}

void
neut_boundary_bound_nodes_2d (struct MESH Mesh, struct BOUNDARY Bound, int id, int side,
                              int **pnodes, int *pnodeqty)
{
  int i, ori, pos, elt, facet, nextelt, nextfacet, lastnode;
  int *surfnodes = ut_alloc_1d_int (2);
  int *nextsurfnodes = ut_alloc_1d_int (2);

  if (Mesh.Dimension != 2)
    abort ();

  if (Bound.BoundEltQty[id] == 0)
  {
    (*pnodeqty) = 0;
    return;
  }

  (*pnodes) = ut_alloc_1d_int (Bound.BoundEltQty[id] + 1);

  elt = Bound.BoundElts[id][side][1];
  facet = Bound.BoundEltFacets[id][side][1];
  neut_elt_facet_nodes (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder, Mesh.EltNodes[elt], facet, 1, surfnodes);

  ori = 1;
  if (Bound.BoundEltQty[id] > 1)
  {
    nextelt = Bound.BoundElts[id][side][2];
    nextfacet = Bound.BoundEltFacets[id][side][2];
    neut_elt_facet_nodes (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder, Mesh.EltNodes[nextelt], nextfacet, 1, nextsurfnodes);

    if (ut_array_1d_int_eltpos (nextsurfnodes, 2, surfnodes[1]) != -1)
    {
      ori = 1;
      (*pnodes)[0] = surfnodes[0];
    }
    else if (ut_array_1d_int_eltpos (nextsurfnodes, 2, surfnodes[0]) != -1)
    {
      ori = -1;
      (*pnodes)[0] = surfnodes[1];
    }
    else
      abort ();
  }
  else
    (*pnodes)[0] = surfnodes[0];

  for (i = 1; i <= Bound.BoundEltQty[id]; i++)
  {
    elt = Bound.BoundElts[id][side][i];
    facet = Bound.BoundEltFacets[id][side][i];
    neut_elt_facet_nodes (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder, Mesh.EltNodes[elt], facet, 1, surfnodes);

    lastnode = (*pnodes)[i - 1];
    if (i > 1)
    {
      pos = ut_array_1d_int_eltpos (Mesh.EltNodes[elt], 3, lastnode);
      if (pos == -1)
	abort ();

      if (surfnodes[0] == lastnode)
	ori = 1;
      else if (surfnodes[1] == lastnode)
	ori = -1;
      else
	abort ();
    }

    (*pnodes)[i] = (ori == 1) ? surfnodes[1] : surfnodes[0];
    lastnode = (*pnodes)[i];
  }
  (*pnodeqty) = Bound.BoundEltQty[id] + 1;

  ut_free_1d_int (surfnodes);
  ut_free_1d_int (nextsurfnodes);

  return;
}

void
neut_boundary_bound_nodes_3d (struct MESH Mesh, struct BOUNDARY Bound, int id, int side,
                              int **pnodes, int *pnodeqty)
{
  int i, j, elt, facet;
  int *surfnodes = ut_alloc_1d_int (3);
  int *nextsurfnodes = ut_alloc_1d_int (3);

  if (Mesh.Dimension != 3)
    abort ();

  if (Bound.BoundEltQty[id] == 0)
  {
    (*pnodeqty) = 0;
    return;
  }

  (*pnodes) = ut_alloc_1d_int (3 * Bound.BoundEltQty[id]);

  (*pnodeqty) = 0;
  for (i = 1; i <= Bound.BoundEltQty[id]; i++)
  {
    elt = Bound.BoundElts[id][side][i];
    facet = Bound.BoundEltFacets[id][side][i];
    neut_elt_facet_nodes (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder, Mesh.EltNodes[elt], facet, 1, surfnodes);

    for (j = 0; j < 3; j++)
      (*pnodes)[(*pnodeqty)++] = surfnodes[j];
  }

  ut_free_1d_int (surfnodes);
  ut_free_1d_int (nextsurfnodes);

  return;
}
