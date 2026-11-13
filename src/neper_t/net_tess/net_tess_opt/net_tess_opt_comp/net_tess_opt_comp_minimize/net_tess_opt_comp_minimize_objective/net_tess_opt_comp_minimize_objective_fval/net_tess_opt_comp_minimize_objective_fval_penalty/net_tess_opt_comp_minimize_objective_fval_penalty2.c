/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_minimize_objective_fval_penalty_.h"

// Distance between two sets of polyhedra (minimum distance between two of
// their polyhedra), considering the periodic images of the second set
double
net_tess_opt_comp_minimize_objective_fval_penalty_setdist (struct TOPT *pTOpt,
                                                           int *polys1,
                                                           int polyqty1,
                                                           int *polys2,
                                                           int polyqty2)
{
  int i, j, k, d, sx, sy, sz, s[3], qty;
  double dist, mindist = DBL_MAX, shift[3], **coos = NULL;
  int smax[3];

  for (d = 0; d < 3; d++)
    smax[d] = ((*pTOpt).SSet.Periodic && (*pTOpt).SSet.Periodic[d]) ? 1 : 0;

  for (j = 0; j < polyqty2; j++)
  {
    struct POLY Poly2 = (*pTOpt).Poly[polys2[j]];

    qty = Poly2.VerQty;
    coos = ut_alloc_2d (qty, 3);

    for (sx = -smax[0]; sx <= smax[0]; sx++)
      for (sy = -smax[1]; sy <= smax[1]; sy++)
        for (sz = -smax[2]; sz <= smax[2]; sz++)
        {
          ut_array_1d_int_set_3 (s, sx, sy, sz);
          for (d = 0; d < 3; d++)
            shift[d] = s[d] ? s[d] * (*pTOpt).SSet.PeriodicDist[d] : 0;

          for (k = 0; k < qty; k++)
            ut_array_1d_add (Poly2.VerCoo[k + 1], shift, 3, coos[k]);

          for (i = 0; i < polyqty1; i++)
          {
            struct POLY Poly1 = (*pTOpt).Poly[polys1[i]];

            ut_space_polypts_polypts_dist (Poly1.VerCoo + 1, Poly1.VerQty,
                                           coos, qty, &dist);
            mindist = ut_num_min (mindist, dist);
          }
        }

    ut_free_2d (&coos, qty);
  }

  return mindist;
}

// Distance needed to make a non-contiguous cell contiguous: total length
// of the minimum spanning tree of its contiguous parts (sets of
// polyhedra), the length of an edge being the distance between the two
// parts
double
net_tess_opt_comp_minimize_objective_fval_penalty_noncontiguous (struct TOPT
                                                                 *pTOpt,
                                                                 int **setpoly,
                                                                 int *setpolyqty,
                                                                 int setqty)
{
  int i, j, next;
  double total = 0, **dist = ut_alloc_2d (setqty, setqty);
  double *key = ut_alloc_1d (setqty);
  int *intree = ut_alloc_1d_int (setqty);

  for (i = 0; i < setqty; i++)
    for (j = i + 1; j < setqty; j++)
    {
      dist[i][j] =
        net_tess_opt_comp_minimize_objective_fval_penalty_setdist (pTOpt,
                                                                   setpoly[i],
                                                                   setpolyqty[i],
                                                                   setpoly[j],
                                                                   setpolyqty[j]);
      dist[j][i] = dist[i][j];
    }

  // Prim's algorithm, from part 0
  intree[0] = 1;
  for (i = 1; i < setqty; i++)
    key[i] = dist[0][i];

  for (j = 1; j < setqty; j++)
  {
    next = -1;
    for (i = 1; i < setqty; i++)
      if (!intree[i] && (next == -1 || key[i] < key[next]))
        next = i;

    intree[next] = 1;
    total += key[next];

    for (i = 1; i < setqty; i++)
      if (!intree[i])
        key[i] = ut_num_min (key[i], dist[next][i]);
  }

  ut_free_2d (&dist, setqty);
  ut_free_1d (&key);
  ut_free_1d_int (&intree);

  return total;
}
