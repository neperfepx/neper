/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_g.h"

double **
ol_g_alloc ()
{
  double **g = ut_alloc_2d (3, 3);

  g[0][0] = 1;
  g[1][1] = 1;
  g[2][2] = 1;

  return g;
}

void
ol_g_free (double **g)
{
  if (!g)
    return;
  else
    ut_free_2d (g, 3);

  return;
}

void
ol_g_set_zero (double **g)
{
  ut_array_2d_zero (g, 3, 3);

  return;
}

void
ol_g_set_id (double **g)
{
  ut_array_2d_zero (g, 3, 3);
  g[0][0] = 1;
  g[1][1] = 1;
  g[2][2] = 1;

  return;
}

void
ol_g_set_this (double **g, double g11, double g12, double g13, double g21, double g22, double g23, double g31, double g32, double g33)
{
  g[0][0] = g11;
  g[0][1] = g12;
  g[0][2] = g13;
  g[1][0] = g21;
  g[1][1] = g22;
  g[1][2] = g23;
  g[2][0] = g31;
  g[2][1] = g32;
  g[2][2] = g33;

  return;
}

void
ol_g_memcpy (double **gsrc, double **gdest)
{
  ut_array_2d_memcpy (gdest, 3, 3, gsrc);

  return;
}

int
ol_g_fscanf (FILE * file, double **g)
{
  if (fscanf (file, "%lf%lf%lf%lf%lf%lf%lf%lf%lf",
	      &g[0][0], &g[0][1], &g[0][2],
	      &g[1][0], &g[1][1], &g[1][2],
	      &g[2][0], &g[2][1], &g[2][2]) == 9)
    return 1;
  else
    return 0;
}

int
ol_g_fprintf (FILE * file, double **g, char *format)
{
  int i, j, res;

  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      fprintf (file, format, g[i][j]);
      fprintf (file, " ");
    }
    res = fprintf (file, "\n");
  }

  return res;
}
