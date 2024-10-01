/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_header_.h"

void
nev_print_png_header (struct IN_V In, struct PRINT Print, FILE * file)
{
  int i;
  int *rgb = ut_alloc_1d_int (3);

  if (!ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects"))
  {
    fprintf (file, "#version 3.7;\n");
    fprintf (file, "#include \"shapes.inc\"\n");

    if (In.scenebackground)
    {
      int *rgb = ut_alloc_1d_int (3);
      ut_color_name_rgb (In.scenebackground, rgb);
      fprintf (file, "background {color rgb<%f,%f,%f>}\n", rgb[0] / 255.,
               rgb[1] / 255., rgb[2] / 255.);
      ut_free_1d_int (&rgb);
    }

    fprintf (file, "camera {\n");
    fprintf (file, "%s\n", Print.cameraprojection);

    fprintf (file, "right x*image_width/image_height\n");
    fprintf (file, "location <%f, %f, %f>\n", Print.cameracoo[0],
             Print.cameracoo[2], Print.cameracoo[1]);

    fprintf (file, "look_at <%f, %f, %f>\n", Print.cameralookat[0],
             Print.cameralookat[2], Print.cameralookat[1]);

    fprintf (file, "angle %f\n", Print.cameraangle);

    fprintf (file, "sky <%f, %f, %f>\n", Print.camerasky[0],
             Print.camerasky[2], Print.camerasky[1]);

    fprintf (file, "}\n");

    for (i = 0; i < Print.lightsourceqty; i++)
      if (!strncmp (Print.lightsources[i], "point", 5))
      {
        int size;
        char *fct = NULL, **vars = NULL, **vals = NULL;

        ut_string_function (Print.lightsources[i], &fct, &vars, &vals, &size);

        int j;
        int shadow = 0;
        double *coo = ut_alloc_1d (3);
        char *col = NULL;
        ut_string_string ("white", &col);

        for (j = 0; j < size; j++)
        {
          if (!strcmp (vars[j], "coo"))
          {
            if (!strcmp (vals[j], "camera"))
              ut_array_1d_memcpy (Print.cameracoo, 3, coo);
            else
            {
              int qty, k;
              char **tmp = NULL;
              ut_list_break (vals[j], NEUT_SEP_DEP, &tmp, &qty);
              if (qty != 3)
                ut_print_exprbug (vals[j]);
              for (k = 0; k < 3; k++)
                coo[k] = atof (tmp[k]);
              ut_free_2d_char (&tmp, qty);
            }
          }
          else if (!strcmp (vars[j], "col"))
            ut_string_string (vals[j], &col);
          else if (!strcmp (vars[j], "shadow"))
            shadow = atoi (vals[j]);
          else
            ut_print_exprbug (vals[j]);
        }

        ut_color_name_rgb (col, rgb);
        fprintf (file,
                 "light_source { <%f, %f, %f> rgb<%f, %f, %f> %s}\n\n",
                 coo[0], coo[2], coo[1], rgb[0] / 255., rgb[1] / 255., rgb[2] / 255.,
                 !shadow ? "shadowless" : "");

        ut_free_1d (&coo);
        ut_free_1d_char (&col);
        ut_free_2d_char (&vars, size);
        ut_free_2d_char (&vals, size);
      }

    fprintf (file, "global_settings { assumed_gamma 2.2 }\n");
  }

  fprintf (file, "union {\n\n");

  ut_free_1d_int (&rgb);

  return;
}
