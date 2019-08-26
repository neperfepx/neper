/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_header_.h"

void
nev_print_header (FILE * file, struct PRINT Print)
{
  if (!ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov:objects"))
  {
    fprintf (file, "#include \"shapes.inc\"\n");

    if (Print.imagebackground)
    {
      int *rgb = ut_alloc_1d_int (3);
      ut_color_name_rgb (Print.imagebackground, rgb);
      fprintf (file, "background {color rgb<%f,%f,%f>}\n",
               rgb[0] / 255., rgb[1] / 255., rgb[2] / 255.);
      ut_free_1d_int (rgb);
    }

    fprintf (file, "camera {\n");
    fprintf (file, "%s\n", Print.cameraprojection);

    fprintf (file, "right x*image_width/image_height\n");
    fprintf (file, "location <%f, %f, %f>\n",
             Print.cameracoo[0], Print.cameracoo[2], Print.cameracoo[1]);

    fprintf (file, "look_at <%f, %f, %f>\n",
             Print.cameralookat[0],
             Print.cameralookat[2], Print.cameralookat[1]);

    fprintf (file, "angle %f\n", Print.cameraangle);

    fprintf (file, "sky <%f, %f, %f>\n",
             Print.camerasky[0], Print.camerasky[2], Print.camerasky[1]);

    fprintf (file, "}\n");

    if (Print.showshadow == 1)
      fprintf (file,
               "light_source { <%f, %f, %f> rgb<1, 1, 1>  shadowless }\n\n",
               Print.cameracoo[0], Print.cameracoo[2], Print.cameracoo[1]);
  }

  fprintf (file, "union {\n\n");

  return;
}
