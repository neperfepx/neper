/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1d_.h"

// Meshing a unit segment by equidistribution of the mesh size map
//   s(x) = min (clt, cl1 + clgrad x, cl2 + clgrad (1 - x)),
// with clgrad = ln (pl): for a number of elements n, the nodes are placed so
// that int dx / s(x) is the same, n* / n (n* = int_0^1 dx / s(x)), over each
// element, so that the ratio between the lengths of two adjacent elements is
// about pl in the parts of increasing s.  As for the 2D and 3D meshes, n is
// chosen so that the mean ratio of the elt lengths to the mesh size map at
// the elt centres, q, is the closest to 1 (in log scale), among the integers
// around n*.  The characteristic lengths of the nodes are set to q s (the
// size of the mesh at the nodes).  *pqty is the number of body nodes, and
// *pcoo and *pcl (of size *pqty + 2) include the two extremities.
void
nem_mesh_1d_unitsegment (double cl1, double cl2, double clt, double pl,
                         int *pqty, double **pcoo, double **pcl)
{
  int k, n, nmin, nmax, pieceqty;
  double clgrad, nstar, q, qn, x[6], a[6], b[6], F[6];

  if (cl1 <= 0 || cl2 <= 0 || clt <= 0 || pl < 1)
  {
    printf ("cl1 = %f cl2 = %f clt = %f pl = %f\n", cl1, cl2, clt, pl);
    ut_print_neperbug ();
  }

  clgrad = log (pl);

  nem_meshing_1d_useg_pieces (cl1, cl2, clt, clgrad, x, a, b, F, &pieceqty);
  nstar = F[pieceqty];

  // number of elements: the one giving q the closest to 1, among the
  // integers around n* (the elts of the parts of increasing s are a few
  // percent shorter than s at their centres, for n = n*)
  nmin = ut_num_max ((int) floor (nstar) - 1, 1);
  nmax = (int) ceil (nstar) + 1;

  (*pcoo) = ut_alloc_1d (nmax + 1);
  n = -1;
  q = 0;
  for (k = nmin; k <= nmax; k++)
  {
    nem_meshing_1d_useg_place (x, a, b, F, pieceqty, k, *pcoo);
    qn = nem_meshing_1d_useg_sizeratio (cl1, cl2, clt, clgrad, *pcoo, k);
    if (n == -1 || fabs (log (qn)) < fabs (log (q)))
    {
      n = k;
      q = qn;
    }
  }

  // node coordinates
  nem_meshing_1d_useg_place (x, a, b, F, pieceqty, n, *pcoo);

  // node characteristic lengths
  (*pcl) = ut_alloc_1d (n + 1);
  for (k = 0; k <= n; k++)
    (*pcl)[k] = q * nem_meshing_1d_useg_size (cl1, cl2, clt, clgrad, (*pcoo)[k]);

  (*pqty) = n - 1;

  return;
}
