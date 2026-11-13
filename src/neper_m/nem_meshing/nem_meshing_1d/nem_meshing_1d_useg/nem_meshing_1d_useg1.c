/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1d_useg_.h"

// Mesh size map along a unit segment:
//   s(x) = min (clt, cl1 + clgrad x, cl2 + clgrad (1 - x)),
// which is piecewise linear.  nem_meshing_1d_useg_pieces returns its pieces,
// as s(x) = a[i] + b[i] x on [x[i], x[i + 1]] (i = 0 to *pqty - 1), and the
// values of F(x) = int_0^x dt / s(t) at the piece bounds, F[i] (i = 0 to
// *pqty).  Arrays must be of size 6.
void
nem_meshing_1d_useg_pieces (double cl1, double cl2, double clt, double clgrad,
                            double *x, double *a, double *b, double *F,
                            int *pqty)
{
  int i, qty = 0;
  double t[5], u, v, m, s1, s2;

  // candidate piece bounds: ends of the segment, points where the increases
  // from both ends reach clt, and point where they meet
  t[qty++] = 0;
  t[qty++] = 1;
  if (clgrad > 0)
  {
    t[qty++] = (clt - cl1) / clgrad;
    t[qty++] = 1 - (clt - cl2) / clgrad;
    t[qty++] = 0.5 * (1 + (cl2 - cl1) / clgrad);
  }

  ut_array_1d_sort (t, qty);

  (*pqty) = 0;
  x[0] = 0;
  F[0] = 0;
  for (i = 0; i < qty - 1; i++)
  {
    u = ut_num_bound (t[i], 0, 1);
    v = ut_num_bound (t[i + 1], 0, 1);
    if (v - u < 1e-12)
      continue;

    // linear function of the minimum on the piece
    m = 0.5 * (u + v);
    s1 = cl1 + clgrad * m;
    s2 = cl2 + clgrad * (1 - m);
    if (clt <= s1 && clt <= s2)
    {
      a[*pqty] = clt;
      b[*pqty] = 0;
    }
    else if (s1 <= s2)
    {
      a[*pqty] = cl1;
      b[*pqty] = clgrad;
    }
    else
    {
      a[*pqty] = cl2 + clgrad;
      b[*pqty] = -clgrad;
    }

    x[*pqty + 1] = v;
    F[*pqty + 1] = F[*pqty] + nem_meshing_1d_useg_piece_int (a[*pqty], b[*pqty], u, v);
    (*pqty)++;
  }

  return;
}

// int_u^v dt / (a + b t)
double
nem_meshing_1d_useg_piece_int (double a, double b, double u, double v)
{
  if (fabs (b) < 1e-12)
    return (v - u) / a;
  else
    return log ((a + b * v) / (a + b * u)) / b;
}

// x such that int_u^x dt / (a + b t) = dF
double
nem_meshing_1d_useg_piece_intinv (double a, double b, double u, double dF)
{
  if (fabs (b) < 1e-12)
    return u + a * dF;
  else
    return ((a + b * u) * exp (b * dF) - a) / b;
}

// value of the mesh size map
double
nem_meshing_1d_useg_size (double cl1, double cl2, double clt, double clgrad,
                          double x)
{
  return ut_num_min (clt, ut_num_min (cl1 + clgrad * x, cl2 + clgrad * (1 - x)));
}

// Placing the n + 1 nodes of a unit segment (coo, of size n + 1) so that the
// integral of 1 / s is the same over all elements, from the pieces of the
// mesh size map (see nem_meshing_1d_useg_pieces)
void
nem_meshing_1d_useg_place (double *x, double *a, double *b, double *F,
                           int pieceqty, int n, double *coo)
{
  int i = 0, k;
  double r = F[pieceqty] / n;

  coo[0] = 0;
  coo[n] = 1;
  for (k = 1; k < n; k++)
  {
    while (i < pieceqty - 1 && F[i + 1] < k * r)
      i++;
    coo[k] = nem_meshing_1d_useg_piece_intinv (a[i], b[i], x[i], k * r - F[i]);
  }

  return;
}

// Mean ratio of the elt lengths to the mesh size map at the elt centres (as
// for the 2D and 3D meshes, see nem_mesh_gmsh_sizeratio)
double
nem_meshing_1d_useg_sizeratio (double cl1, double cl2, double clt,
                               double clgrad, double *coo, int n)
{
  int k;
  double sum = 0;

  for (k = 0; k < n; k++)
    sum += (coo[k + 1] - coo[k])
      / nem_meshing_1d_useg_size (cl1, cl2, clt, clgrad, 0.5 * (coo[k] + coo[k + 1]));

  return sum / n;
}
