/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include"ut.h"

#ifdef HAVE_PNG
#include <png.h>

void
ut_png_alloc (int sizex, int sizey, png_structp * ppng_ptr,
              png_infop * pinfo_ptr, png_bytep ** prow_ptr)
{
  unsigned int y;

  (*ppng_ptr) =
    png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  (*pinfo_ptr) = png_create_info_struct (*ppng_ptr);

  /* image parameters */
  // (*pinfo_ptr)->width = sizex;
  // (*pinfo_ptr)->height = sizey;
  // (*pinfo_ptr)->color_type = PNG_COLOR_TYPE_RGB;
  // (*pinfo_ptr)->bit_depth = 8;
  // (*pinfo_ptr)->rowbytes = (*pinfo_ptr)->width * 4;
  png_set_IHDR (*ppng_ptr, *pinfo_ptr, sizex, sizey, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);

  /* allocation */
  (*prow_ptr) = (png_bytep *) malloc (sizeof (png_bytep) * sizey);
  for (y = 0; y < (unsigned int) sizey; y++)
    (*prow_ptr)[y] = (png_byte *) malloc (4 * sizex);

  return;
}

void
ut_png_pixel_set (png_bytep ** prow_ptr, int x, int y, int *rgb)
{
  int i;
  png_byte *row;
  png_byte *ptr;

  row = (*prow_ptr)[y];
  ptr = &(row[x * 3]);

  for (i = 0; i < 3; i++)
    ptr[i] = rgb[i];

  return;
}

void
ut_png_name_fscanf (char *filename, png_structp * ppng_ptr,
                    png_infop * pinfo_ptr, png_bytep ** prow_ptr)
{
  int i;
  unsigned char header[8];      // 8 is the maximum size that can be checked
  int height, width;
  // png_byte color_type;
  // png_byte bit_depth;

  /* open file and test for it being a png */
  FILE *fp = fopen (filename, "rb");
  if (!fp)
    ut_print_message (2, 0, "Failed to open file `%s' for reading.\n",
                      filename);

  int status = fread (header, 1, 8, fp);
  if (status != 8 || png_sig_cmp (header, 0, 8))
    ut_print_message (2, 0, "File `%s' is not a PNG file.\n", filename);

  /* initialize stuff */
  (*ppng_ptr) =
    png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!(*ppng_ptr))
    ut_print_message (2, 0, "png_create_read_struct failed.\n");

  (*pinfo_ptr) = png_create_info_struct ((*ppng_ptr));
  if (!(*pinfo_ptr))
    ut_print_message (2, 0, "png_create_info_struct failed.\n");

  if (setjmp (png_jmpbuf ((*ppng_ptr))))
    ut_print_message (2, 0, "init_io failed.\n");

  png_init_io ((*ppng_ptr), fp);
  png_set_sig_bytes ((*ppng_ptr), 16);

  png_read_info ((*ppng_ptr), (*pinfo_ptr));

  // width = (*pinfo_ptr)->width;
  height = png_get_image_height ((*ppng_ptr), (*pinfo_ptr));
  width = png_get_image_width ((*ppng_ptr), (*pinfo_ptr));
  // color_type = (*pinfo_ptr)->color_type;
  // bit_depth = (*pinfo_ptr)->bit_depth;

  // number_of_passes = png_set_interlace_handling ((*ppng_ptr));
  png_set_interlace_handling ((*ppng_ptr));
  png_read_update_info ((*ppng_ptr), (*pinfo_ptr));


  /* read file */
  if (setjmp (png_jmpbuf ((*ppng_ptr))))
    ut_print_message (2, 0, "read_image failed.\n");

  (*prow_ptr) = (png_bytep *) malloc (sizeof (png_bytep) * height);
  for (i = 0; i < height; i++)
    (*prow_ptr)[i] = (png_byte *) malloc (4 * width);

  png_read_image ((*ppng_ptr), (*prow_ptr));

  fclose (fp);
}

void
ut_png_name_fprintf (char *filename, png_structp png_ptr, png_infop info_ptr,
                     png_bytep * row_ptr)
{
  /* create file */
  FILE *fp = fopen (filename, "wb");

  png_init_io (png_ptr, fp);

  /* write header */
  if (setjmp (png_jmpbuf (png_ptr)))
    abort ();

  png_write_info (png_ptr, info_ptr);

  /* write bytes */
  if (setjmp (png_jmpbuf (png_ptr)))
    abort ();

  png_write_image (png_ptr, row_ptr);

  /* end write */
  if (setjmp (png_jmpbuf (png_ptr)))
    abort ();

  png_write_end (png_ptr, NULL);

  /* cleanup heap allocation */

  fclose (fp);
}

void
ut_png_free (png_structp png_ptr, png_infop info_ptr, png_bytep * row_ptr)
{
  unsigned int y;
  unsigned int height = png_get_image_height (png_ptr, info_ptr);

  for (y = 0; y < height; y++)
    free (row_ptr[y]);
  free (row_ptr);

  return;
}

#endif // HAVE_PNG
