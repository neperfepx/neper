/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_SPACE_ELLIPSE_H
#define UT_SPACE_ELLIPSE_H

#endif                          /* UT_SPACE_ELLIPSE_H */

  extern double GetRoot (double r0, double z0, double z1, double g);

  extern double DistancePointEllipse (double e0, double e1, double y0,
                                      double y1, double *px0, double *px1);

#ifdef __cplusplus
}
#endif
