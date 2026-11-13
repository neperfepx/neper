/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_stdtriangle_.h"

void
net_domain_stdtriangle_compute_planes (struct PF Pf, double **eq)
{
  int i;
  double *tmp = ut_alloc_1d (3);
  double *pt1 = ut_alloc_1d (3);
  double *pt2 = ut_alloc_1d (3);

  ut_array_1d_set_4 (eq[0], 0, 0, 0, -1);
  ut_array_1d_set_4 (eq[1], 1e-6, 0, 0, 1);

  // ipfpts are 2D points (z = 0)
  for (i = 0; i < Pf.ipfptqty; i++)
  {
    ut_array_1d_memcpy (Pf.ipfpts[i], 2, pt1);
    ut_array_1d_memcpy (Pf.ipfpts[ut_array_1d_rotpos (0, Pf.ipfptqty - 1, i, 1)], 2,
                        pt2);
    ut_array_1d_memcpy (pt1, 3, tmp);
    tmp[2] += 1;
    ut_space_points_plane (pt1, pt2, tmp, eq[i + 2]);
  }
  ut_free_1d (&tmp);
  ut_free_1d (&pt1);
  ut_free_1d (&pt2);

  return;
}
