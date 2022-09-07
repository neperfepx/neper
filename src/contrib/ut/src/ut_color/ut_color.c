/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "ut.h"

int
ut_color_name_rgb (char *name, int *c)
{
  int i;
  int res = 0;
  char **parts = NULL;
  int qty;

  if (name[0] >= '0' && name[0] <= '9')
  {
    if (strchr (name, ',') != NULL)
      ut_list_break (name, ",", &parts, &qty);
    else if (strchr (name, '/') != NULL)
      ut_list_break (name, "/", &parts, &qty);
    else if (strchr (name, '|') != NULL)
      ut_list_break (name, "|", &parts, &qty);
    else if (strchr (name, '.') != NULL)
      ut_list_break (name, ".", &parts, &qty);
    else if (strchr (name, ':') != NULL)
      ut_list_break (name, ":", &parts, &qty);
    else if (strchr (name, ';') != NULL)
      ut_list_break (name, ";", &parts, &qty);
    else
      qty = -1;

    if (qty != 3)
      res = -1;
    else
      for (i = 0; i < 3; i++)
        sscanf (parts[i], "%d", c + i);

    ut_free_2d_char (&parts, qty);
  }
  else if (!strcmp (name, "aliceblue"))
    ut_array_1d_int_set_3 (c, 240, 248, 255);
  else if (!strcmp (name, "antiquewhite"))
    ut_array_1d_int_set_3 (c, 250, 235, 215);
  else if (!strcmp (name, "aqua"))
    ut_array_1d_int_set_3 (c, 0, 255, 255);
  else if (!strcmp (name, "aquamarine"))
    ut_array_1d_int_set_3 (c, 127, 255, 212);
  else if (!strcmp (name, "azure"))
    ut_array_1d_int_set_3 (c, 240, 255, 255);
  else if (!strcmp (name, "beige"))
    ut_array_1d_int_set_3 (c, 245, 245, 220);
  else if (!strcmp (name, "bisque"))
    ut_array_1d_int_set_3 (c, 255, 228, 196);
  else if (!strcmp (name, "black"))
    ut_array_1d_int_set_3 (c, 0, 0, 0);
  else if (!strcmp (name, "blanchedalmond"))
    ut_array_1d_int_set_3 (c, 255, 235, 205);
  else if (!strcmp (name, "blue"))
    ut_array_1d_int_set_3 (c, 0, 0, 255);
  else if (!strcmp (name, "blueviolet"))
    ut_array_1d_int_set_3 (c, 138, 43, 226);
  else if (!strcmp (name, "brown"))
    ut_array_1d_int_set_3 (c, 165, 42, 42);
  else if (!strcmp (name, "burlywood"))
    ut_array_1d_int_set_3 (c, 222, 184, 135);
  else if (!strcmp (name, "cadetblue"))
    ut_array_1d_int_set_3 (c, 95, 158, 160);
  else if (!strcmp (name, "chartreuse"))
    ut_array_1d_int_set_3 (c, 127, 255, 0);
  else if (!strcmp (name, "chocolate"))
    ut_array_1d_int_set_3 (c, 210, 105, 30);
  else if (!strcmp (name, "coral"))
    ut_array_1d_int_set_3 (c, 255, 127, 80);
  else if (!strcmp (name, "cornflowerblue"))
    ut_array_1d_int_set_3 (c, 100, 149, 237);
  else if (!strcmp (name, "cornsilk"))
    ut_array_1d_int_set_3 (c, 255, 248, 220);
  else if (!strcmp (name, "crimson"))
    ut_array_1d_int_set_3 (c, 220, 20, 60);
  else if (!strcmp (name, "cyan"))
    ut_array_1d_int_set_3 (c, 0, 255, 255);
  else if (!strcmp (name, "darkblue"))
    ut_array_1d_int_set_3 (c, 0, 0, 139);
  else if (!strcmp (name, "darkcyan"))
    ut_array_1d_int_set_3 (c, 0, 139, 139);
  else if (!strcmp (name, "darkgoldenrod"))
    ut_array_1d_int_set_3 (c, 184, 134, 11);
  else if (!strcmp (name, "darkgray"))
    ut_array_1d_int_set_3 (c, 64, 64, 64);
  else if (!strcmp (name, "darkgreen"))
    ut_array_1d_int_set_3 (c, 0, 100, 0);
  else if (!strcmp (name, "darkgrey"))
    ut_array_1d_int_set_3 (c, 64, 64, 64);
  else if (!strcmp (name, "darkkhaki"))
    ut_array_1d_int_set_3 (c, 189, 183, 107);
  else if (!strcmp (name, "darkmagenta"))
    ut_array_1d_int_set_3 (c, 139, 0, 139);
  else if (!strcmp (name, "darkolivegreen"))
    ut_array_1d_int_set_3 (c, 85, 107, 47);
  else if (!strcmp (name, "darkorange"))
    ut_array_1d_int_set_3 (c, 255, 140, 0);
  else if (!strcmp (name, "darkorchid"))
    ut_array_1d_int_set_3 (c, 153, 50, 204);
  else if (!strcmp (name, "darkred"))
    ut_array_1d_int_set_3 (c, 139, 0, 0);
  else if (!strcmp (name, "darksalmon"))
    ut_array_1d_int_set_3 (c, 233, 150, 122);
  else if (!strcmp (name, "darkseagreen"))
    ut_array_1d_int_set_3 (c, 143, 188, 143);
  else if (!strcmp (name, "darkslateblue"))
    ut_array_1d_int_set_3 (c, 72, 61, 139);
  else if (!strcmp (name, "darkslategray"))
    ut_array_1d_int_set_3 (c, 47, 79, 79);
  else if (!strcmp (name, "darkslategrey"))
    ut_array_1d_int_set_3 (c, 47, 79, 79);
  else if (!strcmp (name, "darkturquoise"))
    ut_array_1d_int_set_3 (c, 0, 206, 209);
  else if (!strcmp (name, "darkviolet"))
    ut_array_1d_int_set_3 (c, 148, 0, 211);
  else if (!strcmp (name, "deeppink"))
    ut_array_1d_int_set_3 (c, 255, 20, 147);
  else if (!strcmp (name, "deepskyblue"))
    ut_array_1d_int_set_3 (c, 0, 191, 255);
  else if (!strcmp (name, "dimgray"))
    ut_array_1d_int_set_3 (c, 105, 105, 105);
  else if (!strcmp (name, "dimgrey"))
    ut_array_1d_int_set_3 (c, 105, 105, 105);
  else if (!strcmp (name, "dodgerblue"))
    ut_array_1d_int_set_3 (c, 30, 144, 255);
  else if (!strcmp (name, "firebrick"))
    ut_array_1d_int_set_3 (c, 178, 34, 34);
  else if (!strcmp (name, "floralwhite"))
    ut_array_1d_int_set_3 (c, 255, 250, 240);
  else if (!strcmp (name, "forestgreen"))
    ut_array_1d_int_set_3 (c, 34, 139, 34);
  else if (!strcmp (name, "fuchsia"))
    ut_array_1d_int_set_3 (c, 255, 0, 255);
  else if (!strcmp (name, "gainsboro"))
    ut_array_1d_int_set_3 (c, 220, 220, 220);
  else if (!strcmp (name, "ghostwhite"))
    ut_array_1d_int_set_3 (c, 248, 248, 255);
  else if (!strcmp (name, "gold"))
    ut_array_1d_int_set_3 (c, 255, 215, 0);
  else if (!strcmp (name, "goldenrod"))
    ut_array_1d_int_set_3 (c, 218, 165, 32);
  else if (!strcmp (name, "gray"))
    ut_array_1d_int_set_3 (c, 128, 128, 128);
  else if (!strcmp (name, "grey"))
    ut_array_1d_int_set_3 (c, 128, 128, 128);
  else if (!strcmp (name, "green"))
    ut_array_1d_int_set_3 (c, 0, 255, 0);
  else if (!strcmp (name, "greenyellow"))
    ut_array_1d_int_set_3 (c, 173, 255, 47);
  else if (!strcmp (name, "honeydew"))
    ut_array_1d_int_set_3 (c, 240, 255, 240);
  else if (!strcmp (name, "hotpink"))
    ut_array_1d_int_set_3 (c, 255, 105, 180);
  else if (!strcmp (name, "indianred"))
    ut_array_1d_int_set_3 (c, 205, 92, 92);
  else if (!strcmp (name, "indigo"))
    ut_array_1d_int_set_3 (c, 75, 0, 130);
  else if (!strcmp (name, "ivory"))
    ut_array_1d_int_set_3 (c, 255, 255, 240);
  else if (!strcmp (name, "khaki"))
    ut_array_1d_int_set_3 (c, 240, 230, 140);
  else if (!strcmp (name, "lavender"))
    ut_array_1d_int_set_3 (c, 230, 230, 250);
  else if (!strcmp (name, "lavenderblush"))
    ut_array_1d_int_set_3 (c, 255, 240, 245);
  else if (!strcmp (name, "lawngreen"))
    ut_array_1d_int_set_3 (c, 124, 252, 0);
  else if (!strcmp (name, "lemonchiffon"))
    ut_array_1d_int_set_3 (c, 255, 250, 205);
  else if (!strcmp (name, "lightblue"))
    ut_array_1d_int_set_3 (c, 173, 216, 230);
  else if (!strcmp (name, "lightcoral"))
    ut_array_1d_int_set_3 (c, 240, 128, 128);
  else if (!strcmp (name, "lightcyan"))
    ut_array_1d_int_set_3 (c, 224, 255, 255);
  else if (!strcmp (name, "lightgoldenrodyellow"))
    ut_array_1d_int_set_3 (c, 250, 250, 210);
  else if (!strcmp (name, "lightgray"))
    ut_array_1d_int_set_3 (c, 211, 211, 211);
  else if (!strcmp (name, "lightgreen"))
    ut_array_1d_int_set_3 (c, 144, 238, 144);
  else if (!strcmp (name, "lightgrey"))
    ut_array_1d_int_set_3 (c, 211, 211, 211);
  else if (!strcmp (name, "lightpink"))
    ut_array_1d_int_set_3 (c, 255, 182, 193);
  else if (!strcmp (name, "lightsalmon"))
    ut_array_1d_int_set_3 (c, 255, 160, 122);
  else if (!strcmp (name, "lightseagreen"))
    ut_array_1d_int_set_3 (c, 32, 178, 170);
  else if (!strcmp (name, "lightskyblue"))
    ut_array_1d_int_set_3 (c, 135, 206, 250);
  else if (!strcmp (name, "lightslategray"))
    ut_array_1d_int_set_3 (c, 119, 136, 153);
  else if (!strcmp (name, "lightslategrey"))
    ut_array_1d_int_set_3 (c, 119, 136, 153);
  else if (!strcmp (name, "lightsteelblue"))
    ut_array_1d_int_set_3 (c, 176, 196, 222);
  else if (!strcmp (name, "lightyellow"))
    ut_array_1d_int_set_3 (c, 255, 255, 224);
  else if (!strcmp (name, "lime"))
    ut_array_1d_int_set_3 (c, 0, 255, 0);
  else if (!strcmp (name, "limegreen"))
    ut_array_1d_int_set_3 (c, 50, 205, 50);
  else if (!strcmp (name, "linen"))
    ut_array_1d_int_set_3 (c, 250, 240, 230);
  else if (!strcmp (name, "magenta"))
    ut_array_1d_int_set_3 (c, 255, 0, 255);
  else if (!strcmp (name, "maroon"))
    ut_array_1d_int_set_3 (c, 128, 0, 0);
  else if (!strcmp (name, "mediumaquamarine"))
    ut_array_1d_int_set_3 (c, 102, 205, 170);
  else if (!strcmp (name, "mediumblue"))
    ut_array_1d_int_set_3 (c, 0, 0, 205);
  else if (!strcmp (name, "mediumorchid"))
    ut_array_1d_int_set_3 (c, 186, 85, 211);
  else if (!strcmp (name, "mediumpurple"))
    ut_array_1d_int_set_3 (c, 147, 112, 219);
  else if (!strcmp (name, "mediumseagreen"))
    ut_array_1d_int_set_3 (c, 60, 179, 113);
  else if (!strcmp (name, "mediumslateblue"))
    ut_array_1d_int_set_3 (c, 123, 104, 238);
  else if (!strcmp (name, "mediumspringgreen"))
    ut_array_1d_int_set_3 (c, 0, 250, 154);
  else if (!strcmp (name, "mediumturquoise"))
    ut_array_1d_int_set_3 (c, 72, 209, 204);
  else if (!strcmp (name, "mediumvioletred"))
    ut_array_1d_int_set_3 (c, 199, 21, 133);
  else if (!strcmp (name, "midnightblue"))
    ut_array_1d_int_set_3 (c, 25, 25, 112);
  else if (!strcmp (name, "mintcream"))
    ut_array_1d_int_set_3 (c, 245, 255, 250);
  else if (!strcmp (name, "mistyrose"))
    ut_array_1d_int_set_3 (c, 255, 228, 225);
  else if (!strcmp (name, "moccasin"))
    ut_array_1d_int_set_3 (c, 255, 228, 181);
  else if (!strcmp (name, "navajowhite"))
    ut_array_1d_int_set_3 (c, 255, 222, 173);
  else if (!strcmp (name, "navy"))
    ut_array_1d_int_set_3 (c, 0, 0, 128);
  else if (!strcmp (name, "oldlace"))
    ut_array_1d_int_set_3 (c, 253, 245, 230);
  else if (!strcmp (name, "olive"))
    ut_array_1d_int_set_3 (c, 128, 128, 0);
  else if (!strcmp (name, "olivedrab"))
    ut_array_1d_int_set_3 (c, 107, 142, 35);
  else if (!strcmp (name, "orange"))
    ut_array_1d_int_set_3 (c, 255, 165, 0);
  else if (!strcmp (name, "orangered"))
    ut_array_1d_int_set_3 (c, 255, 69, 0);
  else if (!strcmp (name, "orchid"))
    ut_array_1d_int_set_3 (c, 218, 112, 214);
  else if (!strcmp (name, "palegoldenrod"))
    ut_array_1d_int_set_3 (c, 238, 232, 170);
  else if (!strcmp (name, "palegreen"))
    ut_array_1d_int_set_3 (c, 152, 251, 152);
  else if (!strcmp (name, "paleturquoise"))
    ut_array_1d_int_set_3 (c, 175, 238, 238);
  else if (!strcmp (name, "palevioletred"))
    ut_array_1d_int_set_3 (c, 219, 112, 147);
  else if (!strcmp (name, "papayawhip"))
    ut_array_1d_int_set_3 (c, 255, 239, 213);
  else if (!strcmp (name, "peachpuff"))
    ut_array_1d_int_set_3 (c, 255, 218, 185);
  else if (!strcmp (name, "peru"))
    ut_array_1d_int_set_3 (c, 205, 133, 63);
  else if (!strcmp (name, "pink"))
    ut_array_1d_int_set_3 (c, 255, 192, 203);
  else if (!strcmp (name, "plum"))
    ut_array_1d_int_set_3 (c, 221, 160, 221);
  else if (!strcmp (name, "powderblue"))
    ut_array_1d_int_set_3 (c, 176, 224, 230);
  else if (!strcmp (name, "purple"))
    ut_array_1d_int_set_3 (c, 128, 0, 128);
  else if (!strcmp (name, "red"))
    ut_array_1d_int_set_3 (c, 255, 0, 0);
  else if (!strcmp (name, "rosybrown"))
    ut_array_1d_int_set_3 (c, 188, 143, 143);
  else if (!strcmp (name, "royalblue"))
    ut_array_1d_int_set_3 (c, 65, 105, 225);
  else if (!strcmp (name, "saddlebrown"))
    ut_array_1d_int_set_3 (c, 139, 69, 19);
  else if (!strcmp (name, "salmon"))
    ut_array_1d_int_set_3 (c, 250, 128, 114);
  else if (!strcmp (name, "sandybrown"))
    ut_array_1d_int_set_3 (c, 244, 164, 96);
  else if (!strcmp (name, "seagreen"))
    ut_array_1d_int_set_3 (c, 46, 139, 87);
  else if (!strcmp (name, "seashell"))
    ut_array_1d_int_set_3 (c, 255, 245, 238);
  else if (!strcmp (name, "sienna"))
    ut_array_1d_int_set_3 (c, 160, 82, 45);
  else if (!strcmp (name, "silver"))
    ut_array_1d_int_set_3 (c, 192, 192, 192);
  else if (!strcmp (name, "skyblue"))
    ut_array_1d_int_set_3 (c, 135, 206, 235);
  else if (!strcmp (name, "slateblue"))
    ut_array_1d_int_set_3 (c, 106, 90, 205);
  else if (!strcmp (name, "slategray"))
    ut_array_1d_int_set_3 (c, 112, 128, 144);
  else if (!strcmp (name, "slategrey"))
    ut_array_1d_int_set_3 (c, 112, 128, 144);
  else if (!strcmp (name, "snow"))
    ut_array_1d_int_set_3 (c, 255, 250, 250);
  else if (!strcmp (name, "springgreen"))
    ut_array_1d_int_set_3 (c, 0, 255, 127);
  else if (!strcmp (name, "steelblue"))
    ut_array_1d_int_set_3 (c, 70, 130, 180);
  else if (!strcmp (name, "tan"))
    ut_array_1d_int_set_3 (c, 210, 180, 140);
  else if (!strcmp (name, "teal"))
    ut_array_1d_int_set_3 (c, 0, 128, 128);
  else if (!strcmp (name, "thistle"))
    ut_array_1d_int_set_3 (c, 216, 191, 216);
  else if (!strcmp (name, "tomato"))
    ut_array_1d_int_set_3 (c, 255, 99, 71);
  else if (!strcmp (name, "turquoise"))
    ut_array_1d_int_set_3 (c, 64, 224, 208);
  else if (!strcmp (name, "violet"))
    ut_array_1d_int_set_3 (c, 238, 130, 238);
  else if (!strcmp (name, "wheat"))
    ut_array_1d_int_set_3 (c, 245, 222, 179);
  else if (!strcmp (name, "white"))
    ut_array_1d_int_set_3 (c, 255, 255, 255);
  else if (!strcmp (name, "whitesmoke"))
    ut_array_1d_int_set_3 (c, 245, 245, 245);
  else if (!strcmp (name, "yellow"))
    ut_array_1d_int_set_3 (c, 255, 255, 0);
  else if (!strcmp (name, "yellowgreen"))
    ut_array_1d_int_set_3 (c, 154, 205, 50);
  else
    res = -1;
  return res;
}

