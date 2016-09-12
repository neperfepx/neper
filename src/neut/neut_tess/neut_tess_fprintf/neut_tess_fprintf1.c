/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fprintf_.h"

void
neut_tess_fprintf (FILE * file, struct TESS Tess)
{
  neut_tess_fprintf_head (Tess, file);

  neut_tess_fprintf_cell (Tess, file);

  if (Tess.Dim >= 0)
    neut_tess_fprintf_ver (Tess, file);

  if (Tess.Dim >= 1)
    neut_tess_fprintf_edge (Tess, file);

  if (Tess.Dim >= 2)
    neut_tess_fprintf_face (Tess, file);

  if (Tess.Dim == 3)
    neut_tess_fprintf_poly (Tess, file);

  neut_tess_fprintf_dom (Tess, file);

  if (!strncmp (Tess.Type, "periodic", 8))
    neut_tess_fprintf_per (Tess, file);

  if (Tess.ScaleQty > 1)
    neut_tess_fprintf_scale (Tess, file);

  neut_tess_fprintf_foot (file);

  return;
}

void
neut_tess_name_fprintf_gmsh (char *filename, struct TESS Tess)
{
  FILE* file = ut_file_open (filename, "w");

  neut_tess_fprintf_gmsh (file, Tess);

  ut_file_close (file, filename, "w");

  return;
}

void
neut_tess_fprintf_gmsh (FILE * file, struct TESS Tess)
{
  int i, j;

  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerState[i] != -1)
      fprintf (file, "Point (%d) = {%.12f,%.12f,%.12f};\n",
	       i, Tess.VerCoo[i][0], Tess.VerCoo[i][1], Tess.VerCoo[i][2]);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeState[i] != -1)
      fprintf (file, "Line (%d) = {%d,%d};\n",
	       i, Tess.EdgeVerNb[i][0], Tess.EdgeVerNb[i][1]);

  for (i = 1; i <= Tess.FaceQty; i++)
    if (Tess.FaceState[i] != -1)
    {
      fprintf (file, "Line Loop (%d) = {", i);
      for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      {
	fprintf (file, "%d%s", Tess.FaceEdgeNb[i][j] * Tess.FaceEdgeOri[i][j],
		 (j < Tess.FaceVerQty[i]) ? "," : "");
      }
      fprintf (file, "};\n");
      fprintf (file, "Plane Surface (%d) = {%d}; ", i, i);
      fprintf (file, "Physical Surface (%d) = {%d};\n", i, i);
    }

  for (i = 1; i <= Tess.PolyQty; i++)
    if (Tess.PolyState[i] != -1)
    {
      fprintf (file, "Surface Loop (%d) = {", i);
      for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      {
	fprintf (file, "%d%s", Tess.PolyFaceNb[i][j] * Tess.PolyFaceOri[i][j],
		 (j < Tess.PolyFaceQty[i]) ? "," : "");
      }
      fprintf (file, "};\n");
      fprintf (file, "Volume (%d) = {%d};\n", i, i);
    }

  return;
}

void
neut_tess_fprintf_ply (FILE * file, struct TESS Tess)
{
  int i, j;

  fprintf (file, "ply\n");
  fprintf (file, "format ascii 1.0\n");
  fprintf (file, "element vertex %d\n", Tess.VerQty);
  fprintf (file, "property float x\n");
  fprintf (file, "property float y\n");
  fprintf (file, "property float z\n");
  fprintf (file, "element face %d\n", Tess.FaceQty);
  fprintf (file, "property list uchar int vertex_indices\n");
  fprintf (file, "element cell %d\n", Tess.CellQty);
  fprintf (file, "property list uchar int face_indices\n");
  fprintf (file, "end_header\n");

  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerState[i] != -1)
      fprintf (file, "%.12f %.12f %.12f\n",
	       Tess.VerCoo[i][0], Tess.VerCoo[i][1], Tess.VerCoo[i][2]);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    fprintf (file, "%d ", Tess.FaceVerQty[i]);
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      fprintf (file, "%d%s", Tess.FaceVerNb[i][j] - 1,
	       (j < Tess.FaceVerQty[i]) ? " " : "\n");
  }

  if (Tess.Dim == 2)
    for (i = 1; i <= Tess.CellQty; i++)
      fprintf (file, "1 %d\n", i - 1);

  else if (Tess.Dim == 3)
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      fprintf (file, "%d ", Tess.PolyFaceQty[i]);
      for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
	fprintf (file, "%d%s", Tess.PolyFaceNb[i][j] - 1,
		 (j < Tess.PolyFaceQty[i]) ? " " : "\n");
    }

  else
    abort ();

  return;
}

