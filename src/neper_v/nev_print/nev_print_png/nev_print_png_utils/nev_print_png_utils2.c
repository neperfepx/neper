/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_utils_.h"

int
nev_print_png_cylinder_test (double *coo1, double *coo2, double rad)
{
  return !(rad <= 0 || ut_space_dist (coo1, coo2) < 1e-9);
}
