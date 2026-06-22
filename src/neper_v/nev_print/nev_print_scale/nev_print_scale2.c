/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_scale_.h"

void
nev_print_png_scale (struct IN_V In, FILE * file, struct PRINT Print, struct DATA Data)
{
  int i, tickqty;
  double min, max, z, *tickvals = NULL;
  char **ticks = NULL;
  char *tickformat = NULL;
  struct DATA Data2;

  neut_data_set_default (&Data2);

  nev_print_scale_ticks (Data.Scale, &min, &max, &ticks, &tickvals, &tickqty, &tickformat);

  int binqty = 1000;
  double **data = ut_alloc_2d (2 * (binqty + 1) + 1, 1);
  double *coo = ut_alloc_1d (3);
  struct NODES N;
  struct MESH M;
  int *eltnodes = ut_alloc_1d_int (3);
  char *tmp = ut_alloc_1d_char (1000);
  double *basecol = ut_alloc_1d (3);
  char *texture = ut_alloc_1d_char (1000);

  if (!strcmp (Print.colormode, "bright"))
    ut_string_string ("texture {pigment{rgb<0,0,0>}}", &texture);
  else if (!strcmp (Print.colormode, "dark"))
    sprintf (texture, "texture { pigment { rgbt <1,1,1,0.> } finish {ambient %f diffuse %f reflection %f} }",
            Print.lightambient, Print.lightdiffuse, Print.lightreflection);

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

  struct PRINT Print2;
  neut_print_set_default (&Print2);
  Print2.cameralookat[0] = 0.05;
  Print2.cameralookat[1] = 0;
  Print2.cameralookat[2] = 0.5;
  Print2.cameracoo[0] = 0;
  Print2.cameracoo[1] = -5;
  Print2.cameracoo[2] = 0.5;
  Print2.cameraangle = 6;
  Print2.lightambient = 0.6;
  Print2.lightdiffuse = 0.6;
  Print2.lightsourceqty = 1;
  Print2.lightsources = ut_alloc_1d_pchar (1);
  ut_string_string ("point(coo=0:-5:0.5)", Print2.lightsources);
  sprintf (Print2.cameraprojection, "orthographic");
  ut_string_string (Print.background, &(Print2.background));

  ut_string_string (Data.ColScheme, &(Data2.ColScheme));
  ut_string_string (Data.Scale, &(Data2.Scale));
  Data2.Qty = N.NodeQty;
  Data2.ColData = ut_alloc_2d (Data2.Qty + 1, 1);
  Data2.Col = ut_alloc_2d_int (Data2.Qty + 1, 3);

  for (i = 1; i <= Data2.Qty; i++)
    Data2.ColData[i][0] = data[i][0];

  neut_data_init_color_real (&Data2);

  neut_print_povheader (In.imageformat, Print2, file);

  nev_print_real_png_mesh2d (file, N, M, NULL, Data2.Col, NULL, (char *) "elt", Print2);

  neut_print_povfoot (NULL, In.imageformat, file);

  fprintf (file, "union {\n");
  fprintf (file,
           "cylinder {<-0.1,0,0>,<-0.1,0,1>,0.0025 %s}\n", texture);
  fprintf (file,
           "cylinder {<0,0,0>,<0,0,1>,0.0025 %s}\n", texture);
  fprintf (file,
           "cylinder {<-0.1,0,0>,<0,0,0>,0.0025 %s}\n", texture);
  fprintf (file,
           "cylinder {<-0.1,0,1>,<0,0,1>,0.0025 %s}\n", texture);

  fprintf (file, "union {\n");

  for (i = 0; i < tickqty; i++)
  {
    z = (tickvals[i] - min) / (max - min);

    fprintf (file, "cylinder {<-0.1,0,%f>,<0,0,%f>,0.0025 %s}\n", z, z, texture);

    fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0.1, 0\n", ticks[i]);
    fprintf (file, "%s\n", texture);
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    fprintf (file, "translate <0,0,%f>\n", z);
    fprintf (file, "translate <0.02,0,-0.02>\n");
    fprintf (file, "}\n");
  }

  if (Data.ScaleTitle)
  {
    fprintf (file, "#declare ScaleTitle =\n");
    fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0.1, 0\n", Data.ScaleTitle);
    fprintf (file, "%s\n", texture);
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    if (strlen (Data.ScaleTitle) > 3)
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

  nev_print_png_scale_minmax (file, Data, Print);

  fprintf (file, "}\n");

  fprintf (file, "rotate<-90, 0, 0>\nscale<1,1,-1>}\n");

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  ut_free_1d (&coo);
  ut_free_1d_int (&eltnodes);
  ut_free_2d_char (&ticks, tickqty);
  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tickformat);
  neut_data_free (&Data2);
  ut_free_1d (&tickvals);
  ut_free_1d (&basecol);
  ut_free_1d_char (&texture);

  return;
}
