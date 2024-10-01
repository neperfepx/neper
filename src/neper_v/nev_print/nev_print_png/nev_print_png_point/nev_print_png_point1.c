/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_point_.h"

void
nev_print_png_point (FILE * file, struct POINT Point, struct DATA PointData,
                     struct PRINT Print, int *show)
{
  int i, point_qty, printpoint_qty, size;
  char *texture = NULL, *bordertexture = NULL;
  int *hidden = NULL;

  neut_data_type_size (PointData.RadDataType, &size);

  point_qty = ut_array_1d_int_sum (show + 1, Point.Qty);
  if (point_qty == 0)
    return;

  texture = ut_alloc_1d_char (100);
  hidden = ut_alloc_1d_int (Point.Qty + 1);

  ut_array_1d_int_set (hidden + 1, Point.Qty, 0);

  printpoint_qty = 0;

  if (PointData.BRad > 0)
    fprintf (file,
             "#declare pointedge =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f diffuse %f reflection %f} }\n",
             PointData.BCol[0] / 255., PointData.BCol[1] / 255.,
             PointData.BCol[2] / 255., Print.lightambient, Print.lightdiffuse, Print.lightreflection);

  ut_string_string ("pointedge", &bordertexture);

  for (i = 1; i <= Point.Qty; i++)
    if (show[i])
    {
      sprintf (texture, "point%d", i);

      fprintf (file,
               "#declare point%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f diffuse %f reflection %f} }\n",
               i, PointData.Col[i][0] / 255., PointData.Col[i][1] / 255.,
               PointData.Col[i][2] / 255., PointData.Trs[i], Print.lightambient, Print.lightdiffuse, Print.lightreflection);

      char *string = ut_alloc_1d_char (100);

      if ((!strcmp (PointData.RadDataType, "none")
        || !strcmp (PointData.RadDataType, "rad")
        || !strcmp (PointData.RadDataType, "real"))
       && (!PointData.SymbolDataType || strcmp (PointData.SymbolDataType, "string")))
        nev_print_png_sphere (file, PointData.Coo[i], PointData.Rad[i], texture);

      if (PointData.SymbolDataType && !strcmp (PointData.SymbolDataType, "string"))
      {
        char *symbol = ut_alloc_1d_char (20);
        double *params = ut_alloc_1d (20);

        nev_print_symbol_expr_params (PointData.Rad[i], PointData.SymbolData[i], symbol, params);

        if (!strcmp (symbol, "sphere"))
          nev_print_png_sphere (file, PointData.Coo[i], params[0], texture);

        else if (!strcmp (symbol, "cube"))
          nev_print_png_cube (file, PointData.Coo[i], params, texture,
                              PointData.BRad, bordertexture);

        else if (!strcmp (symbol, "cylinder"))
          nev_print_png_cyl (file, PointData.Coo[i], params, texture);

        else if (!strcmp (symbol, "arrow"))
          nev_print_png_arr (file, PointData.Coo[i], params, texture);

        else if (!strcmp (symbol, "circle") || !strcmp (symbol, "disc"))
          nev_print_png_disc (file, PointData.Coo[i], params, texture);

        else if (!strcmp (symbol, "ellipsoid"))
          nev_print_png_ellipsoid (file, PointData.Coo[i], params, texture);

        else if (!strcmp (symbol, "torus"))
          nev_print_png_tor (file, PointData.Coo[i], params, texture);

        ut_free_1d_char (&symbol);
        ut_free_1d (&params);
      }

      printpoint_qty++;
      ut_free_1d_char (&string);
    }

  ut_print_message (0, 4,
                    "Number of points      reduced by %3.0f%% (to %d).\n",
                    100 * (double) hidden[0] / point_qty,
                    point_qty - hidden[0]);

  ut_free_1d_char (&texture);
  ut_free_1d_char (&bordertexture);

  return;
}
