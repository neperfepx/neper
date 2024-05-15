/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_scale_.h"

void
nev_print_png_scale (struct IN_V In, FILE * file, char *ColScheme, char *scale, char *scaletitle)
{
  int i, tickqty;
  double min, max, z, *tickvals = NULL;
  char **ticks = NULL;
  char *tickformat = NULL;

  nev_print_scale_ticks (scale, &min, &max, &ticks, &tickvals, &tickqty, &tickformat);

  int binqty = 1000;
  double **data = ut_alloc_2d (2 * (binqty + 1) + 1, 1);
  double *coo = ut_alloc_1d (3);
  struct NODES N;
  struct MESH M;
  int *eltnodes = ut_alloc_1d_int (3);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  M.Dimension = 2;
  ut_string_string ("tri", &(M.EltType));
  M.EltOrder = 1;

  // adding bottom nodes
  coo[2] = 0;
  coo[0] = 0;
  neut_nodes_addnode (&N, coo, 0);
  data[N.NodeQty][0] = min;

  coo[0] = -0.1;
  neut_nodes_addnode (&N, coo, 0);
  data[N.NodeQty][0] = min;

  // adding other nodes
  for (i = 1; i <= binqty; i++)
  {
    coo[2] = (double) i / (double) binqty;

    coo[0] = 0;
    neut_nodes_addnode (&N, coo, 0);
    data[N.NodeQty][0] = min + (max - min) * i / (double) binqty;

    coo[0] = -0.1;
    neut_nodes_addnode (&N, coo, 0);
    data[N.NodeQty][0] = data[N.NodeQty - 1][0];

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

  neut_nodes_scale (&N, 1, 1, 1);

  struct PRINT Print;
  neut_print_set_default (&Print);
  Print.cameralookat[0] = 0.05;
  Print.cameralookat[1] = 0;
  Print.cameralookat[2] = 0.5;
  Print.cameracoo[0] = 0;
  Print.cameracoo[1] = -5;
  Print.cameracoo[2] = 0.5;
  Print.cameraangle = 6;
  Print.lightambient = 0.6;
  Print.lightdiffuse = 0.6;
  Print.lightsourceqty = 1;
  Print.lightsources = ut_alloc_1d_pchar (1);
  ut_string_string ("point(coo=0:-5:0.5)", Print.lightsources);
  sprintf (Print.cameraprojection, "orthographic");

  int **rgb = ut_alloc_2d_int (N.NodeQty + 1, 3);

  neut_data_real_color (data, NULL, N.NodeQty, scale, ColScheme, rgb, NULL);

  nev_print_png_header (In, Print, file);
  nev_print_png_mesh2d (file, N, M, NULL, rgb, NULL, (char *) "elt", Print);
  nev_print_png_foot (In, file);

  fprintf (file, "union {\n");
  fprintf (file,
           "cylinder {<-0.1,0,0>,<-0.1,0,1>,0.0025 texture {pigment{rgb<0,0,0>}}}\n");
  fprintf (file,
           "cylinder {<0,0,0>,<0,0,1>,0.0025 texture {pigment{rgb<0,0,0>}}}\n");
  fprintf (file,
           "cylinder {<-0.1,0,0>,<0,0,0>,0.0025 texture {pigment{rgb<0,0,0>}}}\n");
  fprintf (file,
           "cylinder {<-0.1,0,1>,<0,0,1>,0.0025 texture {pigment{rgb<0,0,0>}}}\n");

  fprintf (file, "union {\n");

  for (i = 0; i < tickqty; i++)
  {
    z = (tickvals[i] - min) / (max - min);

    fprintf (file,
             "cylinder {<-0.1,0,%f>,<0,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n",
             z, z);

    fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0.1, 0\n", ticks[i]);
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
      fprintf (file, "rotate <0,-90,0>\n");
      fprintf (file, "translate <0.08,0,0.04>\n");
    }

    fprintf (file, "translate <0,0,0.46>\n");
    fprintf (file, "translate <0.17,0,0>\n");
    fprintf (file, "}\n");
    fprintf (file, "#declare Min = min_extent (ScaleTitle);\n");
    fprintf (file, "#declare Max = max_extent (ScaleTitle);\n");
    fprintf (file, "object {ScaleTitle translate + (Min - Max) / 2}\n");
  }

  ut_free_1d (&tickvals);
  ut_free_2d_char (&ticks, tickqty);

  fprintf (file, "}\n");

  fprintf (file, "rotate<-90, 0, 0>\nscale<1,1,-1>}\n");

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  ut_free_1d (&coo);
  ut_free_1d_int (&eltnodes);
  ut_free_2d_char (&ticks, tickqty);
  ut_free_1d_char (&tickformat);

  return;
}
