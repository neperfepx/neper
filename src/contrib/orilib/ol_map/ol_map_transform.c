/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_map_transform.h"

void
ol_map_transformparam (struct OL_MAP Map, double **F, size_t * pxsize,
                       size_t * pysize, double *S)
{
  int i;
  double xmin, xmax, ymin, ymax;

  /* C = corners of the new map (before translation) */
  double **C = ut_alloc_2d (4, 2);
  double *tmp = ut_alloc_1d (4);

  /* C = F oldC */
  C[0][0] = 0;
  C[0][1] = 0;

  C[1][0] = F[0][0] * Map.xsize;
  C[1][1] = F[1][0] * Map.xsize;

  C[2][0] = F[0][1] * Map.ysize;
  C[2][1] = F[1][1] * Map.ysize;

  C[3][0] = F[0][0] * Map.xsize + F[0][1] * Map.ysize;
  C[3][1] = F[1][0] * Map.xsize + F[1][1] * Map.ysize;

  /* searching bounds */

  for (i = 0; i < 4; i++)
    tmp[i] = C[i][0];
  xmin = ut_array_1d_min (tmp, 4);
  xmax = ut_array_1d_max (tmp, 4);

  for (i = 0; i < 4; i++)
    tmp[i] = C[i][1];
  ymin = ut_array_1d_min (tmp, 4);
  ymax = ut_array_1d_max (tmp, 4);

  /* xsize, ysize, S */

  (*pxsize) = (size_t) ut_num_d2ri (xmax - xmin);
  (*pysize) = (size_t) ut_num_d2ri (ymax - ymin);

  S[0] = -xmin;
  S[1] = -ymin;

  ut_free_1d (&tmp);
  ut_free_2d (&C, 4);

  return;
}

void
ol_map_transform (struct OL_MAP Map, double **F, size_t xsize, size_t ysize,
                  double *S, struct OL_MAP *pMap2)
{
  size_t i, j;
  size_t x, y;
  double det;
  double **Fi = ut_alloc_2d (2, 2);     /* the inverse of F */

  (*pMap2).xsize = xsize;
  (*pMap2).ysize = ysize;

  det = F[0][0] * F[1][1] - F[0][1] * F[1][0];
  (*pMap2).stepsize = Map.stepsize / sqrt (det);
  (*pMap2) = ol_map_alloc ((*pMap2).xsize, (*pMap2).ysize,
                           (*pMap2).stepsize, Map.crysym);

  /* Calculating Fi = inverse(F).
   * determinant (F) = 0 ==> impossible transformation */
  if (ut_mat_inverse_22 (F, Fi) == -1)
    abort ();

  /* building new map from old data */
  for (i = 0; i < (*pMap2).xsize; i++)
    for (j = 0; j < (*pMap2).ysize; j++)
    {
      /* corresponding old coo */
      x = ut_num_d2ri (Fi[0][0] * (i - S[0]) + Fi[0][1] * (j - S[1]));
      y = ut_num_d2ri (Fi[1][0] * (i - S[0]) + Fi[1][1] * (j - S[1]));

      /* if out of the old map -> id = 0, else recording id and q */
      if (x > Map.xsize - 1 || y > Map.ysize - 1)
        (*pMap2).id[i][j] = 0;
      else
      {
        (*pMap2).id[i][j] = Map.id[x][y];
        ol_q_memcpy (Map.q[x][y], (*pMap2).q[i][j]);
      }
    }

  ut_free_2d (&Fi, 2);

  return;
}
