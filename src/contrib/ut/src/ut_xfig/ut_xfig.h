/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_XFIG_H
#define UT_XFIG_H
/// \file ut_xfig.h
/// \brief XFig-related functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Plot a line
/// \param file: file pointer
/// \param p1: start point
/// \param p2: end point
/// \param style: style, among \c line an \c arrow
/// \param type: type
/// \param thickness: thickness
/// \param col: colour
  extern void ut_xfig_plotline (FILE *file, int *p1, int *p2, char *style, int type, int thickness, int col);

#endif                          /* UT_XFIG_H */

#ifdef __cplusplus
}
#endif
