/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// Source: https://www.geometrictools.com/Documentation/DistancePointEllipseEllipsoid.pdf

//Pseudocode for robustly computing the closest ellipse point and distance to a query point. It
//is required that e0 >= e1 > 0, y0 >= 0, and y1 >= 0.
//e0,e1 = ellipse dimension 0 and 1, where 0 is greater and both are positive.
//y0,y1 = initial point on ellipse axis (center of ellipse is 0,0)
//x0,x1 = intersection point

double
GetRoot (double r0, double z0, double z1, double g)
{
  double n0 = r0 * z0;
  double s0 = z1 - 1;
  double s1 = (g < 0 ? 0 : sqrt (n0 * n0 + z1 * z1) - 1);
  double s = 0;
  int i;
  for (i = 0; i < 1000; ++i)
  {
    s = (s0 + s1) / 2;

    if (s == s0 || s == s1)
      break;

    double ratio0 = n0 / (s + r0);
    double ratio1 = z1 / (s + 1);
    g = ratio0 * ratio0 + ratio1 * ratio1 - 1;

    if (g > 0)
      s0 = s;
    else if (g < 0)
      s1 = s;
    else
      break;
  }
  return s;
}

double
DistancePointEllipse (double e0, double e1, double y0, double y1,
                      double *px0, double *px1)
{
  double distance;

  if (y1 > 0)
  {
    if (y0 > 0)
    {
      double z0 = y0 / e0;
      double z1 = y1 / e1;
      double g = z0 * z0 + z1 * z1 - 1;

      if (g != 0)
      {
        double r0 = (e0 / e1) * (e0 / e1);
        double sbar = GetRoot (r0, z0, z1, g);

        (*px0) = r0 * y0 / (sbar + r0);
        (*px1) = y1 / (sbar + 1);
        distance =
          sqrt (((*px0) - y0) * ((*px0) - y0) +
                ((*px1) - y1) * ((*px1) - y1));
      }
      else
      {
        (*px0) = y0;
        (*px1) = y1;
        distance = 0;
      }
    }
    else                        // y0 == 0
    {
      (*px0) = 0;
      (*px1) = e1;
      distance = fabs (y1 - e1);
    }
  }
  else
  {                             // y1 == 0
    double numer0 = e0 * y0, denom0 = e0 * e0 - e1 * e1;

    if (numer0 < denom0)
    {
      double xde0 = numer0 / denom0;

      (*px0) = e0 * xde0;
      (*px1) = e1 * sqrt (1 - xde0 * xde0);
      distance = sqrt (((*px0) - y0) * ((*px0) - y0) + (*px1) * (*px1));
    }
    else
    {
      (*px0) = e0;
      (*px1) = 0;
      distance = fabs (y0 - e0);
    }
  }

  return distance;
}