int
ut_color_name_isvalid (char *name)
{
  int status, *c = ut_alloc_1d_int (3);

  status = ut_color_name_rgb (name, c);

  ut_free_1d_int (&c);

  return (status != -1);
}

void
ut_color_changeintensity (int *rgb, double intensity, int *rgb2)
{
  int i;

  if (intensity < 0)
    intensity = 0;
  if (intensity > 1)
    intensity = 1;
  if (intensity >= 0 && intensity < 0.5)
  {
    for (i = 0; i < 3; i++)
      rgb2[i] = (int) (2 * intensity * rgb[i]);
  }
  else if (intensity >= 0.5 && intensity <= 1)
  {
    for (i = 0; i < 3; i++)
      rgb2[i] =
        (int) (2 * (255 - (double) rgb[i]) * intensity - 255 +
               2 * (double) rgb[i]);
  }

  return;
}


int
ut_color_bar_val_color (char *scheme, double beg, double end, double val,
                        int *col)
{
  int i, pos;
  double pos2;
  char **cols = NULL;
  int colqty;
  double *vals = NULL;
  int *col0 = ut_alloc_1d_int (3);
  int *col1 = ut_alloc_1d_int (3);
  int status = 0;
  char *fct = NULL;

  if (ut_string_strcmp (scheme, "legacy"))
  {
    ut_string_function (scheme, &fct, NULL, &cols, &colqty);
    if (strcmp (fct, "custom"))
      ut_print_exprbug (scheme);
  }
  else
    ut_string_function ("custom(blue,cyan,yellow,red)", &fct, NULL, &cols, &colqty);

  vals = ut_alloc_1d (colqty);
  for (i = 0; i < colqty; i++)
    vals[i] = beg + (end - beg) * i / (colqty - 1);
  pos = ut_array_1d_eltpos_interpolate (vals, colqty, val);
  if (pos == -1)
  {
    status = -1;
    if (beg < end)
    {
      if (val < vals[0])
        ut_color_name_rgb (cols[0], col);
      else
        ut_color_name_rgb (cols[colqty - 1], col);
    }
    else
    {
      if (val < vals[0])
        ut_color_name_rgb (cols[colqty - 1], col);
      else
        ut_color_name_rgb (cols[0], col);
    }
  }
  else
  {
    if (pos < colqty - 1)
    {
      ut_color_name_rgb (cols[pos], col0);
      ut_color_name_rgb (cols[pos + 1], col1);
      if (vals[pos + 1] - vals[pos] != 0)
        pos2 = (val - vals[pos]) / (vals[pos + 1] - vals[pos]);
      else
        pos2 = 0.5;
      for (i = 0; i < 3; i++)
        col[i] = ut_num_d2ri (col0[i] + pos2 * (col1[i] - col0[i]));
    }
    else
      ut_color_name_rgb (cols[pos], col);
  }

  ut_free_1d_int (&col0);
  ut_free_1d_int (&col1);
  ut_free_2d_char (&cols, colqty);
  ut_free_1d (&vals);
  ut_free_1d_char (&fct);

  return status;
}

