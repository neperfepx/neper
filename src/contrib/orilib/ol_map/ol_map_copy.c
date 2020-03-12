/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_map_copy.h"

void
ol_map_memcpy (struct OL_MAP Map1, struct OL_MAP *pMap2)
{
  size_t i, j, k;

  (*pMap2).stepsize = Map1.stepsize;
  (*pMap2).xsize = Map1.xsize;
  (*pMap2).ysize = Map1.ysize;
  (*pMap2).gmap = ol_g_alloc ();
  ol_g_memcpy (Map1.gmap, (*pMap2).gmap);

  for (i = 0; i < (*pMap2).xsize; i++)
    for (j = 0; j < (*pMap2).ysize; j++)
    {
      (*pMap2).id[i][j] = Map1.id[i][j];
      ol_q_memcpy (Map1.q[i][j], (*pMap2).q[i][j]);
      for (k = 0; k < 3; k++)
        (*pMap2).rgb[i][j][k] = Map1.rgb[i][j][k];
    }

  return;
}

void
ol_map_submap (struct OL_MAP Map1, int X0, int Y0, size_t xsize, size_t ysize,
               struct OL_MAP *pMap2)
{
  size_t i, j, k;

  (*pMap2).stepsize = Map1.stepsize;
  (*pMap2).xsize = xsize;
  (*pMap2).ysize = ysize;
  ol_g_memcpy (Map1.gmap, (*pMap2).gmap);
  if (Map1.crysym)
    ut_string_string (Map1.crysym, &((*pMap2).crysym));
  else
    ut_free_1d_char (&(*pMap2).crysym);

  for (i = 0; i < xsize; i++)
    for (j = 0; j < ysize; j++)
    {
      (*pMap2).id[i][j] = Map1.id[i + X0][j + Y0];
      ol_q_memcpy (Map1.q[i + X0][j + Y0], (*pMap2).q[i][j]);
      for (k = 0; k < 3; k++)
        (*pMap2).rgb[i][j][k] = Map1.rgb[i + X0][j + Y0][k];
    }

  return;
}

void
ol_map_submap_center (struct OL_MAP Map1, int X0, int Y0, size_t xsize,
                      size_t ysize, struct OL_MAP *pMap2)
{
  ol_map_submap (Map1, X0 - (xsize - 1) / 2, Y0 - (ysize - 1) / 2,
                 xsize, ysize, pMap2);

  return;
}

void
ol_map_set (struct OL_MAP Map, struct OL_SET *pSet)
{
  unsigned int i, j, size;
  int id;

  size = 0;
  for (i = 0; i < Map.xsize; i++)
    for (j = 0; j < Map.ysize; j++)
      if (Map.id[i][j] == 1)
        size++;

  (*pSet) = ol_set_alloc (size, Map.crysym);

  id = 0;
  for (i = 0; i < Map.xsize; i++)
    for (j = 0; j < Map.ysize; j++)
      if (Map.id[i][j] == 1)
      {
        ol_q_memcpy (Map.q[i][j], (*pSet).q[id]);
        (*pSet).weight[id] = 1;
        id++;
      }

  ut_string_string (Map.crysym, &((*pSet).crysym));

  return;
}
