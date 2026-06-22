/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_scale_.h"

void
nev_print_png_scale_minmax (FILE *file, struct DATA Data, struct PRINT Print)
{
  int id, print_min, print_max, tickqty;
  double min, max, datamin, datamax, pos[2], *tickvals = NULL;
  char *tmp = ut_alloc_1d_char (1000);
  char *tickformat = NULL, **ticks = NULL;

  nev_print_scale_ticks (Data.Scale, &min, &max, &ticks, &tickvals, &tickqty, &tickformat);

  datamin = ut_array_2d_col_min (Data.ColData + 1, 0, Data.Qty);
  datamax = ut_array_2d_col_max (Data.ColData + 1, 0, Data.Qty);

  print_min = print_max = 0;
  if ((datamin < min - 1e-3 * (max - min) && Print.showscalemin == -1) || Print.showscalemin == 1)
    print_min = 1;
  if ((datamax > max + 1e-3 * (max - min) && Print.showscalemax == -1) || Print.showscalemax == 1)
    print_max = 1;

  if (print_min && print_max)
  {
    pos[0] = -0.17;
    pos[1] = -0.25;
  }
  else
    pos[0] = -0.21;

  int rgb = !strcmp (Print.colormode, "bright") ? 0 : 1;

  id = 0;
  if (print_min)
  {
    sprintf (tmp, tickformat, datamin);

    fprintf (file, "#declare MinString =\n");
    fprintf (file, "text {ttf \"timrom.ttf\" \"min\" 0.1, 0\n");
    fprintf (file, "pigment { rgbt <%d,%d,%d,0.> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}\n", rgb, rgb, rgb);
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    fprintf (file, "translate <-0.1,0,%f>\n", pos[id]);
    fprintf (file, "}\n");
    fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0.1, 0\n", tmp);
    fprintf (file, "pigment { rgbt <%d,%d,%d,0.> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}\n", rgb, rgb, rgb);
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    fprintf (file, "translate <0.02,0,%f>\n", pos[id]);
    fprintf (file, "}\n");
    fprintf (file, "object {MinString}\n");

    id++;
  }

  if (print_max)
  {
    sprintf (tmp, tickformat, datamax);

    fprintf (file, "#declare MaxString =\n");
    fprintf (file, "text {ttf \"timrom.ttf\" \"max\" 0.1, 0\n");
    fprintf (file, "pigment { rgbt <%d,%d,%d,0.> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}\n", rgb, rgb, rgb);
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    fprintf (file, "translate <-0.1,0,%f>\n", pos[id]);
    fprintf (file, "}\n");
    fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0.1, 0\n", tmp);
    fprintf (file, "pigment { rgbt <%d,%d,%d,0.> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}\n", rgb, rgb, rgb);
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    fprintf (file, "translate <0.02,0,%f>\n", pos[id]);
    fprintf (file, "}\n");
    fprintf (file, "object {MaxString}\n");

    id++;
  }

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tickformat);
  ut_free_2d_char (&ticks, tickqty);
  ut_free_1d (&tickvals);

  return;
}