void
ut_color_palette (int size, double min, double max, int **palette)
{
  int i;
  int pqty = 137;
  int **p = ut_alloc_2d_int (pqty, 3);
  p[0][0] = 0;
  p[0][1] = 0;
  p[0][2] = 0;
  p[1][0] = 255;
  p[1][1] = 0;
  p[1][2] = 0;
  p[2][0] = 0;
  p[2][1] = 255;
  p[2][2] = 0;
  p[3][0] = 0;
  p[3][1] = 0;
  p[3][2] = 255;
  p[4][0] = 255;
  p[4][1] = 255;
  p[4][2] = 0;
  p[5][0] = 255;
  p[5][1] = 0;
  p[5][2] = 255;
  p[6][0] = 0;
  p[6][1] = 255;
  p[6][2] = 255;
  p[7][0] = 255;
  p[7][1] = 255;
  p[7][2] = 255;
  p[8][0] = 128;
  p[8][1] = 0;
  p[8][2] = 0;
  p[9][0] = 0;
  p[9][1] = 0;
  p[9][2] = 128;
  p[10][0] = 127;
  p[10][1] = 255;
  p[10][2] = 0;
  p[11][0] = 0;
  p[11][1] = 255;
  p[11][2] = 127;
  p[12][0] = 128;
  p[12][1] = 128;
  p[12][2] = 0;
  p[13][0] = 128;
  p[13][1] = 0;
  p[13][2] = 128;
  p[14][0] = 0;
  p[14][1] = 128;
  p[14][2] = 128;
  p[15][0] = 128;
  p[15][1] = 128;
  p[15][2] = 128;
  p[16][0] = 0;
  p[16][1] = 191;
  p[16][2] = 255;
  p[17][0] = 124;
  p[17][1] = 252;
  p[17][2] = 0;
  p[18][0] = 64;
  p[18][1] = 64;
  p[18][2] = 64;
  p[19][0] = 255;
  p[19][1] = 69;
  p[19][2] = 0;
  p[20][0] = 192;
  p[20][1] = 192;
  p[20][2] = 192;
  p[21][0] = 255;
  p[21][1] = 250;
  p[21][2] = 250;
  p[22][0] = 139;
  p[22][1] = 0;
  p[22][2] = 0;
  p[23][0] = 0;
  p[23][1] = 0;
  p[23][2] = 139;
  p[24][0] = 255;
  p[24][1] = 140;
  p[24][2] = 0;
  p[25][0] = 240;
  p[25][1] = 255;
  p[25][2] = 255;
  p[26][0] = 248;
  p[26][1] = 248;
  p[26][2] = 255;
  p[27][0] = 255;
  p[27][1] = 255;
  p[27][2] = 240;
  p[28][0] = 0;
  p[28][1] = 0;
  p[28][2] = 205;
  p[29][0] = 255;
  p[29][1] = 182;
  p[29][2] = 193;
  p[30][0] = 245;
  p[30][1] = 255;
  p[30][2] = 250;
  p[31][0] = 75;
  p[31][1] = 0;
  p[31][2] = 130;
  p[32][0] = 240;
  p[32][1] = 128;
  p[32][2] = 128;
  p[33][0] = 255;
  p[33][1] = 192;
  p[33][2] = 203;
  p[34][0] = 255;
  p[34][1] = 127;
  p[34][2] = 80;
  p[35][0] = 250;
  p[35][1] = 128;
  p[35][2] = 114;
  p[36][0] = 255;
  p[36][1] = 250;
  p[36][2] = 240;
  p[37][0] = 127;
  p[37][1] = 255;
  p[37][2] = 212;
  p[38][0] = 255;
  p[38][1] = 250;
  p[38][2] = 205;
  p[39][0] = 255;
  p[39][1] = 215;
  p[39][2] = 0;
  p[40][0] = 0;
  p[40][1] = 100;
  p[40][2] = 0;
  p[41][0] = 255;
  p[41][1] = 165;
  p[41][2] = 0;
  p[42][0] = 240;
  p[42][1] = 248;
  p[42][2] = 255;
  p[43][0] = 224;
  p[43][1] = 255;
  p[43][2] = 255;
  p[44][0] = 255;
  p[44][1] = 255;
  p[44][2] = 224;
  p[45][0] = 139;
  p[45][1] = 0;
  p[45][2] = 139;
  p[46][0] = 0;
  p[46][1] = 139;
  p[46][2] = 139;
  p[47][0] = 205;
  p[47][1] = 133;
  p[47][2] = 63;
  p[48][0] = 70;
  p[48][1] = 130;
  p[48][2] = 180;
  p[49][0] = 255;
  p[49][1] = 240;
  p[49][2] = 245;
  p[50][0] = 255;
  p[50][1] = 245;
  p[50][2] = 238;
  p[51][0] = 0;
  p[51][1] = 250;
  p[51][2] = 154;
  p[52][0] = 72;
  p[52][1] = 61;
  p[52][2] = 139;
  p[53][0] = 184;
  p[53][1] = 134;
  p[53][2] = 11;
  p[54][0] = 255;
  p[54][1] = 160;
  p[54][2] = 122;
  p[55][0] = 255;
  p[55][1] = 228;
  p[55][2] = 196;
  p[56][0] = 135;
  p[56][1] = 206;
  p[56][2] = 250;
  p[57][0] = 250;
  p[57][1] = 250;
  p[57][2] = 210;
  p[58][0] = 240;
  p[58][1] = 255;
  p[58][2] = 240;
  p[59][0] = 255;
  p[59][1] = 248;
  p[59][2] = 220;
  p[60][0] = 255;
  p[60][1] = 218;
  p[60][2] = 185;
  p[61][0] = 245;
  p[61][1] = 245;
  p[61][2] = 245;
  p[62][0] = 255;
  p[62][1] = 99;
  p[62][2] = 71;
  p[63][0] = 112;
  p[63][1] = 128;
  p[63][2] = 144;
  p[64][0] = 255;
  p[64][1] = 105;
  p[64][2] = 180;
  p[65][0] = 253;
  p[65][1] = 245;
  p[65][2] = 230;
  p[66][0] = 255;
  p[66][1] = 235;
  p[66][2] = 205;
  p[67][0] = 189;
  p[67][1] = 183;
  p[67][2] = 107;
  p[68][0] = 255;
  p[68][1] = 228;
  p[68][2] = 181;
  p[69][0] = 0;
  p[69][1] = 206;
  p[69][2] = 209;
  p[70][0] = 60;
  p[70][1] = 179;
  p[70][2] = 113;
  p[71][0] = 199;
  p[71][1] = 21;
  p[71][2] = 133;
  p[72][0] = 238;
  p[72][1] = 130;
  p[72][2] = 238;
  p[73][0] = 173;
  p[73][1] = 255;
  p[73][2] = 47;
  p[74][0] = 255;
  p[74][1] = 239;
  p[74][2] = 213;
  p[75][0] = 143;
  p[75][1] = 188;
  p[75][2] = 143;
  p[76][0] = 188;
  p[76][1] = 143;
  p[76][2] = 143;
  p[77][0] = 255;
  p[77][1] = 20;
  p[77][2] = 147;
  p[78][0] = 139;
  p[78][1] = 69;
  p[78][2] = 19;
  p[79][0] = 148;
  p[79][1] = 0;
  p[79][2] = 211;
  p[80][0] = 30;
  p[80][1] = 144;
  p[80][2] = 255;
  p[81][0] = 119;
  p[81][1] = 136;
  p[81][2] = 153;
  p[82][0] = 222;
  p[82][1] = 184;
  p[82][2] = 135;
  p[83][0] = 255;
  p[83][1] = 222;
  p[83][2] = 173;
  p[84][0] = 250;
  p[84][1] = 240;
  p[84][2] = 230;
  p[85][0] = 123;
  p[85][1] = 104;
  p[85][2] = 238;
  p[86][0] = 64;
  p[86][1] = 224;
  p[86][2] = 208;
  p[87][0] = 135;
  p[87][1] = 206;
  p[87][2] = 235;
  p[88][0] = 72;
  p[88][1] = 209;
  p[88][2] = 204;
  p[89][0] = 245;
  p[89][1] = 245;
  p[89][2] = 220;
  p[90][0] = 255;
  p[90][1] = 228;
  p[90][2] = 225;
  p[91][0] = 210;
  p[91][1] = 180;
  p[91][2] = 140;
  p[92][0] = 250;
  p[92][1] = 235;
  p[92][2] = 215;
  p[93][0] = 216;
  p[93][1] = 191;
  p[93][2] = 216;
  p[94][0] = 50;
  p[94][1] = 205;
  p[94][2] = 50;
  p[95][0] = 233;
  p[95][1] = 150;
  p[95][2] = 122;
  p[96][0] = 176;
  p[96][1] = 196;
  p[96][2] = 222;
  p[97][0] = 65;
  p[97][1] = 105;
  p[97][2] = 225;
  p[98][0] = 152;
  p[98][1] = 251;
  p[98][2] = 152;
  p[99][0] = 220;
  p[99][1] = 20;
  p[99][2] = 60;
  p[100][0] = 245;
  p[100][1] = 222;
  p[100][2] = 179;
  p[101][0] = 186;
  p[101][1] = 85;
  p[101][2] = 211;
  p[102][0] = 230;
  p[102][1] = 230;
  p[102][2] = 250;
  p[103][0] = 240;
  p[103][1] = 230;
  p[103][2] = 140;
  p[104][0] = 144;
  p[104][1] = 238;
  p[104][2] = 144;
  p[105][0] = 175;
  p[105][1] = 238;
  p[105][2] = 238;
  p[106][0] = 47;
  p[106][1] = 79;
  p[106][2] = 79;
  p[107][0] = 153;
  p[107][1] = 50;
  p[107][2] = 204;
  p[108][0] = 46;
  p[108][1] = 139;
  p[108][2] = 87;
  p[109][0] = 154;
  p[109][1] = 205;
  p[109][2] = 50;
  p[110][0] = 138;
  p[110][1] = 43;
  p[110][2] = 226;
  p[111][0] = 219;
  p[111][1] = 112;
  p[111][2] = 147;
  p[112][0] = 107;
  p[112][1] = 142;
  p[112][2] = 35;
  p[113][0] = 147;
  p[113][1] = 112;
  p[113][2] = 219;
  p[114][0] = 244;
  p[114][1] = 164;
  p[114][2] = 96;
  p[115][0] = 85;
  p[115][1] = 107;
  p[115][2] = 47;
  p[116][0] = 102;
  p[116][1] = 205;
  p[116][2] = 170;
  p[117][0] = 106;
  p[117][1] = 90;
  p[117][2] = 205;
  p[118][0] = 238;
  p[118][1] = 232;
  p[118][2] = 170;
  p[119][0] = 34;
  p[119][1] = 139;
  p[119][2] = 34;
  p[120][0] = 25;
  p[120][1] = 25;
  p[120][2] = 112;
  p[121][0] = 32;
  p[121][1] = 178;
  p[121][2] = 170;
  p[122][0] = 211;
  p[122][1] = 211;
  p[122][2] = 211;
  p[123][0] = 218;
  p[123][1] = 112;
  p[123][2] = 214;
  p[124][0] = 100;
  p[124][1] = 149;
  p[124][2] = 237;
  p[125][0] = 160;
  p[125][1] = 82;
  p[125][2] = 45;
  p[126][0] = 178;
  p[126][1] = 34;
  p[126][2] = 34;
  p[127][0] = 176;
  p[127][1] = 224;
  p[127][2] = 230;
  p[128][0] = 205;
  p[128][1] = 92;
  p[128][2] = 92;
  p[129][0] = 105;
  p[129][1] = 105;
  p[129][2] = 105;
  p[130][0] = 173;
  p[130][1] = 216;
  p[130][2] = 230;
  p[131][0] = 210;
  p[131][1] = 105;
  p[131][2] = 30;
  p[132][0] = 165;
  p[132][1] = 42;
  p[132][2] = 42;
  p[133][0] = 218;
  p[133][1] = 165;
  p[133][2] = 32;
  p[134][0] = 220;
  p[134][1] = 220;
  p[134][2] = 220;
  p[135][0] = 221;
  p[135][1] = 160;
  p[135][2] = 221;
  p[136][0] = 95;
  p[136][1] = 158;
  p[136][2] = 160;
  int id = 0;
  for (i = 0; i < size; i++)
  {
    if (ut_array_1d_int_mean (p[id], 3) / 255 >= min
        && ut_array_1d_int_mean (p[id], 3) / 255 <= max)
      ut_array_1d_int_memcpy (p[id], 3, palette[i]);
    else
      i--;
    id = (id < pqty - 1) ? id + 1 : 0;
  }

  return;
}

