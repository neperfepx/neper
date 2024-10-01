/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include"ut.h"

void
ut_xfig_plotline (FILE * out, int *p1, int *p2, char *style, int type,
                  int thickness, int col)
{
  int x1, x2, y1b, y2;

  x1 = 10 * p1[0];
  y1b = 10 * p1[1];

  x2 = 10 * p2[0];
  y2 = 10 * p2[1];

  if (!strcmp (style, "line"))
  {
    fprintf (out, "2 1 %d %d %d 7 50 -1 -1 4.000 0 0 -1 0 0 2\n", type,
             thickness, col);
    fprintf (out, "\t\t%d %d %d %d\n", x1, y1b, x2, y2);
  }
  else if (!strcmp (style, "arrow"))
  {
    fprintf (out, "2 1 %d %d %d 7 50 -1 -1 0.000 0 0 -1 1 0 2\n", type,
             thickness, col);
    fprintf (out, "\t\t1 1 1.0 90.00 120.00\n");
    fprintf (out, "\t\t%d %d %d %d\n", x1, y1b, x2, y2);
  }

  return;
}
