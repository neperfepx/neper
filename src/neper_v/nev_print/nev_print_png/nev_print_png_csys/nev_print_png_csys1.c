/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_csys_.h"

void
nev_print_png_csys (FILE * file, struct DATA CsysData, struct PRINT Print)
{
  int i;
  double *O = ut_alloc_1d (3);
  double **g = ol_g_alloc ();
  double **s = ol_g_alloc ();
  char *edge_rad = ut_alloc_1d_char (10);
  char *edge_texture = ut_alloc_1d_char (100);
  char *tmp = NULL;

  fprintf (file, "#declare csys =\n");
  fprintf (file,
           "  texture { pigment { rgbt <%f,%f,%f,0.> } finish {ambient %f diffuse %f reflection %f} }",
           CsysData.Col[1][0] / 255., CsysData.Col[1][1] / 255.,
           CsysData.Col[1][2] / 255., Print.lightambient, Print.lightdiffuse, Print.lightreflection);

  sprintf (edge_rad, "%f", CsysData.Rad[1]);
  sprintf (edge_texture, "csys");

  ut_array_2d_scale (g, 3, 3, CsysData.Length[1]);
  ut_array_2d_scale (s, 3, 3, 0.1);

  ut_array_1d_memcpy (CsysData.Coo[1], 3, O);

  double *data = ut_alloc_1d (5);

  for (i = 0; i < 3; i++)
    if (strlen (CsysData.Label[i]) > 0)
    {
      data[0] = CsysData.Rad[1];
      data[1] = CsysData.Length[1];
      ut_array_1d_memcpy (g[i], 3, data + 2);

      nev_print_png_arr (file, O, data, edge_texture);

      fprintf (file, "#declare CamLoc = <%f,%f,%f>;\n", Print.cameracoo[0],
               Print.cameracoo[1], Print.cameracoo[2]);
      fprintf (file, "#declare CamSky = <%f,%f,%f>;\n", Print.camerasky[0],
               Print.camerasky[1], Print.camerasky[2]);
      fprintf (file, "#declare CamLA = <%f,%f,%f>;\n", Print.cameralookat[0],
               Print.cameralookat[1], Print.cameralookat[2]);

      fprintf (file, "#declare TextLowerLeft=<%f, %f, %f>;\n", O[0] + g[i][0],
               O[1] + g[i][1], O[2] + g[i][2]);
      if (!strcmp (Print.cameraprojection, "perspective"))
        fprintf (file,
                 "#declare TransVec1=vnormalize(TextLowerLeft-CamLoc);\n");
      else
        fprintf (file, "#declare TransVec1=vnormalize(CamLA-CamLoc);\n");

      fprintf (file,
               "#declare TransVec2=vnormalize(vcross(TransVec1,CamSky));\n");
      fprintf (file,
               "#declare TransVec3=vnormalize(vcross(TransVec1,TransVec2));\n");
      fprintf (file, "#declare X%d =\n", i + 1);
      ut_string_string (CsysData.Label[i], &tmp);
      ut_string_fnrs (tmp, "_", "", 1);
      ut_string_fnrs (tmp, "$", "", INT_MAX);
      fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0, 0\n", tmp);
      fprintf (file, "pigment {rgb<0,0,0>}\n");
      fprintf (file, "scale %f\n", 0.1 * CsysData.FontSize);
      fprintf (file,
               "Shear_Trans(TransVec2,-TransVec3,TransVec1) translate TextLowerLeft\n");
      fprintf (file, "}\n");
      fprintf (file, "#declare Min = min_extent ( X%d );\n", i + 1);
      fprintf (file, "#declare Max = max_extent ( X%d );\n", i + 1);
      fprintf (file, "object { X%d translate (Min - Max) / 2 + <%f,%f,%f>}\n",
               i + 1, s[i][0], s[i][1], s[i][2]);
    }

  ut_free_1d (&data);
  ut_free_1d (&O);
  ol_g_free (g);
  ol_g_free (s);
  ut_free_1d_char (&edge_rad);
  ut_free_1d_char (&edge_texture);
  ut_free_1d_char (&tmp);

  return;
}