void
ut_color_palette_0208 (int ***ppalette, int *psize)
{
  (*psize) = 92;
  (*ppalette) = ut_alloc_2d_int (*psize, 3);
  (*ppalette)[0][0] = 255;
  (*ppalette)[0][1] = 0;
  (*ppalette)[0][2] = 0;
  (*ppalette)[1][0] = 0;
  (*ppalette)[1][1] = 255;
  (*ppalette)[1][2] = 0;
  (*ppalette)[2][0] = 0;
  (*ppalette)[2][1] = 0;
  (*ppalette)[2][2] = 255;
  (*ppalette)[3][0] = 255;
  (*ppalette)[3][1] = 255;
  (*ppalette)[3][2] = 0;
  (*ppalette)[4][0] = 255;
  (*ppalette)[4][1] = 0;
  (*ppalette)[4][2] = 255;
  (*ppalette)[5][0] = 0;
  (*ppalette)[5][1] = 255;
  (*ppalette)[5][2] = 255;
  (*ppalette)[6][0] = 127;
  (*ppalette)[6][1] = 255;
  (*ppalette)[6][2] = 0;
  (*ppalette)[7][0] = 0;
  (*ppalette)[7][1] = 255;
  (*ppalette)[7][2] = 127;
  (*ppalette)[8][0] = 128;
  (*ppalette)[8][1] = 128;
  (*ppalette)[8][2] = 0;
  (*ppalette)[9][0] = 128;
  (*ppalette)[9][1] = 0;
  (*ppalette)[9][2] = 128;
  (*ppalette)[10][0] = 0;
  (*ppalette)[10][1] = 128;
  (*ppalette)[10][2] = 128;
  (*ppalette)[11][0] = 128;
  (*ppalette)[11][1] = 128;
  (*ppalette)[11][2] = 128;
  (*ppalette)[12][0] = 0;
  (*ppalette)[12][1] = 191;
  (*ppalette)[12][2] = 255;
  (*ppalette)[13][0] = 124;
  (*ppalette)[13][1] = 252;
  (*ppalette)[13][2] = 0;
  (*ppalette)[14][0] = 64;
  (*ppalette)[14][1] = 64;
  (*ppalette)[14][2] = 64;
  (*ppalette)[15][0] = 255;
  (*ppalette)[15][1] = 69;
  (*ppalette)[15][2] = 0;
  (*ppalette)[16][0] = 192;
  (*ppalette)[16][1] = 192;
  (*ppalette)[16][2] = 192;
  (*ppalette)[17][0] = 255;
  (*ppalette)[17][1] = 140;
  (*ppalette)[17][2] = 0;
  (*ppalette)[18][0] = 0;
  (*ppalette)[18][1] = 0;
  (*ppalette)[18][2] = 205;
  (*ppalette)[19][0] = 75;
  (*ppalette)[19][1] = 0;
  (*ppalette)[19][2] = 130;
  (*ppalette)[20][0] = 240;
  (*ppalette)[20][1] = 128;
  (*ppalette)[20][2] = 128;
  (*ppalette)[21][0] = 255;
  (*ppalette)[21][1] = 127;
  (*ppalette)[21][2] = 80;
  (*ppalette)[22][0] = 250;
  (*ppalette)[22][1] = 128;
  (*ppalette)[22][2] = 114;
  (*ppalette)[23][0] = 127;
  (*ppalette)[23][1] = 255;
  (*ppalette)[23][2] = 212;
  (*ppalette)[24][0] = 255;
  (*ppalette)[24][1] = 215;
  (*ppalette)[24][2] = 0;
  (*ppalette)[25][0] = 255;
  (*ppalette)[25][1] = 165;
  (*ppalette)[25][2] = 0;
  (*ppalette)[26][0] = 139;
  (*ppalette)[26][1] = 0;
  (*ppalette)[26][2] = 139;
  (*ppalette)[27][0] = 0;
  (*ppalette)[27][1] = 139;
  (*ppalette)[27][2] = 139;
  (*ppalette)[28][0] = 205;
  (*ppalette)[28][1] = 133;
  (*ppalette)[28][2] = 63;
  (*ppalette)[29][0] = 70;
  (*ppalette)[29][1] = 130;
  (*ppalette)[29][2] = 180;
  (*ppalette)[30][0] = 0;
  (*ppalette)[30][1] = 250;
  (*ppalette)[30][2] = 154;
  (*ppalette)[31][0] = 72;
  (*ppalette)[31][1] = 61;
  (*ppalette)[31][2] = 139;
  (*ppalette)[32][0] = 184;
  (*ppalette)[32][1] = 134;
  (*ppalette)[32][2] = 11;
  (*ppalette)[33][0] = 255;
  (*ppalette)[33][1] = 160;
  (*ppalette)[33][2] = 122;
  (*ppalette)[34][0] = 135;
  (*ppalette)[34][1] = 206;
  (*ppalette)[34][2] = 250;
  (*ppalette)[35][0] = 255;
  (*ppalette)[35][1] = 99;
  (*ppalette)[35][2] = 71;
  (*ppalette)[36][0] = 112;
  (*ppalette)[36][1] = 128;
  (*ppalette)[36][2] = 144;
  (*ppalette)[37][0] = 255;
  (*ppalette)[37][1] = 105;
  (*ppalette)[37][2] = 180;
  (*ppalette)[38][0] = 189;
  (*ppalette)[38][1] = 183;
  (*ppalette)[38][2] = 107;
  (*ppalette)[39][0] = 0;
  (*ppalette)[39][1] = 206;
  (*ppalette)[39][2] = 209;
  (*ppalette)[40][0] = 60;
  (*ppalette)[40][1] = 179;
  (*ppalette)[40][2] = 113;
  (*ppalette)[41][0] = 199;
  (*ppalette)[41][1] = 21;
  (*ppalette)[41][2] = 133;
  (*ppalette)[42][0] = 238;
  (*ppalette)[42][1] = 130;
  (*ppalette)[42][2] = 238;
  (*ppalette)[43][0] = 173;
  (*ppalette)[43][1] = 255;
  (*ppalette)[43][2] = 47;
  (*ppalette)[44][0] = 143;
  (*ppalette)[44][1] = 188;
  (*ppalette)[44][2] = 143;
  (*ppalette)[45][0] = 188;
  (*ppalette)[45][1] = 143;
  (*ppalette)[45][2] = 143;
  (*ppalette)[46][0] = 255;
  (*ppalette)[46][1] = 20;
  (*ppalette)[46][2] = 147;
  (*ppalette)[47][0] = 139;
  (*ppalette)[47][1] = 69;
  (*ppalette)[47][2] = 19;
  (*ppalette)[48][0] = 148;
  (*ppalette)[48][1] = 0;
  (*ppalette)[48][2] = 211;
  (*ppalette)[49][0] = 30;
  (*ppalette)[49][1] = 144;
  (*ppalette)[49][2] = 255;
  (*ppalette)[50][0] = 119;
  (*ppalette)[50][1] = 136;
  (*ppalette)[50][2] = 153;
  (*ppalette)[51][0] = 222;
  (*ppalette)[51][1] = 184;
  (*ppalette)[51][2] = 135;
  (*ppalette)[52][0] = 123;
  (*ppalette)[52][1] = 104;
  (*ppalette)[52][2] = 238;
  (*ppalette)[53][0] = 64;
  (*ppalette)[53][1] = 224;
  (*ppalette)[53][2] = 208;
  (*ppalette)[54][0] = 135;
  (*ppalette)[54][1] = 206;
  (*ppalette)[54][2] = 235;
  (*ppalette)[55][0] = 72;
  (*ppalette)[55][1] = 209;
  (*ppalette)[55][2] = 204;
  (*ppalette)[56][0] = 210;
  (*ppalette)[56][1] = 180;
  (*ppalette)[56][2] = 140;
  (*ppalette)[57][0] = 50;
  (*ppalette)[57][1] = 205;
  (*ppalette)[57][2] = 50;
  (*ppalette)[58][0] = 233;
  (*ppalette)[58][1] = 150;
  (*ppalette)[58][2] = 122;
  (*ppalette)[59][0] = 176;
  (*ppalette)[59][1] = 196;
  (*ppalette)[59][2] = 222;
  (*ppalette)[60][0] = 65;
  (*ppalette)[60][1] = 105;
  (*ppalette)[60][2] = 225;
  (*ppalette)[61][0] = 152;
  (*ppalette)[61][1] = 251;
  (*ppalette)[61][2] = 152;
  (*ppalette)[62][0] = 220;
  (*ppalette)[62][1] = 20;
  (*ppalette)[62][2] = 60;
  (*ppalette)[63][0] = 186;
  (*ppalette)[63][1] = 85;
  (*ppalette)[63][2] = 211;
  (*ppalette)[64][0] = 240;
  (*ppalette)[64][1] = 230;
  (*ppalette)[64][2] = 140;
  (*ppalette)[65][0] = 144;
  (*ppalette)[65][1] = 238;
  (*ppalette)[65][2] = 144;
  (*ppalette)[66][0] = 47;
  (*ppalette)[66][1] = 79;
  (*ppalette)[66][2] = 79;
  (*ppalette)[67][0] = 153;
  (*ppalette)[67][1] = 50;
  (*ppalette)[67][2] = 204;
  (*ppalette)[68][0] = 46;
  (*ppalette)[68][1] = 139;
  (*ppalette)[68][2] = 87;
  (*ppalette)[69][0] = 154;
  (*ppalette)[69][1] = 205;
  (*ppalette)[69][2] = 50;
  (*ppalette)[70][0] = 138;
  (*ppalette)[70][1] = 43;
  (*ppalette)[70][2] = 226;
  (*ppalette)[71][0] = 219;
  (*ppalette)[71][1] = 112;
  (*ppalette)[71][2] = 147;
  (*ppalette)[72][0] = 107;
  (*ppalette)[72][1] = 142;
  (*ppalette)[72][2] = 35;
  (*ppalette)[73][0] = 147;
  (*ppalette)[73][1] = 112;
  (*ppalette)[73][2] = 219;
  (*ppalette)[74][0] = 244;
  (*ppalette)[74][1] = 164;
  (*ppalette)[74][2] = 96;
  (*ppalette)[75][0] = 85;
  (*ppalette)[75][1] = 107;
  (*ppalette)[75][2] = 47;
  (*ppalette)[76][0] = 102;
  (*ppalette)[76][1] = 205;
  (*ppalette)[76][2] = 170;
  (*ppalette)[77][0] = 106;
  (*ppalette)[77][1] = 90;
  (*ppalette)[77][2] = 205;
  (*ppalette)[78][0] = 34;
  (*ppalette)[78][1] = 139;
  (*ppalette)[78][2] = 34;
  (*ppalette)[79][0] = 25;
  (*ppalette)[79][1] = 25;
  (*ppalette)[79][2] = 112;
  (*ppalette)[80][0] = 32;
  (*ppalette)[80][1] = 178;
  (*ppalette)[80][2] = 170;
  (*ppalette)[81][0] = 218;
  (*ppalette)[81][1] = 112;
  (*ppalette)[81][2] = 214;
  (*ppalette)[82][0] = 100;
  (*ppalette)[82][1] = 149;
  (*ppalette)[82][2] = 237;
  (*ppalette)[83][0] = 160;
  (*ppalette)[83][1] = 82;
  (*ppalette)[83][2] = 45;
  (*ppalette)[84][0] = 178;
  (*ppalette)[84][1] = 34;
  (*ppalette)[84][2] = 34;
  (*ppalette)[85][0] = 205;
  (*ppalette)[85][1] = 92;
  (*ppalette)[85][2] = 92;
  (*ppalette)[86][0] = 105;
  (*ppalette)[86][1] = 105;
  (*ppalette)[86][2] = 105;
  (*ppalette)[87][0] = 210;
  (*ppalette)[87][1] = 105;
  (*ppalette)[87][2] = 30;
  (*ppalette)[88][0] = 165;
  (*ppalette)[88][1] = 42;
  (*ppalette)[88][2] = 42;
  (*ppalette)[89][0] = 218;
  (*ppalette)[89][1] = 165;
  (*ppalette)[89][2] = 32;
  (*ppalette)[90][0] = 221;
  (*ppalette)[90][1] = 160;
  (*ppalette)[90][2] = 221;
  (*ppalette)[91][0] = 95;
  (*ppalette)[91][1] = 158;
  (*ppalette)[91][2] = 160;
  return;
}