void
neut_tess_fprintf_obj (FILE * file, struct TESS Tess)
{
  int i, j, edge, face;

  for (i = 1; i <= Tess.VerQty; i++)
  {
    fprintf (file, "v ");
    ut_array_1d_fprintf (file, Tess.VerCoo[i], 3, "%.12f");
  }

  if (Tess.Dim == 2)
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      fprintf (file, "g cell%d\n", i);
      for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      {
	edge = Tess.FaceEdgeNb[i][j];
	fprintf (file, "f %d %d %d %d\n",
		 Tess.EdgeVerNb[edge][0], Tess.EdgeVerNb[edge][1],
		 Tess.EdgeVerNb[edge][1], Tess.EdgeVerNb[edge][0]);
      }
    }

  else if (Tess.Dim == 3)
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      fprintf (file, "g cell%d\n", i);
      for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      {
	face = Tess.PolyFaceNb[i][j];
	fprintf (file, "f ");
	ut_array_1d_int_fprintf (file, Tess.FaceVerNb[face] + 1,
				 Tess.FaceVerQty[face], "%d");
      }
    }

  return;
}

void
neut_tess_fprintf_dec (FILE * file, struct TESS Tess)
{
  int i, j, k, l, face, ver, faceid;

  faceid = 0;
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, "poly reg %4d &\n", i);

    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
    {
      face = Tess.PolyFaceNb[i][j];

      fprintf (file, "face ID %9d ", ++faceid);

      if (Tess.PolyFaceOri[i][j] == 1)
	for (k = Tess.FaceVerQty[face]; k >= 1; k--)
	{
	  ver = Tess.FaceVerNb[face][k];
	  if (k < Tess.FaceVerQty[face])
	    fprintf (file, "                  ");

	  for (l = 0; l < 3; l++)
	    fprintf (file, "%.12f ", Tess.VerCoo[ver][l]);

	  if (!(j == Tess.PolyFaceQty[i] && k == 1))
	    fprintf (file, "&\n");
	}

      else
	for (k = 1; k <= Tess.FaceVerQty[face]; k++)
	{
	  ver = Tess.FaceVerNb[face][k];
	  if (k > 1)
	    fprintf (file, "                  ");

	  for (l = 0; l < 3; l++)
	    fprintf (file, "%.12f ", Tess.VerCoo[ver][l]);

	  if (!(j == Tess.PolyFaceQty[i] && k == Tess.FaceVerQty[face]))
	    fprintf (file, "&\n");
	}
    }
    fprintf (file, "\n");
  }

  fprintf (file, "ret\n");

  return;
}

void
neut_tess_name_fprintf (char *filename, struct TESS Tess)
{
  FILE *fp = ut_file_open (filename, "w");
  neut_tess_fprintf (fp, Tess);
  ut_file_close (fp, filename, "w");

  return;
}

void
neut_tess_fprintf_fe (FILE *file, struct TESS Tess)
{
  int i, j;

  fprintf (file, "vertices\n");
  for (i = 1; i <= Tess.VerQty; i++)
  {
    fprintf (file, "%d ", i);
    ut_array_1d_fprintf (file, Tess.VerCoo[i], 3, "%.12f");
  }

  fprintf (file, "\nedges\n");
  for (i = 1; i <= Tess.EdgeQty; i++)
    fprintf (file, "%d %d %d\n", i, Tess.EdgeVerNb[i][0],
				    Tess.EdgeVerNb[i][1]);

  fprintf (file, "\nfaces\n");
  for (i = 1; i <= Tess.FaceQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      fprintf (file, " %d", Tess.FaceEdgeNb[i][j] * Tess.FaceEdgeOri[i][j]);
    fprintf (file, "\n");
  }

  fprintf (file, "\nbodies\n");
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      fprintf (file, " %d", Tess.PolyFaceNb[i][j] * Tess.PolyFaceOri[i][j]);
    fprintf (file, "\n");
  }

  return;
}
