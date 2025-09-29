/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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

  if (Tess.Type && !strcmp (Tess.Type, "periodic"))
    neut_tess_fprintf_per (Tess, file);

  if (Tess.ScaleQty > 1)
    neut_tess_fprintf_scale (Tess, file);

  neut_tess_fprintf_foot (file);

  return;
}

void
neut_tess_name_fprintf_gmsh (char *filename, struct TESS Tess)
{
  FILE *file = ut_file_open (filename, "w");

  neut_tess_fprintf_gmsh (file, Tess);

  ut_file_close (file, filename, "w");

  return;
}

void
neut_tess_fprintf_gmsh (FILE * file, struct TESS Tess)
{
  int i, j;

  for (i = 1; i <= Tess.VerQty; i++)
    if (!Tess.VerState || Tess.VerState[i] != -1)
    {
      fprintf (file, "Point (%d) = {" REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT "}; ", i,
               Tess.VerCoo[i][0], Tess.VerCoo[i][1], Tess.VerCoo[i][2]);
      fprintf (file, "Physical Point (%d) = {%d};\n", i, i);
    }

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (!Tess.EdgeState || Tess.EdgeState[i] != -1)
    {
      fprintf (file, "Line (%d) = {%d,%d}; ", i, Tess.EdgeVerNb[i][0],
               Tess.EdgeVerNb[i][1]);
      fprintf (file, "Physical Line (%d) = {%d};\n", i, i);
    }

  for (i = 1; i <= Tess.FaceQty; i++)
    if (!Tess.FaceState || Tess.FaceState[i] != -1)
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
    if (!Tess.PolyState || Tess.PolyState[i] != -1)
    {
      fprintf (file, "Surface Loop (%d) = {", i);
      for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      {
        fprintf (file, "%d%s", Tess.PolyFaceNb[i][j] * Tess.PolyFaceOri[i][j],
                 (j < Tess.PolyFaceQty[i]) ? "," : "");
      }
      fprintf (file, "};\n");
      fprintf (file, "Volume (%d) = {%d};\n", i, i);
      fprintf (file, "Physical Volume (%d) = {%d};\n", i, i);
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
      fprintf (file, REAL_PRINT_FORMAT " " REAL_PRINT_FORMAT " " REAL_PRINT_FORMAT "\n", Tess.VerCoo[i][0],
               Tess.VerCoo[i][1], Tess.VerCoo[i][2]);

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
  int i, j, face;

  for (i = 1; i <= Tess.VerQty; i++)
  {
    fprintf (file, "v ");
    ut_array_1d_fprintf (file, Tess.VerCoo[i], 3, REAL_PRINT_FORMAT);
  }

  if (Tess.Dim == 2)
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      fprintf (file, "g cell%d\n", i);
      fprintf (file, "f");
      for (j = 1; j <= Tess.FaceVerQty[i]; j++)
        fprintf (file, " %d", Tess.FaceVerNb[i][j]);
      fprintf (file, "\n");
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
            fprintf (file, REAL_PRINT_FORMAT " ", Tess.VerCoo[ver][l]);

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
            fprintf (file, REAL_PRINT_FORMAT " ", Tess.VerCoo[ver][l]);

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
neut_tess_fprintf_fe (FILE * file, struct TESS Tess)
{
  int i, j;

  fprintf (file, "vertices\n");
  for (i = 1; i <= Tess.VerQty; i++)
  {
    fprintf (file, "%d ", i);
    ut_array_1d_fprintf (file, Tess.VerCoo[i], 3, REAL_PRINT_FORMAT);
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

void
neut_tess_name_fprintf_stl (char *filename, struct TESS Tess)
{
  FILE *file = ut_file_open (filename, "w");

  neut_tess_fprintf_stl (file, Tess);

  ut_file_close (file, filename, "w");

  return;
}

void
neut_tess_fprintf_stl (FILE * file, struct TESS Tess)
{
  int i, j, k;
  struct NODES N;
  struct MESH M;
  double *n = ut_alloc_1d (3);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  fprintf (file, "soliddart\n");

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    neut_tess_face_interpolmesh (Tess, i, &N, &M);

    for (j = 1; j <= M.EltQty; j++)
    {
      neut_mesh_elt_normal (M, N, j, n);
      fprintf (file, "  facet normal ");
      ut_array_1d_fprintf (file, n, 3, REAL_PRINT_FORMAT);
      fprintf (file, "    outer loop\n");
      for (k = 0; k < 3; k++)
      {
        fprintf (file, "      vertex ");
        ut_array_1d_fprintf (file, N.NodeCoo[M.EltNodes[j][k]], 3,
                             REAL_PRINT_FORMAT);
      }
      fprintf (file, "    endloop\n");
      fprintf (file, "  endfacet\n");
    }

    neut_mesh_free (&M);
    neut_nodes_reset (&N);
  }

  fprintf (file, "endsoliddart\n");

  ut_free_1d (&n);

  return;
}

void
neut_tess_name_fprintf_stl_bycell (char *filename, struct TESS Tess)
{
  int i;
  char *filename2 = ut_alloc_1d_char (strlen (filename) + 100);
  char *id = ut_alloc_1d_char (100);
  FILE *file = NULL;
  int length = ut_num_tenlen_int (Tess.CellQty);
  char format[5];

  sprintf (format, "%%0%dd", length);

  for (i = 1; i <= Tess.CellQty; i++)
  {
    sprintf (id, format, i);
    sprintf (filename2, "%s-%s.stl", filename, id);
    file = ut_file_open (filename2, "w");
    neut_tess_cell_fprintf_stl (file, Tess, i);
    ut_file_close (file, filename2, "w");
  }

  ut_free_1d_char (&filename2);
  ut_free_1d_char (&id);

  return;
}

void
neut_tess_cell_fprintf_stl (FILE * file, struct TESS Tess, int cell)
{
  int i, j, k;
  struct NODES N;
  struct MESH M;
  double *n = ut_alloc_1d (3);
  int faceqty, face, *faces = NULL;

  if (Tess.Dim != 3)
    abort ();

  neut_tess_poly_faces (Tess, cell, &faces, &faceqty);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  fprintf (file, "soliddart\n");

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    neut_tess_face_interpolmesh (Tess, face, &N, &M);

    for (j = 1; j <= M.EltQty; j++)
    {
      neut_mesh_elt_normal (M, N, j, n);
      fprintf (file, "  facet normal ");
      ut_array_1d_fprintf (file, n, 3, REAL_PRINT_FORMAT);
      fprintf (file, "    outer loop\n");
      for (k = 0; k < 3; k++)
      {
        fprintf (file, "      vertex ");
        ut_array_1d_fprintf (file, N.NodeCoo[M.EltNodes[j][k]], 3, REAL_PRINT_FORMAT);
      }
      fprintf (file, "    endloop\n");
      fprintf (file, "  endfacet\n");
    }

    neut_mesh_free (&M);
    neut_nodes_reset (&N);
  }

  fprintf (file, "endsoliddart\n");

  ut_free_1d (&n);
  ut_free_1d_int (&faces);

  return;
}

void
neut_tess_fprintf_svg (FILE * file, char *format, struct TESS Tess)
{
  int i, j;
  double *bboxsize = ut_alloc_1d (3);
  char *unit = NULL;
  double **bbox = ut_alloc_2d (3, 2);
  char **vars = NULL, **vals = NULL;
  int varqty;

  ut_string_function (format, NULL, &vars, &vals, &varqty);
  ut_string_string ("", &unit);
  for (i = 0; i < varqty; i++)
    if (!strcmp (vars[i], "unit"))
      ut_string_string (vals[i], &unit);
    else
      ut_print_exprbug (vars[i]);


  neut_tess_bbox (Tess, bbox);

  neut_tess_bboxsize (Tess, bboxsize);

  fprintf (file, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
  fprintf (file, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  fprintf (file, "<!-- Generator: Neper (https://neper.info) -->\n");
  fprintf (file, "<svg width=\"%f%s\" height=\"%f%s\" viewBox=\"%f %f %f %f\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\">\n",
                 bboxsize[0], unit, bboxsize[1], unit, bbox[0][0], bbox[1][0], bbox[0][1], bbox[1][1]);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    fprintf (file,"  <path stroke=\"none\" fill=\"#FFFFFF\" d=\"M ");

    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      fprintf (file, " %f,%f %s", Tess.VerCoo[Tess.FaceVerNb[i][j]][0], Tess.VerCoo[Tess.FaceVerNb[i][j]][1],
          j < Tess.FaceVerQty[i] ? "L" : "Z");

    fprintf (file, "\"/>\n");
  }
  fprintf (file, "</svg>\n");

  ut_free_1d (&bboxsize);
  ut_free_1d_char (&unit);
  ut_free_2d (&bbox, 3);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);

  return;
}

void
neut_tess_fprintf_ovm (FILE *file, struct TESS Tess)
{
  fprintf (file, "OVM ASCII\n");

  fprintf (file, "Vertices\n");
  fprintf (file, "%d\n", Tess.VerQty);
  ut_array_2d_fprintf (file, Tess.VerCoo + 1, Tess.VerQty, 3, "%.12f");

  fprintf (file, "Edges\n");
  fprintf (file, "%d\n", Tess.EdgeQty);
  ut_array_2d_int_fprintf_addval (file, Tess.EdgeVerNb + 1, Tess.EdgeQty, 2, -1, "%d");

  fprintf (file, "Faces\n");
  fprintf (file, "%d\n", Tess.FaceQty);
  for (int i = 1; i <= Tess.FaceQty; i++)
  {
    fprintf (file, "%d", Tess.FaceVerQty[i]);
    for (int j = 1; j <= Tess.FaceVerQty[i]; j++)
      fprintf (file, " %d", (Tess.FaceEdgeNb[i][j] - 1) * 2);
    fprintf (file, "\n");
  }

  fprintf (file, "Polyhedra\n");
  fprintf (file, "%d\n", Tess.PolyQty);
  for (int i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, "%d", Tess.PolyFaceQty[i]);
    for (int j = 1; j <= Tess.PolyFaceQty[i]; j++)
      fprintf (file, " %d", (Tess.PolyFaceNb[i][j] - 1) * 2);
    fprintf (file, "\n");
  }

  return;
}
