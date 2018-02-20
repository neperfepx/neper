/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_point_.h"

void
nev_print_point (FILE * file, struct POINT Point, struct POINTDATA PointData,
		 struct PRINT Print)
{
  int i, point_qty, printpoint_qty, size;
  double ambient = (Print.showshadow == 1) ? 0.6 : 1;
  char *texture = NULL;
  int *hidden = NULL;
  double *data = ut_alloc_1d (12);

  nev_data_type_size (PointData.RadDataType, &size);

  point_qty = ut_array_1d_int_sum (Print.showpoint + 1, Point.PointQty);
  if (point_qty == 0)
    return;

  texture = ut_alloc_1d_char (100);
  hidden = ut_alloc_1d_int (Point.PointQty + 1);

  ut_array_1d_int_set (hidden + 1, Point.PointQty, 0);

  printpoint_qty = 0;

  for (i = 1; i <= Point.PointQty; i++)
    if (Print.showpoint[i])
    {
      fprintf (file,
	       "#declare point%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
	       i,
	       PointData.Col[i][0] / 255.,
	       PointData.Col[i][1] / 255.,
	       PointData.Col[i][2] / 255., PointData.trs[i], ambient);

      sprintf (texture, "point%d", i);

      char *string = ut_alloc_1d_char (100);
      if (PointData.RadDataType == NULL
	  || !strcmp (PointData.RadDataType, "rad"))
      {
	sprintf (string, "%.12f", PointData.Rad[i]);
	nev_print_sphere (file, PointData.Coo[i], string, texture);
      }
      else
      {
	double **V = ol_g_alloc ();
	double **Xp = ol_g_alloc ();
	int j, k, l, start, length, id;

	if (!PointData.Space)
	  ut_array_1d_memcpy (data, size, PointData.RadData[i]);
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
	    ut_array_1d_memcpy (V[j], 3, dR);
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

	ut_array_1d_memcpy (data, start, PointData.RadData[i]);

	id = start;
	for (k = 0; k < length / 3; k++)
	  for (l = 0; l < 3; l++)
	    data[id++] = Xp[k][l];

	if (!strcmp (PointData.RadDataType, "cube"))
	  nev_print_cube (file, PointData.Coo[i], data, texture);
	else if (!strcmp (PointData.RadDataType, "cyl"))
	  nev_print_cyl (file, PointData.Coo[i], data, texture);
	else if (!strcmp (PointData.RadDataType, "tor"))
	  nev_print_tor (file, PointData.Coo[i], data, texture);
	else if (!strcmp (PointData.RadDataType, "disc"))
	  nev_print_disc (file, PointData.Coo[i], data, texture);
	else if (!strcmp (PointData.RadDataType, "ell"))
	  nev_print_ellipsoid (file, PointData.Coo[i], data, texture);
	else
	  abort ();

	ol_g_free (Xp);
	ol_g_free (V);
      }

      printpoint_qty++;
      ut_free_1d_char (string);
    }

  ut_print_message (0, 4,
		    "Number of points      reduced by %3.0f\%% (to %d).\n",
		    100 * (double) hidden[0] / (double) point_qty,
		    point_qty - hidden[0]);

  ut_free_1d (data);

  return;
}
