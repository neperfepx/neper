/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include "ol_map_png.h"

#ifdef HAVE_PNG

void
ol_map_png (struct OL_MAP Map, char *filename)
{
  unsigned int i, j;

  png_infop info_ptr;
  png_bytep *row_pointers;
  png_structp png_ptr;

  ut_png_alloc (Map.xsize, Map.ysize, &png_ptr, &info_ptr, &row_pointers);

  for (j = 0; j < Map.ysize; j++)
    for (i = 0; i < Map.xsize; i++)
      ut_png_pixel_set (&row_pointers, i, j, Map.rgb[i][j]);

  ut_png_name_fprintf (filename, png_ptr, info_ptr, row_pointers);

  ut_png_free (png_ptr, info_ptr, row_pointers);

  return;
}

void
ol_map_rgb_png (unsigned int xsize, unsigned int ysize, int ***rgb,
                char *filename)
{
  unsigned int i, j;

  png_infop info_ptr;
  png_bytep *row_pointers;
  png_structp png_ptr;

  ut_png_alloc (xsize, ysize, &png_ptr, &info_ptr, &row_pointers);

  for (j = 0; j < ysize; j++)
    for (i = 0; i < xsize; i++)
      ut_png_pixel_set (&row_pointers, i, j, rgb[i][j]);

  ut_png_name_fprintf (filename, png_ptr, info_ptr, row_pointers);

  ut_png_free (png_ptr, info_ptr, row_pointers);

  return;
}

#endif
