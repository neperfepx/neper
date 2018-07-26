/* Copyright (C) 2003-2018, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include"ut.h"

#ifdef HAVE_PNG
#include <png.h>

void
ut_png_init_png_file_rgb (int sizex, int sizey, png_structp * ppng_ptr,
		   png_infop * pinfo_ptr, png_bytep ** prow_pointers)
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
  png_set_IHDR (*ppng_ptr, *pinfo_ptr, sizex, sizey, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  /* allocation */
  (*prow_pointers) =
    (png_bytep *) malloc (sizeof (png_bytep) * sizey);
  for (y = 0; y < (unsigned int) sizey; y++)
    (*prow_pointers)[y] = (png_byte *) malloc (4 * sizex);

  return;
}

void
ut_png_set_png_pixel (int x, int y, int *rgb, png_bytep ** prow_pointers)
{
  int i;
  png_byte *row;
  png_byte *ptr;

  row = (*prow_pointers)[y];
  ptr = &(row[x * 3]);

  for (i = 0; i < 3; i++)
    ptr[i] = rgb[i];

  return;
}


void
ut_png_read_png_file (png_structp* ppng_ptr,      png_infop* pinfo_ptr,
               png_bytep**  prow_pointers, char *file_name)
{
  int i;
  unsigned char header[8];  // 8 is the maximum size that can be checked
  int height, width;
  // png_byte color_type;
  // png_byte bit_depth;

  /* open file and test for it being a png */
  FILE *fp = fopen (file_name, "rb");
  if (!fp)
  {
    printf ("[ut_png_read_png_file] File %s could not be opened for reading", file_name);
    abort ();
  }

  int status = fread (header, 1, 8, fp);
  if (status != 8 || png_sig_cmp (header, 0, 8))
  {
    printf ("[ut_png_read_png_file] File %s is not recognized as a PNG file",
	    file_name);
    abort ();
  }

  /* initialize stuff */
  (*ppng_ptr) = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!(*ppng_ptr))
  {
    printf ("[ut_png_read_png_file] png_create_read_struct failed");
    abort ();
  }

  (*pinfo_ptr) = png_create_info_struct ((*ppng_ptr));
  if (!(*pinfo_ptr))
  {
    printf ("[ut_png_read_png_file] png_create_info_struct failed");
    abort ();
  }

  if (setjmp (png_jmpbuf ((*ppng_ptr))))
  {
    printf ("[ut_png_read_png_file] Error during init_io");
    abort ();
  }

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
  {
    printf ("[ut_png_read_png_file] Error during read_image");
    abort ();
  }

  (*prow_pointers) = (png_bytep *) malloc (sizeof (png_bytep) * height);
  for (i = 0; i < height; i++)
    (*prow_pointers)[i] = (png_byte *) malloc (4 * width);

  png_read_image ((*ppng_ptr), (*prow_pointers));

  fclose (fp);
}

void
ut_png_write_png_file (png_structp png_ptr, png_infop info_ptr,
		png_bytep * row_pointers, char *file_name)
{
  /* create file */
  FILE *fp = fopen (file_name, "wb");

  png_init_io (png_ptr, fp);

  /* write header */
  if (setjmp (png_jmpbuf (png_ptr)))
    abort ();

  png_write_info (png_ptr, info_ptr);

  /* write bytes */
  if (setjmp (png_jmpbuf (png_ptr)))
    abort ();

  png_write_image (png_ptr, row_pointers);

  /* end write */
  if (setjmp (png_jmpbuf (png_ptr)))
    abort ();

  png_write_end (png_ptr, NULL);

  /* cleanup heap allocation */

  fclose (fp);
}

void
ut_png_free_png_file (png_structp png_ptr, png_infop info_ptr, png_bytep * row_pointers)
{
  unsigned int y;
  unsigned int height = png_get_image_height (png_ptr, info_ptr);

  for (y = 0; y < height; y++)
    free (row_pointers[y]);
  free (row_pointers);

  return;
}

/*
int
ut_png_average_png_images (png_infop info_ptr1,
		    png_bytep* row_pointers1, double weight1,
		    png_infop info_ptr2,
		    png_bytep* row_pointers2, double weight2,
		    png_structp* ppng_ptr3, png_infop* pinfo_ptr3,
		    png_bytep** prow_pointers3)
{
  int i, j, k;
  int height = info_ptr1->height;
  int width  = info_ptr1->width;
  png_byte *row1 = NULL;
  png_byte *ptr1 = NULL;
  png_byte *row2 = NULL;
  png_byte *ptr2 = NULL;
  int rgba[4];
  unsigned int y;

  // normalizing weights
  weight1 /= (weight1 + weight2);
  weight2 /= (weight1 + weight2);

  if (info_ptr1->height != info_ptr2->height
   || info_ptr1->width  != info_ptr2->width )
  {
    printf ("images must be of the same size!\n");
    abort ();
  }

  ut_png_init_png_file_rgb (width, height, ppng_ptr3, pinfo_ptr3, prow_pointers3);
  (*ppng_ptr3) =
    png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  (*pinfo_ptr3) = png_create_info_struct (*ppng_ptr3);

  // image parameters
  (*pinfo_ptr3)->width = info_ptr1->width;
  (*pinfo_ptr3)->height = info_ptr1->height;
  (*pinfo_ptr3)->color_type = info_ptr1->color_type;
  (*pinfo_ptr3)->bit_depth = info_ptr1->bit_depth;
  (*pinfo_ptr3)->rowbytes = info_ptr1->width * 4;

  // allocation
  (*prow_pointers3) =
    (png_bytep *) malloc (sizeof (png_bytep) * (*pinfo_ptr3)->height);
  for (y = 0; y < (*pinfo_ptr3)->height; y++)
    (*prow_pointers3)[y] = (png_byte *) malloc ((*pinfo_ptr3)->rowbytes);

  for (j = 0; j < height; j++)
  {
    row1 = row_pointers1[j];
    row2 = row_pointers2[j];
    for (i = 0; i < width; i++)
    {
      ptr1 = &(row1[i * 4]);
      ptr2 = &(row2[i * 4]);

      printf ("pixel %d %d --> %d %d %d %d\n", i, j, ptr1[0], ptr1[1], ptr1[2], ptr1[3]);

      for (k = 0; k < 4; k++)
	rgba[k] = weight1 * ptr1[k] + weight2 * ptr2[k];

      ut_png_set_png_pixel (i, j, rgba, prow_pointers3);
    }
  }

  return 0;
}
*/
#endif // HAVE_PNG
