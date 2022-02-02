/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_utils_.h"

int
nev_print_png_cylinder_test (double *coo1, double *coo2, char *edge_rad)
{
  double rad, dist = ut_space_dist (coo1, coo2);

  if (!edge_rad || (sscanf (edge_rad, "%lf", &rad) == 1 && rad <= 0)
      || dist < 1e-9)
    return 0;
  else
    return 1;
}
