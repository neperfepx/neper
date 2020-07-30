/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_PNG_H
#define UT_PNG_H

#ifdef HAVE_PNG
#include <png.h>

/// \file ut_png.h
/// \brief PNG-image functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Allocate a PNG image
/// \param sizex: x size
/// \param sizey: y size
/// \param *ppng_ptr: PNG pointer
/// \param *pinfo_ptr: info pointer
/// \param *prow_ptr: row pointer
  extern void ut_png_alloc (int sizex, int sizey, png_structp * ppng_ptr,
                            png_infop * pinfo_ptr, png_bytep ** prow_ptr);

/// \brief Free a PNG image
/// \param png_ptr: PNG pointer
/// \param info_ptr: info pointer
/// \param row_ptr: row pointer
  extern void ut_png_free (png_structp png_ptr, png_infop info_ptr,
                           png_bytep * row_ptr);

/// \brief Set a pixel
/// \param *prow_ptr: image row ptr
/// \param x: x coordinate
/// \param y: y coordinate
/// \param rgb: RGB color (0-255)
/// \see ut_png_init_png_file_rgb
  extern void ut_png_pixel_set (png_bytep ** prow_ptr, int x, int y,
                                int *rgb);

/// \brief Read a PNG image
/// \param filename: file name
/// \param *ppng_ptr: PNG pointer
/// \param *pinfo_ptr: info pointer
/// \param *prow_ptr: row pointer
  extern void ut_png_name_fscanf (char *filename, png_structp * ppng_ptr,
                                  png_infop * pinfo_ptr,
                                  png_bytep ** prow_ptr);

/// \brief Write a PNG image
/// \param filename: file name
/// \param png_ptr: PNG pointer
/// \param info_ptr: info pointer
/// \param row_ptr: row pointer
  extern void ut_png_name_fprintf (char *filename, png_structp png_ptr,
                                   png_infop info_ptr, png_bytep * row_ptr);

#endif                          // HAVE_PNG

#endif                          /* UT_PNG_H */

#ifdef __cplusplus
}
#endif                          // __cplusplus
