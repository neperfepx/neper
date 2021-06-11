/* Copyright (C) 2003-2021, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

/// \file ut_color.h
/// \brief Color functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Get the RGB levels of a color
/// \param name: color name
/// \param rgb: RGB levels
/// \remark \c name is a name or a list of RGB levels combined with one of ,/|.:;
/// \return pointer to the array
  extern int ut_color_name_rgb (char *name, int *rgb);

/// \brief Check if a color name is valid
/// \param size: size of the array
/// \return 0 if unvalid or 1 if valid
  extern int ut_color_name_isvalid (char *name);

/// \brief Get a color palette
/// \param size: size of the palette
/// \param palette: palette ([0..size-1][0..2])
/// \param min: minimum brightness
/// \param max: maximum brightness
/// \see \c ut_color_palette_0208
  extern void ut_color_palette (int size, double min, double max,
                                int **palette);

/// \brief Get a color palette of brightness between 0.2 and 0.8
/// \param *ppalette: palette ([0..size-1][0..2])
/// \param *psize: size of the palette
  extern void ut_color_palette_0208 (int ***ppalette, int *psize);

/// \brief Get a color out of a color bar
/// \param bar: color bar; if \c NULL, defaults to \c "blue,cyan,yellow,red"
/// \param beg: initial value
/// \param end: final value
/// \param val: value
/// \param col: color
/// \return 0 on success, -1 on failure
  extern int ut_color_bar_val_color (char *bar, double beg, double end,
                                     double val, int *col);

/// \brief Change the intensity of a color
/// \param col: color
/// \param intensity: intensity
/// \param col: output color
/// \note intensity = 0 -> black, 1 -> white, 0.5 -> original color
  extern void ut_color_changeintensity (int *col, double intensity,
                                        int *col2);

#ifdef __cplusplus
}
#endif
