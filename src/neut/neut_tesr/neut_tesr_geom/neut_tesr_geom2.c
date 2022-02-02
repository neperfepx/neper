/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the tgeom-level directory. */

#include "neut_tesr_geom_.h"

int
neut_tesr_cell_boundpoints_test (struct TESR Tesr, int cell, int i, int j,
                                 int k, int connec, char* interior)
{
  if (Tesr.Dim == 2)
    return neut_tesr_cell_boundpoints_test_2d (Tesr, cell, i, j, connec,
                                               interior);
  else if (Tesr.Dim == 3)
    return neut_tesr_cell_boundpoints_test_3d (Tesr, cell, i, j, k, connec,
                                               interior);
  else
    abort ();

  return -1;
}

int
neut_tesr_cell_boundpoints_test_3d (struct TESR Tesr, int cell, int i, int j,
                                    int k, int connec, char* interior)
{
  int res = -1;
  int minx = interior && strstr (interior, "x") ? 0 : -INT_MAX;
  int miny = interior && strstr (interior, "y") ? 0 : -INT_MAX;
  int minz = interior && strstr (interior, "z") ? 0 : -INT_MAX;
  int minxy = ut_num_max_int (minx, miny);
  int minyz = ut_num_max_int (miny, minz);
  int minzx = ut_num_max_int (minz, minx);
  int minxyz = ut_num_max_int (minxy, minyz);

  // interior = 1 means that we consider only grain boundaries, not external
  // surface boundaries.

  // The lower res is, the more it is a surface voxel:
  // -1: has a  face  -neighbor different than cell;
  //  0: has an edge  -neighbor different than cell;
  //  1: has a  vertex-neighbor different than cell;
  //  2: has no        neighbor different than cell.

  if (connec < 0 || connec > 2)
    ut_print_message (2, 0, "connectivity = %d!\n", connec);

  if ((Tesr.VoxCell[i - 1][j][k] == cell || Tesr.VoxCell[i - 1][j][k] <= minx)
      && (Tesr.VoxCell[i + 1][j][k] == cell
          || Tesr.VoxCell[i + 1][j][k] <= minx)
      && (Tesr.VoxCell[i][j - 1][k] == cell
          || Tesr.VoxCell[i][j - 1][k] <= miny)
      && (Tesr.VoxCell[i][j + 1][k] == cell
          || Tesr.VoxCell[i][j + 1][k] <= miny)
      && (Tesr.VoxCell[i][j][k - 1] == cell
          || Tesr.VoxCell[i][j][k - 1] <= minz)
      && (Tesr.VoxCell[i][j][k + 1] == cell
          || Tesr.VoxCell[i][j][k + 1] <= minz))
    res = 0;

  if (res == 0 && connec > 0)
  {
    if ((Tesr.VoxCell[i + 1][j - 1][k] == cell
         || Tesr.VoxCell[i + 1][j - 1][k] <= minxy)
        && (Tesr.VoxCell[i + 1][j + 1][k] == cell
            || Tesr.VoxCell[i + 1][j + 1][k] <= minxy)
        && (Tesr.VoxCell[i - 1][j - 1][k] == cell
            || Tesr.VoxCell[i - 1][j - 1][k] <= minxy)
        && (Tesr.VoxCell[i - 1][j + 1][k] == cell
            || Tesr.VoxCell[i - 1][j + 1][k] <= minxy)
        && (Tesr.VoxCell[i + 1][j][k - 1] == cell
            || Tesr.VoxCell[i + 1][j][k - 1] <= minzx)
        && (Tesr.VoxCell[i + 1][j][k + 1] == cell
            || Tesr.VoxCell[i + 1][j][k + 1] <= minzx)
        && (Tesr.VoxCell[i - 1][j][k - 1] == cell
            || Tesr.VoxCell[i - 1][j][k - 1] <= minzx)
        && (Tesr.VoxCell[i - 1][j][k + 1] == cell
            || Tesr.VoxCell[i - 1][j][k + 1] <= minzx)
        && (Tesr.VoxCell[i][j + 1][k - 1] == cell
            || Tesr.VoxCell[i][j + 1][k - 1] <= minyz)
        && (Tesr.VoxCell[i][j + 1][k + 1] == cell
            || Tesr.VoxCell[i][j + 1][k + 1] <= minyz)
        && (Tesr.VoxCell[i][j - 1][k - 1] == cell
            || Tesr.VoxCell[i][j - 1][k - 1] <= minyz)
        && (Tesr.VoxCell[i][j - 1][k + 1] == cell
            || Tesr.VoxCell[i][j - 1][k + 1] <= minyz))
      res = 1;
  }

  if (res == 1 && connec > 1)
  {
    if ((Tesr.VoxCell[i - 1][j - 1][k - 1] == cell
         || Tesr.VoxCell[i - 1][j - 1][k - 1] <= minxyz)
        && (Tesr.VoxCell[i - 1][j - 1][k + 1] == cell
            || Tesr.VoxCell[i - 1][j - 1][k + 1] <= minxyz)
        && (Tesr.VoxCell[i - 1][j + 1][k - 1] == cell
            || Tesr.VoxCell[i - 1][j + 1][k - 1] <= minxyz)
        && (Tesr.VoxCell[i - 1][j + 1][k + 1] == cell
            || Tesr.VoxCell[i - 1][j + 1][k + 1] <= minxyz)
        && (Tesr.VoxCell[i + 1][j - 1][k - 1] == cell
            || Tesr.VoxCell[i + 1][j - 1][k - 1] <= minxyz)
        && (Tesr.VoxCell[i + 1][j - 1][k + 1] == cell
            || Tesr.VoxCell[i + 1][j - 1][k + 1] <= minxyz)
        && (Tesr.VoxCell[i + 1][j + 1][k - 1] == cell
            || Tesr.VoxCell[i + 1][j + 1][k - 1] <= minxyz)
        && (Tesr.VoxCell[i + 1][j + 1][k + 1] == cell
            || Tesr.VoxCell[i + 1][j + 1][k + 1] <= minxyz))
      res = 2;
  }

  return res != connec;
}

