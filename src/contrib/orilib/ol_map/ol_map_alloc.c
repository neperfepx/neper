/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_map_alloc.h"

struct OL_MAP
ol_map_alloc (size_t xsize, size_t ysize, double stepsize, char *crysym)
{
  struct OL_MAP Map;

  Map.xsize = xsize;
  Map.ysize = ysize;
  Map.stepsize = stepsize;

  Map.gmap = ol_g_alloc ();
  ol_g_set_id (Map.gmap);

  Map.id = ut_alloc_2d_uint (xsize, ysize);
  Map.q = ut_alloc_3d (xsize, ysize, 4);

  if (crysym)
  {
    Map.crysym = ut_alloc_1d_char (strlen (crysym) + 1);
    strcpy (Map.crysym, crysym);
  }
  else
    Map.crysym = NULL;

  return Map;
}

void
ol_map_free (struct OL_MAP *pMap)
{
  ut_free_2d_uint (&(*pMap).id, (*pMap).xsize);
  ut_free_3d (&(*pMap).q, (*pMap).xsize, (*pMap).ysize);
  ol_g_free (&(*pMap).gmap);
  ut_free_1d_char (&(*pMap).crysym);

  (*pMap).xsize = 0;
  (*pMap).ysize = 0;
  (*pMap).stepsize = 0;

  return;
}
