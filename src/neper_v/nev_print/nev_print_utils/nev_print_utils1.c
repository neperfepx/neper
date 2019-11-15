/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_utils_.h"

void
nev_print_segment (FILE * file, double *coo1,
		   double *coo2, char *edge_rad, char *edge_texture)
{
  if (!nev_print_cylinder_test (coo1, coo2, edge_rad))
    return;

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              %s\n", edge_rad);

  if (edge_texture != NULL)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_cone (FILE * file, double *coo1,
		double *coo2, char *edge_rad, char *edge_texture)
{
  if (!nev_print_cylinder_test (coo1, coo2, edge_rad))
    return;

  fprintf (file, "    cone {<%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              %s,\n", edge_rad);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              0\n");

  if (edge_texture != NULL)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_segment_wsph (FILE * file, double *coo1,
			double *coo2, char *edge_rad, char *edge_texture)
{
  if (!nev_print_cylinder_test (coo1, coo2, edge_rad))
    return;

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              %s\n", edge_rad);

  if (edge_texture != NULL)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              %s\n", edge_rad);

  if (edge_texture != NULL)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              %s\n", edge_rad);

  if (edge_texture != NULL)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_sphere (FILE * file, double *coo, char *sphere_rad, char *texture)
{
  double rad;

  if (!sphere_rad || (sscanf (sphere_rad, "%lf", &rad) == 1 && rad <= 0))
    return;

  fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n",
	   coo[0], coo[1], coo[2]);
  fprintf (file, "              %s\n", sphere_rad);

  if (texture != NULL)
    fprintf (file, "    texture { %s }\n", texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_ellipsoid (FILE * file, double *coo, double *rad, char *texture)
{
  fprintf (file,
	   "    sphere {<0.,0.,0.>, 1. scale <%18.15g,%18.15g,%18.15g> matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,0.,0.,0.> translate <%18.15g,%18.15g,%18.15g>\n",
	   rad[0], rad[1], rad[2], rad[3], rad[4], rad[5], rad[6], rad[7],
	   rad[8], rad[9], rad[10], rad[11], coo[0], coo[1], coo[2]);

  if (texture != NULL)
    fprintf (file, "    texture { %s }\n", texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_cube (FILE * file, double *coo, double *rad, char *texture)
{
  fprintf (file,
	   "    box {<-.5,-.5,-.5>, <.5,.5,.5> scale %18.15g matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,0.,0.,0.> translate <%18.15g,%18.15g,%18.15g>\n",
	   2 * rad[0], rad[1], rad[2], rad[3], rad[4], rad[5], rad[6], rad[7],
	   rad[8], rad[9], coo[0], coo[1], coo[2]);

  if (texture != NULL)
    fprintf (file, "    texture { %s }\n", texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_cyl (FILE * file, double *coo, double *rad, char *texture)
{
  double *v = ut_alloc_1d (3);

  ut_array_1d_memcpy (v, 3, rad + 2);
  ut_array_1d_scale (v, 3, 1. / ut_array_1d_norm (v, 3));
  ut_array_1d_scale (v, 3, rad[1] * .5);

  if (rad[0] > 0 && rad[1] > 0)
  {
    fprintf (file,
	     "    cylinder {<%18.15g,%18.15g,%18.15g>, <%18.15g,%18.15g,%18.15g>, %18.15g translate <%18.15g,%18.15g,%18.15g>\n",
	     -v[0], -v[1], -v[2], v[0], v[1], v[2], rad[0], coo[0], coo[1],
	     coo[2]);

    if (texture != NULL)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");
  }

  ut_free_1d (v);

  return;
}

void
nev_print_arr (FILE * file, double *coo, double *rad, char *texture)
{
  double *n = ut_alloc_1d (3);
  double *v = ut_alloc_1d (3);
  double fact = 3;
  double fact2 = 2 * fact;

  ut_array_1d_memcpy (v, 3, rad + 2);
  ut_array_1d_scale (v, 3, 1. / ut_array_1d_norm (v, 3));
  ut_array_1d_memcpy (n, 3, v);
  ut_array_1d_scale (v, 3, rad[1] * .5);

  if (rad[0] > 0 && rad[1] > 0)
  {
    fprintf (file,
	     "    cylinder {<%18.15g,%18.15g,%18.15g>, <%18.15g,%18.15g,%18.15g>, %18.15g translate <%18.15g,%18.15g,%18.15g>\n",
	     -v[0], -v[1], -v[2],
             v[0] - fact2 * rad[0] * n[0],
             v[1] - fact2 * rad[0] * n[1],
             v[2] - fact2 * rad[0] * n[2],
             rad[0], coo[0] + v[0], coo[1] + v[1],
	     coo[2] + v[2]);

    if (texture != NULL)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");

    fprintf (file,
        "    cone {<0.,0.,0.>, %18.15g <%18.15g,%18.15g,%18.15g>, 0. translate <%18.15g,%18.15g,%18.15g>\n",
	     fact * rad[0],
             fact2 * rad[0] * n[0], fact2 * rad[0] * n[1], fact2 * rad[0] * n[2],
             coo[0] + 2 * v[0] - fact2 * rad[0] * n[0],
             coo[1] + 2 * v[1] - fact2 * rad[0] * n[1],
             coo[2] + 2 * v[2] - fact2 * rad[0] * n[2]);

    if (texture != NULL)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");
  }

  ut_free_1d (v);
  ut_free_1d (n);

  return;
}

void
nev_print_tor (FILE * file, double *coo, double *rad, char *texture)
{
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);
  double **g = ol_g_alloc ();

  ut_array_1d_set_3 (v1, 0, 1, 0);
  ut_array_1d_memcpy (v2, 3, rad + 2);
  ol_vect_vect_g (v1, v2, g);

  if (rad[0] > 0 && rad[1] > 0)
  {
    fprintf (file,
	     "    torus {%18.15g,%18.15g matrix <%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g,%18.15g>\n",
	     rad[0], rad[1],
	     g[0][0], g[0][1], g[0][2],
	     g[1][0], g[1][1], g[1][2],
	     g[2][0], g[2][1], g[2][2],
	     coo[0], coo[1], coo[2]);

    if (texture != NULL)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");
  }

  ut_free_1d (v1);
  ut_free_1d (v2);
  ol_g_free (g);

  return;
}

void
nev_print_disc (FILE * file, double *coo, double *rad, char *texture)
{
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);
  double **g = ol_g_alloc ();

  ut_array_1d_set_3 (v1, 0, 1, 0);
  ut_array_1d_memcpy (v2, 3, rad + 2);
  ol_vect_vect_g (v1, v2, g);

  if (rad[0] > 0)
  {
    fprintf (file,
	     "    disc {<%18.15g,%18.15g,%18.15g>,  <%18.15g,%18.15g,%18.15g>, %18.15g\n",
	     coo[0], coo[1], coo[2],
	     rad[1], rad[2], rad[3],
	     rad[0]);

    if (texture != NULL)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");
  }

  ut_free_1d (v1);
  ut_free_1d (v2);
  ol_g_free (g);

  return;
}

void
nev_print_triangle_edge (FILE * file, double *coo1, double *coo2,
			 double *coo3, char *edge_rad, char *edge_texture)
{
  double rad;

  if (!edge_rad || (sscanf (edge_rad, "%lf", &rad) == 1 && rad <= 0))
    return;

  fprintf (file, "  union {\n");

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo3[0], coo3[1], coo3[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo3[0], coo3[1], coo3[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              %s }\n", edge_rad);

  if (edge_texture != NULL)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_mesh2d (FILE * file, struct NODES Nodes, struct MESH Mesh,
		  int *showelt, int **rgb, char *coltype, int showshadow)
{
  int i, showeltqty;

  if (Nodes.NodeQty == 0 || Mesh.EltQty == 0)
    return;

  showeltqty = showelt ? ut_array_1d_int_sum (showelt + 1, Mesh.EltQty)
    : Mesh.EltQty;

  if (showeltqty == 0)
    return;

  fprintf (file, "mesh2 {\n");

  // vertex_vectors
  fprintf (file, "  vertex_vectors {\n");
  fprintf (file, "    %d,\n", Nodes.NodeQty);

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "    <%.12f,%.12f,%.12f>%s\n", Nodes.NodeCoo[i][0],
	     Nodes.NodeCoo[i][1], Nodes.NodeCoo[i][2],
	     (i < Nodes.NodeQty) ? NEUT_SEP_NODEP : " ");
  }

  fprintf (file, "  }\n");

  // texture_list
  fprintf (file, "  texture_list {\n");
  double ambient = showshadow ? 0.6 : 1;
  int *elt_text = ut_alloc_1d_int (Mesh.EltQty + 1);

  if (strcmp (coltype, "elt") == 0)
  {
    fprintf (file, "    %d,\n", showeltqty);

    int text_qty = 0;
    for (i = 1; i <= Mesh.EltQty; i++)
      if (!showelt || showelt[i])
      {
	elt_text[i] = text_qty++;
	fprintf (file,
		 "    texture{pigment{rgb<%.12f,%.12f,%.12f>} finish {ambient %.1f}}\n",
		 rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255.,
		 ambient);
      }
  }
  else if (strcmp (coltype, "node") == 0)
  {
    fprintf (file, "    %d,\n", Nodes.NodeQty);

    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      fprintf (file,
	       "    texture{pigment{rgb<%.12f,%.12f,%.12f>} finish {ambient %.1f}}\n",
	       rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255., ambient);
    }
  }
  else
    ut_error_reportbug ();

  fprintf (file, "  }\n");

  // face_indices
  fprintf (file, "  face_indices {\n");
  if (strcmp (Mesh.EltType, "tri") == 0)
    fprintf (file, "    %d,\n", showeltqty);
  else if (strcmp (Mesh.EltType, "quad") == 0)
    fprintf (file, "    %d,\n", showeltqty * 2);
  else
    ut_error_reportbug ();

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
    ut_error_reportbug ();

  fprintf (file, "  }\n");
  fprintf (file, "}\n");
  ut_free_1d_int (elt_text);

  return;
}

void
nev_print_triangle (FILE * file, double *coo1, double *coo2, double *coo3,
		    char *texture, char *edge_rad, char *edge_texture)
{
  fprintf (file, "triangle {<%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "          <%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "          <%18.15g,%18.15g,%18.15g>\n",
	   coo3[0], coo3[1], coo3[2]);

  fprintf (file, "            texture { %s }\n", texture);
  fprintf (file, "  }\n");

  nev_print_triangle_edge (file, coo1, coo2, coo3, edge_rad, edge_texture);

  return;
}

void
nev_print_tet_edge (FILE * file, double *coo1, double *coo2, double *coo3,
		    double *coo4, char *edge_rad, char *edge_texture)
{
  double rad;

  if (!edge_rad || (sscanf (edge_rad, "%lf", &rad) == 1 && rad <= 0))
    return;

  fprintf (file, "  union {\n");

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo3[0], coo3[1], coo3[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo3[0], coo3[1], coo3[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo4[0], coo4[1], coo4[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo4[0], coo4[1], coo4[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "              %s }\n", edge_rad);

  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo4[0], coo4[1], coo4[2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo3[0], coo3[1], coo3[2]);
  fprintf (file, "              %s }\n", edge_rad);

  if (edge_texture != NULL)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_polygon_edge (FILE * file, int qty, double **coo,
			char *edge_rad, char *edge_texture)
{
  int i;
  double rad;

  if (!edge_rad || (sscanf (edge_rad, "%lf", &rad) == 1 && rad <= 0))
    return;

  fprintf (file, "  union {\n");

  for (i = 0; i < qty - 1; i++)
  {
    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo[i][0], coo[i][1], coo[i][2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	     coo[i + 1][0], coo[i + 1][1], coo[i + 1][2]);
    fprintf (file, "              %s }\n", edge_rad);
  }
  fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	   coo[qty - 1][0], coo[qty - 1][1], coo[qty - 1][2]);
  fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	   coo[0][0], coo[0][1], coo[0][2]);
  fprintf (file, "              %s }\n", edge_rad);

  if (edge_texture != NULL)
    fprintf (file, "    texture { %s }\n", edge_texture);
  fprintf (file, "  }\n");

  return;
}

void
nev_print_polygon (FILE * file, double *eq, int qty, double **coo,
		   char *texture, char *edge_rad, char *edge_texture,
		   int pt, double *p, char *iedge_rad, char *iedge_texture)
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
    ut_array_1d_memcpy (c, 3, coo[pt]);
  else
    ut_array_1d_memcpy (c, 3, p);

  ut_space_polygon_triangles (eq, coo, qty, &tripos, &triqty);

  // drawing face
  for (i = 0; i < triqty; i++)
    nev_print_triangle (file, coo[tripos[i][0]], coo[tripos[i][1]], coo[tripos[i][2]],
                        texture, NULL, NULL);

  nev_print_polygon_edge (file, qty, coo, edge_rad, edge_texture);

  // drawing segment face interpolation
  if (iedge_rad != NULL)
    for (i = 0; i < qty; i++)
      nev_print_segment (file, c, coo[i], iedge_rad, iedge_texture);

  ut_free_1d (c);
  ut_free_2d_int (tripos, triqty);

  return;
}

void
nev_print_tet (FILE * file, double *coo1, double *coo2, double *coo3,
	       double *coo4, char *texture, char *edge_rad,
	       char *edge_texture)
{
  nev_print_triangle (file, coo1, coo2, coo3, texture, NULL, NULL);
  nev_print_triangle (file, coo1, coo3, coo4, texture, NULL, NULL);
  nev_print_triangle (file, coo1, coo4, coo2, texture, NULL, NULL);
  nev_print_triangle (file, coo2, coo3, coo4, texture, NULL, NULL);

  nev_print_tet_edge (file, coo1, coo2, coo3, coo4, edge_rad, edge_texture);

  return;
}

void
nev_print_scale (FILE * file, char *ColScheme, char *scale, char *scaletitle)
{
  int i;
  struct NODES N;
  struct MESH M;
  double *coo = ut_alloc_1d (3);
  int binqty = 100;
  int *eltnodes = ut_alloc_1d_int (3);
  char *minexpr = NULL, *maxexpr = NULL, *ticks = NULL;
  char **parts = NULL;
  int qty;

  double height = 1;

  double *nodescal = ut_alloc_1d (2 * (binqty + 1) + 1);

  double min, max;

  ut_string_separate (scale, NEUT_SEP_DEP, &parts, &qty);
  if (qty < 2)
    ut_error_reportbug ();

  ut_string_string (parts[0], &minexpr);
  ut_string_string (parts[qty - 1], &maxexpr);

  if (qty > 2)
  {
    ticks = ut_alloc_1d_char (strlen (scale) + 1);
    strcpy (ticks, scale);
  }

  ut_string_real (minexpr, &min);
  ut_string_real (maxexpr, &max);

  // In case min and max are equal, putting them apart a little
  if (min == max)
  {
    if (min == 0)
    {
      min = -1e-12;
      max = 1e-12;
    }
    else
    {
      min *= (1 - 1e-12);
      max *= (1 + 1e-12);
    }
  }

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  M.Dimension = 2;
  M.EltType = ut_alloc_1d_char (4);
  strcpy (M.EltType, "tri");
  M.EltOrder = 1;

  // adding bottom nodes
  coo[2] = 0;
  coo[0] = 0;
  neut_nodes_addnode (&N, coo, 0);
  nodescal[N.NodeQty] = min;

  coo[0] = -0.1;
  neut_nodes_addnode (&N, coo, 0);
  nodescal[N.NodeQty] = min;

  // adding other nodes
  for (i = 1; i <= binqty; i++)
  {
    coo[2] = height * i / (double) binqty;

    coo[0] = 0;
    neut_nodes_addnode (&N, coo, 0);
    nodescal[N.NodeQty] = min + (max - min) * i / (double) binqty;

    coo[0] = -0.1;
    neut_nodes_addnode (&N, coo, 0);
    nodescal[N.NodeQty] = nodescal[N.NodeQty - 1];

    eltnodes[0] = N.NodeQty - 1;
    eltnodes[1] = N.NodeQty - 2;
    eltnodes[2] = N.NodeQty - 3;

    neut_mesh_addelt (&M, eltnodes);

    eltnodes[0] = N.NodeQty;
    eltnodes[1] = N.NodeQty - 2;
    eltnodes[2] = N.NodeQty - 1;

    neut_mesh_addelt (&M, eltnodes);
  }

  neut_mesh_addelset (&M, NULL, M.EltQty);

  neut_nodes_scale (&N, height, height, height);

  struct PRINT Print;
  neut_print_set_default (&Print);
  Print.cameralookat[0] = 0.05;
  Print.cameralookat[1] = 0;
  Print.cameralookat[2] = height / 2;
  Print.cameracoo[0] = 0;
  Print.cameracoo[1] = -5;
  Print.cameracoo[2] = height / 2;
  Print.cameraangle = 6;
  sprintf (Print.cameraprojection, "orthographic");

  Print.showshadow = 1;

  int **rgb = ut_alloc_2d_int (N.NodeQty + 1, 3);
  if (ColScheme != NULL)
    for (i = 1; i <= N.NodeQty; i++)
      ut_color_scheme_val_color (ColScheme, min, max, nodescal[i], rgb[i]);
  else
    for (i = 1; i <= N.NodeQty; i++)
      ut_color_scheme_val_color ("blue,cyan,yellow,red", min, max,
				 nodescal[i], rgb[i]);

  nev_print_header (file, Print);
  nev_print_mesh2d (file, N, M, NULL, rgb, "elt", Print.showshadow);
  nev_print_foot (file, Print);

  fprintf (file, "union {\n");
  fprintf (file,
	   "cylinder {<-0.1,0,0>,<-0.1,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n",
	   height);
  fprintf (file,
	   "cylinder {<0,0,0>,<0,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n",
	   height);
  fprintf (file,
	   "cylinder {<-0.1,0,0>,<0,0,0>,0.0025 texture {pigment{rgb<0,0,0>}}}\n");
  fprintf (file,
	   "cylinder {<-0.1,0,%f>,<0,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n",
	   height, height);

  int valqty;
  double z;
  double *val = NULL;
  char **valstring = NULL;
  if (ticks == NULL)
  {
    valqty = 11;
    val = ut_alloc_1d (valqty);
    valstring = ut_alloc_2d_char (valqty, 11);
    for (i = 0; i < valqty; i++)
    {
      val[i] = min + ((double) i / (valqty - 1)) * (max - min);
      sprintf (valstring[i], "%g", val[i]);
    }
  }
  else
  {
    ut_string_separate (ticks, NEUT_SEP_DEP, &valstring, &valqty);
    val = ut_alloc_1d (valqty);
    for (i = 0; i < valqty; i++)
    {
      ut_string_fnr (valstring[i], '_', ' ');
      ut_string_real (valstring[i], &(val[i]));
    }
  }

  fprintf (file, "union {\n");

  for (i = 0; i < valqty; i++)
  {
    z = height * (val[i] - min) / (max - min);

    fprintf (file,
	     "cylinder {<-0.1,0,%f>,<0,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n",
	     z, z);

    fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0.1, 0\n", valstring[i]);
    fprintf (file, "pigment {rgb<0,0,0>}\n");
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    fprintf (file, "translate <0,0,%f>\n", z);
    fprintf (file, "translate <0.02,0,-0.02>\n");
    fprintf (file, "}\n");
  }

  if (scaletitle)
  {
    fprintf (file, "#declare ScaleTitle =\n");
    fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0.1, 0\n", scaletitle);
    fprintf (file, "pigment {rgb<0,0,0>}\n");
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    if (strlen (scaletitle) > 3)
    {
      fprintf (file, "rotate <0,90,0>\n");
      fprintf (file, "translate <0,0,0.04>\n");
    }

    fprintf (file, "translate <0,0,0.46>\n");
    fprintf (file, "translate <0.17,0,0>\n");
    fprintf (file, "}\n");
    fprintf (file, "#declare Min = min_extent (ScaleTitle);\n");
    fprintf (file, "#declare Max = max_extent (ScaleTitle);\n");
    fprintf (file, "object {ScaleTitle translate - (Min - Max) / 2}\n");
  }

  ut_free_1d (val);
  ut_free_2d_char (valstring, valqty);

  fprintf (file, "}\n");

  fprintf (file, "rotate<-90, 0, 0>\nscale<1,1,-1>}\n");

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  ut_free_1d (coo);
  ut_free_1d_int (eltnodes);
  ut_free_1d (nodescal);

  return;
}

void
nev_print_arrow (FILE * file, double *O,
		 double *v, char *edge_rad, char *edge_texture)
{
  int i;
  double **coo = ut_alloc_2d (3, 3);
  char *tip_rad = ut_alloc_1d_char (strlen (edge_rad) + 6);
  double edge_rad_val;
  double *vnorm = ut_alloc_1d (3);

  ut_array_1d_memcpy (vnorm, 3, v);
  ut_array_1d_scale (vnorm, 3, 1. / ut_array_1d_norm (vnorm, 3));

  sscanf (edge_rad, "%lf", &edge_rad_val);

  sprintf (tip_rad, "3 * %s\n", edge_rad);

  for (i = 0; i < 3; i++)
  {
    coo[0][i] = O[i];
    coo[2][i] = O[i] + v[i];
  }

  for (i = 0; i < 3; i++)
    coo[1][i] = coo[2][i] - 6 * edge_rad_val * vnorm[i];

  nev_print_segment (file, coo[0], coo[1], edge_rad, edge_texture);
  nev_print_cone (file, coo[1], coo[2], tip_rad, edge_texture);
  nev_print_sphere (file, coo[0], edge_rad, edge_texture);

  ut_free_2d (coo, 3);
  ut_free_1d_char (tip_rad);
  ut_free_1d (vnorm);

  return;
}
