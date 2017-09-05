/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the tgeom-level directory. */

#include "neut_tesr_geom_.h"

int
neut_tesr_cell_boundpoints_test (struct TESR Tesr, int cell, int i, int j,
				 int k, int connec, int interior)
{
  if (Tesr.Dim == 2)
    return neut_tesr_cell_boundpoints_test_2d (Tesr, cell, i, j, connec, interior);
  else if (Tesr.Dim == 3)
    return neut_tesr_cell_boundpoints_test_3d (Tesr, cell, i, j, k, connec, interior);
  else
    abort ();

  return -1;
}

int
neut_tesr_cell_boundpoints_test_3d (struct TESR Tesr, int cell, int i, int j,
				    int k, int connec, int interior)
{
  int res = -1, min = interior == 1 ? 0 : -INT_MAX;

  // interior = 1 means that we consider only grain boundaries, not external
  // surface boundaries.

  // The lower res is, the more it is a surface voxel:
  // -1: has a  face  -neighbour different than cell;
  //  0: has an edge  -neighbour different than cell;
  //  1: has a  vertex-neighbour different than cell;
  //  2: has no        neighbour different than cell.

  if (connec < 0 || connec > 2)
    ut_print_message (2, 0, "connectivity = %d!\n", connec);

  if ((Tesr.VoxCell[i - 1][j][k] == cell || Tesr.VoxCell[i - 1][j][k] <= min)
   && (Tesr.VoxCell[i + 1][j][k] == cell || Tesr.VoxCell[i + 1][j][k] <= min)
   && (Tesr.VoxCell[i][j - 1][k] == cell || Tesr.VoxCell[i][j - 1][k] <= min)
   && (Tesr.VoxCell[i][j + 1][k] == cell || Tesr.VoxCell[i][j + 1][k] <= min)
   && (Tesr.VoxCell[i][j][k - 1] == cell || Tesr.VoxCell[i][j][k - 1] <= min)
   && (Tesr.VoxCell[i][j][k + 1] == cell || Tesr.VoxCell[i][j][k + 1] <= min))
    res = 0;

  if (res == 0 && connec > 0)
  {
    if ((Tesr.VoxCell[i + 1][j - 1][k] == cell || Tesr.VoxCell[i + 1][j - 1][k] <= min)
     && (Tesr.VoxCell[i + 1][j + 1][k] == cell || Tesr.VoxCell[i + 1][j + 1][k] <= min)
     && (Tesr.VoxCell[i - 1][j - 1][k] == cell || Tesr.VoxCell[i - 1][j - 1][k] <= min)
     && (Tesr.VoxCell[i - 1][j + 1][k] == cell || Tesr.VoxCell[i - 1][j + 1][k] <= min)
     && (Tesr.VoxCell[i + 1][j][k - 1] == cell || Tesr.VoxCell[i + 1][j][k - 1] <= min)
     && (Tesr.VoxCell[i + 1][j][k + 1] == cell || Tesr.VoxCell[i + 1][j][k + 1] <= min)
     && (Tesr.VoxCell[i - 1][j][k - 1] == cell || Tesr.VoxCell[i - 1][j][k - 1] <= min)
     && (Tesr.VoxCell[i - 1][j][k + 1] == cell || Tesr.VoxCell[i - 1][j][k + 1] <= min)
     && (Tesr.VoxCell[i][j + 1][k - 1] == cell || Tesr.VoxCell[i][j + 1][k - 1] <= min)
     && (Tesr.VoxCell[i][j + 1][k + 1] == cell || Tesr.VoxCell[i][j + 1][k + 1] <= min)
     && (Tesr.VoxCell[i][j - 1][k - 1] == cell || Tesr.VoxCell[i][j - 1][k - 1] <= min)
     && (Tesr.VoxCell[i][j - 1][k + 1] == cell || Tesr.VoxCell[i][j - 1][k + 1] <= min))
      res = 1;
  }

  if (res == 1 && connec > 1)
  {
    if ((Tesr.VoxCell[i - 1][j - 1][k - 1] == cell || Tesr.VoxCell[i - 1][j - 1][k - 1] <= min)
     && (Tesr.VoxCell[i - 1][j - 1][k + 1] == cell || Tesr.VoxCell[i - 1][j - 1][k + 1] <= min)
     && (Tesr.VoxCell[i - 1][j + 1][k - 1] == cell || Tesr.VoxCell[i - 1][j + 1][k - 1] <= min)
     && (Tesr.VoxCell[i - 1][j + 1][k + 1] == cell || Tesr.VoxCell[i - 1][j + 1][k + 1] <= min)
     && (Tesr.VoxCell[i + 1][j - 1][k - 1] == cell || Tesr.VoxCell[i + 1][j - 1][k - 1] <= min)
     && (Tesr.VoxCell[i + 1][j - 1][k + 1] == cell || Tesr.VoxCell[i + 1][j - 1][k + 1] <= min)
     && (Tesr.VoxCell[i + 1][j + 1][k - 1] == cell || Tesr.VoxCell[i + 1][j + 1][k - 1] <= min)
     && (Tesr.VoxCell[i + 1][j + 1][k + 1] == cell || Tesr.VoxCell[i + 1][j + 1][k + 1] <= min))
      res = 2;
  }

  return res != connec;
}

int
neut_tesr_cell_boundpoints_test_2d (struct TESR Tesr, int cell, int i, int j,
				    int connec, int interior)
{
  int res = -1, min = interior == 1 ? 0 : INT_MAX;

  if (connec < 0 || connec > 2)
    ut_print_message (2, 0, "connectivity = %d!\n", connec);

  if (   (Tesr.VoxCell[i - 1][j][1] == cell || Tesr.VoxCell[i - 1][j][1] <= min)
      && (Tesr.VoxCell[i + 1][j][1] == cell || Tesr.VoxCell[i + 1][j][1] <= min)
      && (Tesr.VoxCell[i][j - 1][1] == cell || Tesr.VoxCell[i][j - 1][1] <= min)
      && (Tesr.VoxCell[i][j + 1][1] == cell || Tesr.VoxCell[i][j + 1][1] <= min))
    res = 0;

  if (res == 0 && connec > 0)
  {
    if (   (Tesr.VoxCell[i + 1][j - 1][1] == cell || Tesr.VoxCell[i + 1][j - 1][1] <= min)
	&& (Tesr.VoxCell[i + 1][j + 1][1] == cell || Tesr.VoxCell[i + 1][j + 1][1] <= min)
	&& (Tesr.VoxCell[i - 1][j - 1][1] == cell || Tesr.VoxCell[i - 1][j - 1][1] <= min)
	&& (Tesr.VoxCell[i - 1][j + 1][1] == cell || Tesr.VoxCell[i - 1][j + 1][1] <= min))
      res = 1;
  }

  return res != connec;
}
