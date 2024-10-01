/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_utils_.h"

void
nev_print_png_segment (FILE * file, double *coo1, double *coo2, double rad,
                   char *edge_texture)
{
  if (!nev_print_png_cylinder_test (coo1, coo2, rad))
    return;

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo1[0],
           coo1[1], coo1[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo2[0],
           coo2[1], coo2[2]);
  fprintf (file, "              %18.15g\n", rad);

  if (edge_texture)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_png_cone (FILE * file, double *coo1, double *coo2, double rad,
                char *edge_texture)
{
  if (!nev_print_png_cylinder_test (coo1, coo2, rad))
    return;

  fprintf (file, "    cone {<%18.15g,%18.15g,%18.15g>,\n", coo1[0], coo1[1],
           coo1[2]);
  fprintf (file, "              %18.15g,\n", rad);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo2[0],
           coo2[1], coo2[2]);
  fprintf (file, "              0\n");

  if (edge_texture)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_png_segment_wsph (FILE * file, double *coo1, double *coo2,
                        double rad, char *edge_texture)
{
  if (!nev_print_png_cylinder_test (coo1, coo2, rad))
    return;

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo1[0],
           coo1[1], coo1[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo2[0],
           coo2[1], coo2[2]);
  fprintf (file, "              %18.15g\n", rad);

  if (edge_texture)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n", coo1[0], coo1[1],
           coo1[2]);
  fprintf (file, "              %18.15g\n", rad);

  if (edge_texture)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n", coo2[0], coo2[1],
           coo2[2]);
  fprintf (file, "              %18.15g\n", rad);

  if (edge_texture)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_png_sphere (FILE * file, double *coo, double rad, char *texture)
{
  if (rad <= 0)
    return;

  fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n", coo[0], coo[1], coo[2]);
  fprintf (file, "              %18.15g\n", rad);

  if (texture)
    fprintf (file, "    texture { %s }\n", texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_png_ellipsoid (FILE * file, double *coo, double *rad, char *texture)
{
  fprintf (file,
           "    sphere {<0.,0.,0.>, 1. scale <%18.15g,%18.15g,%18.15g> matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,0.,0.,0.> translate <%18.15g,%18.15g,%18.15g>\n",
           rad[0], rad[1], rad[2], rad[3], rad[4], rad[5], rad[6], rad[7],
           rad[8], rad[9], rad[10], rad[11], coo[0], coo[1], coo[2]);

  if (texture)
    fprintf (file, "    texture { %s }\n", texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_png_cube (FILE * file, double *coo, double *data, char *texture,
                    double brad, char *bordertexture)
{
  fprintf (file,
           "    box {<-.5,-.5,-.5>, <.5,.5,.5> scale %18.15g matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,0.,0.,0.> translate <%18.15g,%18.15g,%18.15g>\n",
           2 * data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
           data[8], data[9], coo[0], coo[1], coo[2]);

  if (texture)
    fprintf (file, "    texture { %s }\n", texture);
  fprintf (file, "  }\n");

  if (brad > 0)
  {
    double **vercoo = ut_alloc_2d (8, 3);
    int i, **edges = ut_alloc_2d_int (12, 2);

    ut_array_1d_set_3 (vercoo[0] , 0, 0, 0);
    ut_array_1d_set_3 (vercoo[1] , 1, 0, 0);
    ut_array_1d_set_3 (vercoo[2] , 1, 1, 0);
    ut_array_1d_set_3 (vercoo[3] , 0, 1, 0);
    ut_array_1d_set_3 (vercoo[4] , 0, 0, 1);
    ut_array_1d_set_3 (vercoo[5] , 1, 0, 1);
    ut_array_1d_set_3 (vercoo[6] , 1, 1, 1);
    ut_array_1d_set_3 (vercoo[7] , 0, 1, 1);
    ut_array_2d_addval (vercoo, 8, 3, -0.5, vercoo);
    ut_array_1d_int_set_2 (edges[0], 0, 1);
    ut_array_1d_int_set_2 (edges[1], 1, 2);
    ut_array_1d_int_set_2 (edges[2], 2, 3);
    ut_array_1d_int_set_2 (edges[3], 3, 0);
    ut_array_1d_int_set_2 (edges[4], 4, 5);
    ut_array_1d_int_set_2 (edges[5], 5, 6);
    ut_array_1d_int_set_2 (edges[6], 6, 7);
    ut_array_1d_int_set_2 (edges[7], 7, 4);
    ut_array_1d_int_set_2 (edges[8], 0, 4);
    ut_array_1d_int_set_2 (edges[9], 1, 5);
    ut_array_1d_int_set_2 (edges[10], 2, 6);
    ut_array_1d_int_set_2 (edges[11], 3, 7);

    fprintf (file, "union\n{\n");

    for (i = 0; i < 12; i++)
      nev_print_png_segment_wsph (file, vercoo[edges[i][0]], vercoo[edges[i][1]], brad / (2 * data[0]), bordertexture);

    fprintf (file,
             "    scale %18.15g matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,0.,0.,0.> translate <%18.15g,%18.15g,%18.15g>\n",
             2 * data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
             data[8], data[9], coo[0], coo[1], coo[2]);

    fprintf (file, "}\n");

    ut_free_2d (&vercoo, 8);
    ut_free_2d_int (&edges, 12);
  }

  return;
}

void
nev_print_png_hcp (FILE * file, double *coo, double *rad, char *texture,
                   double brad, char *bordertexture)
{
  fprintf (file, "    prism {\n");
  fprintf (file, "      linear_sweep\n");
  fprintf (file, "      linear_spline\n");
  // a = 1, c = 1.633
  fprintf (file, "      " REAL_PRINT_FORMAT ",\n", -0.5 * 1.633); // c = 1.633
  fprintf (file, "      " REAL_PRINT_FORMAT ",\n",  0.5 * 1.633); // c = 1.633
  fprintf (file, "      7,\n");
  fprintf (file, "<-.5,-.866>, <-1.,0.>, <-.5,.866>, <.5,.866>, <1.,0.>,<.5,-.866>,<-.5,-.866>\n");
  fprintf (file, "scale %18.15g\n", rad[0]); // scaling to a
  fprintf (file, "rotate <90.,0.,0.>\n");
  fprintf (file, "matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,0.,0.,0.>\n",
           rad[1], rad[2], rad[3], rad[4], rad[5], rad[6], rad[7], rad[8], rad[9]);
  fprintf (file, "translate <%18.15g,%18.15g,%18.15g>\n", coo[0], coo[1], coo[2]);

  if (texture)
    fprintf (file, "    texture { %s }\n", texture);
  fprintf (file, "  }\n");

  if (brad > 0)
  {
    double **vercoo = ut_alloc_2d (12, 3);
    int i, **edges = ut_alloc_2d_int (18, 2);

    ut_array_1d_set_3 (vercoo[0] , -0.5, -0.866, -0.8165);
    ut_array_1d_set_3 (vercoo[1] , -1.0,  0.000, -0.8165);
    ut_array_1d_set_3 (vercoo[2] , -0.5,  0.866, -0.8165);
    ut_array_1d_set_3 (vercoo[3] ,  0.5,  0.866, -0.8165);
    ut_array_1d_set_3 (vercoo[4] ,  1.0,  0.000, -0.8165);
    ut_array_1d_set_3 (vercoo[5] ,  0.5, -0.866, -0.8165);

    ut_array_1d_set_3 (vercoo[6] , -0.5, -0.866,  0.8165);
    ut_array_1d_set_3 (vercoo[7] , -1.0,  0.000,  0.8165);
    ut_array_1d_set_3 (vercoo[8] , -0.5,  0.866,  0.8165);
    ut_array_1d_set_3 (vercoo[9] ,  0.5,  0.866,  0.8165);
    ut_array_1d_set_3 (vercoo[10],  1.0,  0.000,  0.8165);
    ut_array_1d_set_3 (vercoo[11],  0.5, -0.866,  0.8165);

    ut_array_1d_int_set_2 (edges[0], 0, 1);
    ut_array_1d_int_set_2 (edges[1], 1, 2);
    ut_array_1d_int_set_2 (edges[2], 2, 3);
    ut_array_1d_int_set_2 (edges[3], 3, 4);
    ut_array_1d_int_set_2 (edges[4], 4, 5);
    ut_array_1d_int_set_2 (edges[5], 5, 0);

    ut_array_1d_int_set_2 (edges[6], 6, 7);
    ut_array_1d_int_set_2 (edges[7], 7, 8);
    ut_array_1d_int_set_2 (edges[8], 8, 9);
    ut_array_1d_int_set_2 (edges[9], 9, 10);
    ut_array_1d_int_set_2 (edges[10], 10, 11);
    ut_array_1d_int_set_2 (edges[11], 11, 6);

    ut_array_1d_int_set_2 (edges[12], 0, 6);
    ut_array_1d_int_set_2 (edges[13], 1, 7);
    ut_array_1d_int_set_2 (edges[14], 2, 8);
    ut_array_1d_int_set_2 (edges[15], 3, 9);
    ut_array_1d_int_set_2 (edges[16], 4,10);
    ut_array_1d_int_set_2 (edges[17], 5,11);

    fprintf (file, "union\n{\n");

    for (i = 0; i < 18; i++)
      nev_print_png_segment_wsph (file, vercoo[edges[i][0]], vercoo[edges[i][1]], brad / (2 * rad[0]), bordertexture);

    fprintf (file, "scale %18.15g\n", rad[0]); // scaling to a
    fprintf (file, "matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,0.,0.,0.>\n",
             rad[1], rad[2], rad[3], rad[4], rad[5], rad[6], rad[7], rad[8], rad[9]);
    fprintf (file, "translate <%18.15g,%18.15g,%18.15g>\n", coo[0], coo[1], coo[2]);

    fprintf (file, "}\n");

    ut_free_2d (&vercoo, 8);
    ut_free_2d_int (&edges, 12);
  }

  return;
}

void
nev_print_png_cyl (FILE * file, double *coo, double *data, char *texture)
{
  double *v = ut_alloc_1d (3);

  ut_array_1d_memcpy (data + 2, 3, v);
  ut_array_1d_scale (v, 3, 1. / ut_array_1d_norm (v, 3));
  ut_array_1d_scale (v, 3, data[1] * .5);

  if (data[0] > 0 && data[1] > 0)
  {
    fprintf (file,
             "    cylinder {<%18.15g,%18.15g,%18.15g>, <%18.15g,%18.15g,%18.15g>, %18.15g translate <%18.15g,%18.15g,%18.15g>\n",
             -v[0], -v[1], -v[2], v[0], v[1], v[2], data[0], coo[0], coo[1],
             coo[2]);

    if (texture)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");
  }

  ut_free_1d (&v);

  return;
}

void
nev_print_png_tor (FILE * file, double *coo, double *rad, char *texture)
{
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);
  double **g = ol_g_alloc ();

  ut_array_1d_set_3 (v1, 0, 1, 0);
  ut_array_1d_memcpy (rad + 2, 3, v2);
  ol_vect_vect_g (v1, v2, g);

  if (rad[0] > 0 && rad[1] > 0)
  {
    fprintf (file,
             "    torus {%18.15g,%18.15g matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g>\n",
             rad[0], rad[1], g[0][0], g[0][1], g[0][2], g[1][0], g[1][1],
             g[1][2], g[2][0], g[2][1], g[2][2], coo[0], coo[1], coo[2]);

    if (texture)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");
  }

  ut_free_1d (&v1);
  ut_free_1d (&v2);
  ol_g_free (g);

  return;
}

void
nev_print_png_disc (FILE * file, double *coo, double *rad, char *texture)
{
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);
  double **g = ol_g_alloc ();

  ut_array_1d_set_3 (v1, 0, 1, 0);
  ut_array_1d_memcpy (rad + 2, 3, v2);
  ol_vect_vect_g (v1, v2, g);

  if (rad[0] > 0)
  {
    fprintf (file,
             "    disc {<%18.15g,%18.15g,%18.15g>,  <%18.15g,%18.15g,%18.15g>, %18.15g\n",
             coo[0], coo[1], coo[2], rad[1], rad[2], rad[3], rad[0]);

    if (texture)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");
  }

  ut_free_1d (&v1);
  ut_free_1d (&v2);
  ol_g_free (g);

  return;
}

void
nev_print_png_triangle_edge (FILE * file, double *coo1, double *coo2,
                         double *coo3, char *edge_rad, char *edge_texture)
{
  double rad;

  if (!edge_rad || (sscanf (edge_rad, "%lf", &rad) == 1 && rad <= 0))
    return;

  fprintf (file, "  union {\n");

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo1[0],
           coo1[1], coo1[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo2[0],
           coo2[1], coo2[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo2[0],
           coo2[1], coo2[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo3[0],
           coo3[1], coo3[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo3[0],
           coo3[1], coo3[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo1[0],
           coo1[1], coo1[2]);
  fprintf (file, "              %s }\n", edge_rad);

  if (edge_texture)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_png_mesh2d (FILE * file, struct NODES Nodes, struct MESH Mesh,
                      int *showelt, int **rgb, double *trs, char *coltype,
                      struct PRINT Print)
{
  int i, showeltqty;

  if (Nodes.NodeQty == 0 || Mesh.EltQty == 0)
    return;

  showeltqty =
    showelt ? ut_array_1d_int_sum (showelt + 1, Mesh.EltQty) : Mesh.EltQty;

  if (showeltqty == 0)
    return;

  fprintf (file, "mesh2 {\n");

  // vertex_vectors
  fprintf (file, "  vertex_vectors {\n");
  fprintf (file, "    %d,\n", Nodes.NodeQty);

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "    <" REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT ">%s\n",
             Nodes.NodeCoo[i][0],
             Nodes.NodeCoo[i][1], Nodes.NodeCoo[i][2],
             (i < Nodes.NodeQty) ? NEUT_SEP_NODEP : " ");
  }

  fprintf (file, "  }\n");

  // texture_list
  fprintf (file, "  texture_list {\n");
  int *elt_text = ut_alloc_1d_int (Mesh.EltQty + 1);

  if (strcmp (coltype, "elt") == 0)
  {
    fprintf (file, "    %d,\n", showeltqty);

    int text_qty = 0;
    for (i = 1; i <= Mesh.EltQty; i++)
      if (!showelt || showelt[i])
      {
        elt_text[i] = text_qty++;
        if (!trs || trs[i] == 0)
          fprintf (file,
                   "    texture{pigment{rgb<" REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT ">} finish {ambient %f diffuse %f reflection %f}}\n",
                   rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255.,
                   Print.lightambient, Print.lightdiffuse, Print.lightreflection);
        else
          fprintf (file,
                   "    texture{pigment{rgbt<" REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT ">} finish {ambient %f diffuse %f reflection %f}}\n",
                   rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255., trs[i],
                   Print.lightambient, Print.lightdiffuse, Print.lightreflection);
      }
  }
  else if (strcmp (coltype, "node") == 0)
  {
    fprintf (file, "    %d,\n", Nodes.NodeQty);

    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      if (!trs || trs[i] == 0)
        fprintf (file,
                 "    texture{pigment{rgb<" REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT  "," REAL_PRINT_FORMAT  ">} finish {ambient %f diffuse %f reflection %f}}\n",
                 rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255.,
                 Print.lightambient, Print.lightdiffuse, Print.lightreflection);
      else
        fprintf (file,
                 "    texture{pigment{rgbt<" REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT "," REAL_PRINT_FORMAT  "," REAL_PRINT_FORMAT  ">} finish {ambient %f diffuse %f reflection %f}}\n",
                 rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255., trs[i],
                 Print.lightambient, Print.lightdiffuse, Print.lightreflection);
    }
  }
  else
    ut_print_neperbug ();

  fprintf (file, "  }\n");

  // face_indices
  fprintf (file, "  face_indices {\n");
  if (strcmp (Mesh.EltType, "tri") == 0)
    fprintf (file, "    %d,\n", showeltqty);
  else if (strcmp (Mesh.EltType, "quad") == 0)
    fprintf (file, "    %d,\n", showeltqty * 2);
  else
    ut_print_neperbug ();

  if (!strcmp (coltype, "elt"))
  {
    if (strcmp (Mesh.EltType, "tri") == 0)
    {
      for (i = 1; i <= Mesh.EltQty; i++)
        if (!showelt || showelt[i])
          fprintf (file, "    <%d,%d,%d>,%d%c\n", Mesh.EltNodes[i][0] - 1,
                   Mesh.EltNodes[i][1] - 1, Mesh.EltNodes[i][2] - 1,
                   elt_text[i], (i < Mesh.EltQty) ? ',' : ' ');
    }
    else if (strcmp (Mesh.EltType, "quad") == 0)
      for (i = 1; i <= Mesh.EltQty; i++)
        if (!showelt || showelt[i])
        {
          fprintf (file, "    <%d,%d,%d>,%d%c\n", Mesh.EltNodes[i][0] - 1,
                   Mesh.EltNodes[i][1] - 1, Mesh.EltNodes[i][2] - 1,
                   elt_text[i], (i < Mesh.EltQty) ? ',' : ',');
          fprintf (file, "    <%d,%d,%d>,%d%c\n", Mesh.EltNodes[i][0] - 1,
                   Mesh.EltNodes[i][2] - 1, Mesh.EltNodes[i][3] - 1,
                   elt_text[i], (i < Mesh.EltQty) ? ',' : ' ');
        }
  }
  else if (!strcmp (coltype, "node"))
  {
    if (strcmp (Mesh.EltType, "tri") == 0)
    {
      for (i = 1; i <= Mesh.EltQty; i++)
        if (!showelt || showelt[i])
          fprintf (file, "    <%d,%d,%d>,%d,%d,%d%c\n",
                   Mesh.EltNodes[i][0] - 1, Mesh.EltNodes[i][1] - 1,
                   Mesh.EltNodes[i][2] - 1, Mesh.EltNodes[i][0] - 1,
                   Mesh.EltNodes[i][1] - 1, Mesh.EltNodes[i][2] - 1,
                   (i < Mesh.EltQty) ? ',' : ' ');
    }
    else if (strcmp (Mesh.EltType, "quad") == 0)
    {
      for (i = 1; i <= Mesh.EltQty; i++)
        if (!showelt || showelt[i])
        {
          fprintf (file, "    <%d,%d,%d>,%d,%d,%d%c\n",
                   Mesh.EltNodes[i][0] - 1, Mesh.EltNodes[i][1] - 1,
                   Mesh.EltNodes[i][2] - 1, Mesh.EltNodes[i][0] - 1,
                   Mesh.EltNodes[i][1] - 1, Mesh.EltNodes[i][2] - 1,
                   (i < Mesh.EltQty) ? ',' : ' ');
          fprintf (file, "    <%d,%d,%d>,%d,%d,%d%c\n",
                   Mesh.EltNodes[i][0] - 1, Mesh.EltNodes[i][2] - 1,
                   Mesh.EltNodes[i][3] - 1, Mesh.EltNodes[i][0] - 1,
                   Mesh.EltNodes[i][2] - 1, Mesh.EltNodes[i][3] - 1,
                   (i < Mesh.EltQty) ? ',' : ' ');
        }
    }
  }
  else
    ut_print_neperbug ();

  fprintf (file, "  }\n");
  fprintf (file, "}\n");
  ut_free_1d_int (&elt_text);

  return;
}

void
nev_print_png_mesh1d (FILE *file, struct NODES Nodes, struct MESH Mesh, int *showelt,
                      int **Col, double *Rad, struct PRINT Print)
{
  int i, texture_unique;
  int *ColU = ut_alloc_1d_int (3);
  char *texture = ut_alloc_1d_char (100);

  texture_unique = 1;
  if (!Col)
  {}

  else
    for (i = 1; i <= Mesh.EltQty; i++)
      if (!showelt || showelt[i])
      {
        if (!ColU)
        {
          ColU = ut_alloc_1d_int (3);
          ut_array_1d_int_memcpy (Col[i], 3, ColU);
        }
        else if (!ut_array_1d_int_equal (Col[i], 3, ColU, 3))
        {
          texture_unique = 0;
          break;
        }
      }

  if (texture_unique)
  {
    fprintf (file,
             "#declare elt1d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f diffuse %f reflection %f} }\n",
             ColU[0] / 255., ColU[1] / 255., ColU[2] / 255., Print.lightambient, Print.lightdiffuse, Print.lightreflection);

    strcpy (texture, "elt1d");
  }

  for (i = 1; i <= Mesh.EltQty; i++)
    if (!showelt || showelt[i])
    {
      if (!texture_unique)
      {
        fprintf (file,
                 "#declare elt1d%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f diffuse %f reflection %f} }\n",
                 i, Col[i][0] / 255., Col[i][1] / 255., Col[i][2] / 255.,
                 Print.lightambient, Print.lightdiffuse, Print.lightreflection);

        sprintf (texture, "elt1d%d", i);
      }

      nev_print_png_segment_wsph (file, Nodes.NodeCoo[Mesh.EltNodes[i][0]],
                              Nodes.NodeCoo[Mesh.EltNodes[i][1]], Rad[i],
                              texture);
    }

  return;
}

void
nev_print_png_triangle (FILE * file, double *coo1, double *coo2, double *coo3,
                    char *texture, char *edge_rad, char *edge_texture)
{
  fprintf (file, "triangle {<%18.15g,%18.15g,%18.15g>,\n", coo1[0], coo1[1],
           coo1[2]);
  fprintf (file, "          <%18.15g,%18.15g,%18.15g>,\n", coo2[0], coo2[1],
           coo2[2]);
  fprintf (file, "          <%18.15g,%18.15g,%18.15g>\n", coo3[0], coo3[1],
           coo3[2]);

  fprintf (file, "            texture { %s }\n", texture);
  fprintf (file, "  }\n");

  nev_print_png_triangle_edge (file, coo1, coo2, coo3, edge_rad, edge_texture);

  return;
}

void
nev_print_png_tet_edge (FILE * file, double *coo1, double *coo2, double *coo3,
                    double *coo4, char *edge_rad, char *edge_texture)
{
  double rad;

  if (!edge_rad || (sscanf (edge_rad, "%lf", &rad) == 1 && rad <= 0))
    return;

  fprintf (file, "  union {\n");

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo1[0],
           coo1[1], coo1[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo2[0],
           coo2[1], coo2[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo2[0],
           coo2[1], coo2[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo3[0],
           coo3[1], coo3[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo3[0],
           coo3[1], coo3[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo1[0],
           coo1[1], coo1[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo4[0],
           coo4[1], coo4[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo1[0],
           coo1[1], coo1[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo4[0],
           coo4[1], coo4[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo2[0],
           coo2[1], coo2[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo4[0],
           coo4[1], coo4[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo3[0],
           coo3[1], coo3[2]);
  fprintf (file, "              %s }\n", edge_rad);

  if (edge_texture)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_png_polygon_edge (FILE * file, int qty, double **coo, double rad,
                        char *edge_texture)
{
  int i;

  if (rad <= 0)
    return;

  fprintf (file, "  union {\n");

  for (i = 0; i < qty - 1; i++)
  {
    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n", coo[i][0],
             coo[i][1], coo[i][2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo[i + 1][0], coo[i + 1][1], coo[i + 1][2]);
    fprintf (file, "              %18.15g}\n", rad);
  }
  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
           coo[qty - 1][0], coo[qty - 1][1], coo[qty - 1][2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n", coo[0][0],
           coo[0][1], coo[0][2]);
  fprintf (file, "              %18.15g}\n", rad);

  if (edge_texture)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_png_polygon (FILE * file, double *eq, int qty, double **coo,
                   char *texture, double edge_rad, char *edge_texture, int pt,
                   double *p, double iedge_rad, char *iedge_texture)
{
  int i, triqty, **tripos = NULL;
  double *c = ut_alloc_1d (3);

  if (pt < 0 && p == NULL)
  {
    for (i = 0; i < qty; i++)
      ut_array_1d_add (c, coo[i], 3, c);
    ut_array_1d_scale (c, 3, 1. / qty);
  }
  else if (pt >= 0)
    ut_array_1d_memcpy (coo[pt], 3, c);
  else
    ut_array_1d_memcpy (p, 3, c);

  ut_space_polygon_triangles (eq, coo, qty, &tripos, &triqty);

  // drawing face
  for (i = 0; i < triqty; i++)
    nev_print_png_triangle (file, coo[tripos[i][0]], coo[tripos[i][1]],
                        coo[tripos[i][2]], texture, NULL, NULL);

  nev_print_png_polygon_edge (file, qty, coo, edge_rad, edge_texture);

  // drawing segment face interpolation
  if (iedge_rad)
    for (i = 0; i < qty; i++)
      nev_print_png_segment (file, c, coo[i], iedge_rad, iedge_texture);

  ut_free_1d (&c);
  ut_free_2d_int (&tripos, triqty);

  return;
}

void
nev_print_png_tet (FILE * file, double *coo1, double *coo2, double *coo3,
               double *coo4, char *texture, char *edge_rad,
               char *edge_texture)
{
  nev_print_png_triangle (file, coo1, coo2, coo3, texture, NULL, NULL);
  nev_print_png_triangle (file, coo1, coo3, coo4, texture, NULL, NULL);
  nev_print_png_triangle (file, coo1, coo4, coo2, texture, NULL, NULL);
  nev_print_png_triangle (file, coo2, coo3, coo4, texture, NULL, NULL);

  nev_print_png_tet_edge (file, coo1, coo2, coo3, coo4, edge_rad, edge_texture);

  return;
}

void
nev_print_png_arr (FILE * file, double *coo, double *data, char *texture)
{
  int i;
  double *n = ut_alloc_1d (3);

  ut_array_1d_memcpy (data + 2, 3, n);
  ut_array_1d_normalize (n, 3);

  double tail_rad = data[0];
  double tip_rad = data[0] * 3;
  double total_length = data[1];
  double tip_length = 2 * tip_rad;
  double tail_length = total_length - tip_length;
  double *start = ut_alloc_1d (3);
  double *end = ut_alloc_1d (3);

  if (data[0] > 0 && data[1] > 0)
  {
    ut_array_1d_memcpy (coo, 3, start);
    for (i = 0; i < 3; i++)
      end[i] = coo[i] + tail_length * n[i];

    nev_print_png_segment (file, start, end, tail_rad, texture);

    ut_array_1d_memcpy (end, 3, start);
    for (i = 0; i < 3; i++)
      end[i] = coo[i] + total_length * n[i];

    nev_print_png_cone (file, start, end, tip_rad, texture);
  }

  ut_free_1d (&n);

  return;
}
