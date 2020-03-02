/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_point_fscanf_.h"

void
neut_point_fscanf (FILE * file, struct POINT *pPoint)
{
  (*pPoint).PointQty = ut_file_nbwords_pointer (file) / 3;
  (*pPoint).Dim = 3;

  (*pPoint).PointCoo = ut_alloc_2d ((*pPoint).PointQty + 1, 3);
  ut_array_2d_fscanf (file, (*pPoint).PointCoo + 1, (*pPoint).PointQty,
                      (*pPoint).Dim);

  return;
}

void
neut_point_fnscanf (char *filename, struct POINT *pPoint)
{
  FILE *file = ut_file_open (filename, "r");

  neut_point_fscanf (file, pPoint);
  ut_file_close (file, filename, "r");

  return;
}
