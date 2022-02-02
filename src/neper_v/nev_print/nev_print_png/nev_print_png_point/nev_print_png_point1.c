/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_point_.h"

void
nev_print_png_point (FILE * file, struct POINT Point, struct DATA PointData,
                 struct PRINT Print)
{
  int i, point_qty, printpoint_qty, size;
  double ambient = (Print.sceneshadow == 1) ? 0.6 : 1;
  char *texture = NULL, *bordertexture = NULL;
  int *hidden = NULL;
  double *data = ut_alloc_1d (12);

  neut_data_type_size (PointData.RadDataType, &size);

  point_qty = ut_array_1d_int_sum (Print.showpoint + 1, Point.PointQty);
  if (point_qty == 0)
    return;

  texture = ut_alloc_1d_char (100);
  hidden = ut_alloc_1d_int (Point.PointQty + 1);

  ut_array_1d_int_set (hidden + 1, Point.PointQty, 0);

  printpoint_qty = 0;

  if (PointData.BRad > 0)
    fprintf (file,
             "#declare pointedge =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
             PointData.BCol[0] / 255., PointData.BCol[1] / 255.,
             PointData.BCol[2] / 255., ambient);

  ut_string_string ("pointedge", &bordertexture);

  for (i = 1; i <= Point.PointQty; i++)
    if (Print.showpoint[i])
    {
      fprintf (file,
               "#declare point%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
               i, PointData.Col[i][0] / 255., PointData.Col[i][1] / 255.,
               PointData.Col[i][2] / 255., PointData.Trs[i], ambient);

      sprintf (texture, "point%d", i);

      char *string = ut_alloc_1d_char (100);
      if (!strcmp (PointData.RadDataType, "none")
       || !strcmp (PointData.RadDataType, "real"))
      {
        sprintf (string, "%.12f", PointData.Rad[i]);
        nev_print_png_sphere (file, PointData.Coo[i], string, texture);
      }
      else
      {
        double **V = ol_g_alloc ();
        double **Xp = ol_g_alloc ();
        int j, k, l, start, length, id;

        if (!PointData.Space)
          ut_array_1d_memcpy (PointData.RadData[i], size, data);
        else if (!strcmp (PointData.Space, "rodrigues"))
        {
          double *dR = ol_R_alloc ();
          double *R = ol_R_alloc ();
          for (j = 0; j < 3; j++)
          {
            ol_R_set_zero (dR);
            dR[j] = 0.001;
            ol_R_R_R_ref (PointData.Coo[i], dR, R);
            ut_array_1d_sub (PointData.Coo[i], R, 3, dR);
            ut_array_1d_scale (dR, 3, 1. / 0.001);
            ut_array_1d_memcpy (dR, 3, V[j]);
          }

          ol_R_free (dR);
          ol_R_free (R);
        }
        else
          abort ();

        if (!strcmp (PointData.RadDataType, "cube"))
        {
          start = 1;
          length = 9;
        }
        else if (!strcmp (PointData.RadDataType, "cyl"))
        {
          start = 2;
          length = 3;
        }
        else if (!strcmp (PointData.RadDataType, "arr"))
        {
          start = 2;
          length = 3;
        }
        else if (!strcmp (PointData.RadDataType, "tor"))
        {
          start = 2;
          length = 3;
        }
        else if (!strcmp (PointData.RadDataType, "disc"))
        {
          start = 2;
          length = 3;
        }
        else if (!strcmp (PointData.RadDataType, "ell"))
        {
          start = 3;
          length = 9;
        }
        else
          abort ();

        id = start;
        for (k = 0; k < length / 3; k++)
          for (l = 0; l < 3; l++)
            Xp[k][l] = PointData.RadData[i][id++];

        ol_g_g_g_ref (Xp, V, Xp);

        ut_array_1d_memcpy (PointData.RadData[i], start, data);

        id = start;
        for (k = 0; k < length / 3; k++)
          for (l = 0; l < 3; l++)
            data[id++] = Xp[k][l];

        if (!strcmp (PointData.RadDataType, "cube"))
          nev_print_png_cube (file, PointData.Coo[i], data, texture, PointData.BRad, bordertexture);
        else if (!strcmp (PointData.RadDataType, "cyl"))
          nev_print_png_cyl (file, PointData.Coo[i], data, texture);
        else if (!strcmp (PointData.RadDataType, "arr"))
          nev_print_png_arr (file, PointData.Coo[i], data, texture);
        else if (!strcmp (PointData.RadDataType, "tor"))
          nev_print_png_tor (file, PointData.Coo[i], data, texture);
        else if (!strcmp (PointData.RadDataType, "disc"))
          nev_print_png_disc (file, PointData.Coo[i], data, texture);
        else if (!strcmp (PointData.RadDataType, "ell"))
          nev_print_png_ellipsoid (file, PointData.Coo[i], data, texture);
        else
          abort ();

        ol_g_free (Xp);
        ol_g_free (V);
      }

      printpoint_qty++;
      ut_free_1d_char (&string);
    }

  ut_print_message (0, 4,
                    "Number of points      reduced by %3.0f%% (to %d).\n",
                    100 * (double) hidden[0] / point_qty,
                    point_qty - hidden[0]);

  ut_free_1d (&data);
  ut_free_1d_char (&texture);
  ut_free_1d_char (&bordertexture);

  return;
}