int
neut_tesr_cell_boundpoints_test_2d (struct TESR Tesr, int cell, int i, int j,
                                    int connec, char* interior)
{
  int res = -1;
  int minx = interior && strstr (interior, "x") ? 0 : -INT_MAX;
  int miny = interior && strstr (interior, "y") ? 0 : -INT_MAX;
  int minxy = ut_num_max_int (minx, miny);

  if (connec < 0 || connec > 2)
    ut_print_message (2, 0, "connectivity = %d!\n", connec);

  if ((Tesr.VoxCell[i - 1][j][1] == cell || Tesr.VoxCell[i - 1][j][1] <= minx)
      && (Tesr.VoxCell[i + 1][j][1] == cell
          || Tesr.VoxCell[i + 1][j][1] <= minx)
      && (Tesr.VoxCell[i][j - 1][1] == cell
          || Tesr.VoxCell[i][j - 1][1] <= miny)
      && (Tesr.VoxCell[i][j + 1][1] == cell
          || Tesr.VoxCell[i][j + 1][1] <= miny))
    res = 0;

  if (res == 0 && connec > 0)
  {
    if ((Tesr.VoxCell[i + 1][j - 1][1] == cell
         || Tesr.VoxCell[i + 1][j - 1][1] <= minxy)
        && (Tesr.VoxCell[i + 1][j + 1][1] == cell
            || Tesr.VoxCell[i + 1][j + 1][1] <= minxy)
        && (Tesr.VoxCell[i - 1][j - 1][1] == cell
            || Tesr.VoxCell[i - 1][j - 1][1] <= minxy)
        && (Tesr.VoxCell[i - 1][j + 1][1] == cell
            || Tesr.VoxCell[i - 1][j + 1][1] <= minxy))
      res = 1;
  }

  return res != connec;
}
