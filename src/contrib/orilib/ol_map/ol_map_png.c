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

  ut_png_init_png_file_rgb (Map.xsize, Map.ysize, &png_ptr, &info_ptr,
		     &row_pointers);

  for (j = 0; j < Map.ysize; j++)
    for (i = 0; i < Map.xsize; i++)
      ut_png_set_png_pixel (i, j, Map.rgb[i][j], &row_pointers);

  ut_png_write_png_file (png_ptr, info_ptr, row_pointers, filename);

  ut_png_free_png_file (png_ptr, info_ptr, row_pointers);

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

  ut_png_init_png_file_rgb (xsize, ysize, &png_ptr, &info_ptr, &row_pointers);

  for (j = 0; j < ysize; j++)
    for (i = 0; i < xsize; i++)
      ut_png_set_png_pixel (i, j, rgb[i][j], &row_pointers);

  ut_png_write_png_file (png_ptr, info_ptr, row_pointers, filename);

  ut_png_free_png_file (png_ptr, info_ptr, row_pointers);

  return;
}

#endif
