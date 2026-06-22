/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_print_.h"

void
neut_print_set_default (struct PRINT *pPrint)
{
  (*pPrint).colormode = NULL;
  ut_string_string ("bright", &(*pPrint).colormode);

  (*pPrint).background = NULL;
  ut_string_string ("default", &(*pPrint).background);

  // camera ------------------------------------------------------------
  (*pPrint).cameracoo = ut_alloc_1d (3);
  (*pPrint).cameralookat = ut_alloc_1d (3);
  (*pPrint).camerasky = ut_alloc_1d (3);
  (*pPrint).cameraangle = 25;
  (*pPrint).cameraprojection = ut_alloc_1d_char (100);

  // light ------------------------------------------------------------
  (*pPrint).lightsourceqty = 0;
  (*pPrint).lightsources = NULL;
  (*pPrint).lightambient = 0;
  (*pPrint).lightdiffuse = 0;
  (*pPrint).lightreflection = 0;

  (*pPrint).showtess = -1;
  (*pPrint).showtesr = -1;
  (*pPrint).showmesh = -1;
  (*pPrint).showslice = -1;
  (*pPrint).showsliceelt1d = -1;

  (*pPrint).inputqty = 0;
  (*pPrint).inputs = NULL;

  (*pPrint).showseed = ut_alloc_1d_int (1);
  (*pPrint).showcrystal = ut_alloc_1d_int (1);
  (*pPrint).showver = ut_alloc_1d_int (1);
  (*pPrint).showedge = ut_alloc_1d_int (1);
  (*pPrint).showface = ut_alloc_1d_int (1);
  (*pPrint).showfaceinter = 0;
  (*pPrint).showpoly = ut_alloc_1d_int (1);
  (*pPrint).shownode = ut_alloc_1d_int (1);
  (*pPrint).showelset0d = ut_alloc_1d_int (1);
  (*pPrint).showelset1d = ut_alloc_1d_int (1);
  (*pPrint).showelset2d = ut_alloc_1d_int (1);
  (*pPrint).showelset3d = ut_alloc_1d_int (1);
  (*pPrint).showelt0d = ut_alloc_1d_int (1);
  (*pPrint).showelt1d = ut_alloc_1d_int (1);
  (*pPrint).showelt2d = ut_alloc_1d_int (1);
  (*pPrint).showelt3d = ut_alloc_1d_int (1);
  (*pPrint).showpoint = NULL;
  (*pPrint).showcrystal[0] = -1;
  (*pPrint).showseed[0] = -1;
  (*pPrint).showver[0] = -1;
  (*pPrint).showedge[0] = -1;
  (*pPrint).showface[0] = -1;
  (*pPrint).showpoly[0] = -1;
  (*pPrint).shownode[0] = -1;
  (*pPrint).showelt0d[0] = -1;
  (*pPrint).showelt1d[0] = -1;
  (*pPrint).showelt2d[0] = -1;
  (*pPrint).showelt3d[0] = -1;
  (*pPrint).showelset0d[0] = -1;
  (*pPrint).showelset1d[0] = -1;
  (*pPrint).showelset2d[0] = -1;
  (*pPrint).showelset3d[0] = -1;
  (*pPrint).showcsys = -1;
  (*pPrint).showvox = NULL;
  (*pPrint).showvoxstring = NULL;
  (*pPrint).showvoidvoxstring = NULL;
  (*pPrint).showedgestring = NULL;
  (*pPrint).showscale = -1;
  (*pPrint).showscalemin = -1;
  (*pPrint).showscalemax = -1;

  (*pPrint).datareduction = 1;

  return;
}

void
neut_print_free (struct PRINT *pPrint)
{
  ut_free_1d_char (&(*pPrint).colormode);

  ut_free_1d_char (&(*pPrint).background);

  // camera ------------------------------------------------------------
  ut_free_1d (&(*pPrint).cameracoo);

  ut_free_1d (&(*pPrint).cameralookat);

  ut_free_1d (&(*pPrint).camerasky);

  ut_free_1d_char (&(*pPrint).cameraprojection);

  // light ------------------------------------------------------------
  ut_free_2d_char (&(*pPrint).lightsources, (*pPrint).lightsourceqty);

  // image -------------------------------------------------------------
  ut_free_1d_int (&(*pPrint).showseed);
  ut_free_1d_int (&(*pPrint).showcrystal);
  ut_free_1d_int (&(*pPrint).showver);
  ut_free_1d_int (&(*pPrint).showedge);
  ut_free_1d_int (&(*pPrint).showface);
  ut_free_1d_int (&(*pPrint).showpoly);
  ut_free_1d_int (&(*pPrint).shownode);
  ut_free_1d_int (&(*pPrint).showelt3d);
  ut_free_1d_int (&(*pPrint).showelt2d);
  ut_free_1d_int (&(*pPrint).showelt1d);
  ut_free_1d_int (&(*pPrint).showelt0d);
  ut_free_2d_int (&(*pPrint).showpoint, (*pPrint).inputqty);
  ut_free_2d_char (&(*pPrint).inputs, (*pPrint).inputqty);

  return;
}

void
neut_print_outdir (char *outdir, struct SIM Sim, char *format, char **pdir)
{
  int i, qty;
  char **parts = NULL;

  ut_list_break (outdir, NEUT_SEP_NODEP, &parts, &qty);

  ut_string_string (".", pdir);

  for (i = 0; i < qty; i++)
  {
    if (!strcmp (parts[i], "sim_dir"))
    {
      if (!neut_sim_isvoid (Sim))
      {
        (*pdir) = ut_string_paste3 (Sim.simdir, "/images/", format);
        break;
      }
    }

    else
    {
      ut_string_string (parts[i], pdir);
      break;
    }
  }

  if (strcmp (outdir, "."))
    ut_sys_mkdir (outdir);

  ut_free_2d_char (&parts, qty);

  return;
}

void
neut_print_imagesize (char *imagesize, int *pwidth, int *pheight)
{
  int valqty;
  char **vals = NULL;

  ut_list_break (imagesize, NEUT_SEP_DEP, &vals, &valqty);
  if (valqty != 2)
    ut_print_message (2, 2, "Expression `%s' could not be processed.\n",
                      imagesize);
  if (pwidth)
    ut_string_int (vals[0], pwidth);
  if (pheight)
    ut_string_int (vals[1], pheight);
  ut_free_2d_char (&vals, valqty);

  return;
}

int
neut_print_cell_show (struct PRINT Print, struct TESS Tess, int cell)
{
  if (Tess.Dim == 2)
    return Print.showface[cell];
  else if (Tess.Dim == 3)
    return Print.showpoly[cell];
  else
    abort ();
}

void
neut_print_povheader (char *format, struct PRINT Print, FILE * file)
{
  int i;
  int *rgb = ut_alloc_1d_int (3);

  if (!ut_list_testelt (format, NEUT_SEP_NODEP, "pov:objects"))
  {
    fprintf (file, "#version 3.7;\n");
    fprintf (file, "#include \"shapes.inc\"\n");

    if (Print.background)
    {
      int *rgb = ut_alloc_1d_int (3);
      ut_color_name_rgb (Print.background, rgb);
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

void
neut_print_povfoot (char *includepov, char *imageformat, FILE * file)
{
  int i, j, k, povqty, *povqty2 = NULL, argqty;
  double tmp;
  char ***povs = NULL;
  FILE *filein = NULL;
  char *line = ut_alloc_1d_char (1000);
  char *fct = NULL, **vars = NULL, **vals = NULL;

  if (includepov)
  {
    ut_list_break2 (includepov, NEUT_SEP_NODEP, NEUT_SEP_DEP, &povs,
                    &povqty2, &povqty);

    for (i = 0; i < povqty; i++)
    {
      filein = ut_file_open (povs[i][0], "R");

      fprintf (file, "\n");
      fprintf (file, "// Including file %s ---------\n", povs[i][0]);

      for (j = 2; j <= povqty2[i]; j++)
        fprintf (file, "union {\n\n");

      while (fgets (line, 1000, filein) != NULL)
        fprintf (file, "%s", line);
      fprintf (file, "// Included file %s ---------\n", povs[i][0]);

      for (j = 2; j <= povqty2[i]; j++)
      {
        ut_string_function (povs[i][j - 1], &fct, &vars, &vals, &argqty);

        fprintf (file, "\n%s<", fct);
        for (k = 0; k < argqty; k++)
        {
          ut_math_eval (vals[k], 0, NULL, NULL, &tmp);
          fprintf (file, REAL_PRINT_FORMAT "%s", tmp, k < argqty - 1 ? "," : ">\n");
        }
        fprintf (file, "}\n");

        ut_free_2d_char (&vars, argqty);
        ut_free_2d_char (&vals, argqty);
      }
      fprintf (file, "\n");

      ut_file_close (filein, povs[i][0], "R");
    }
  }

  if (!ut_list_testelt (imageformat, NEUT_SEP_NODEP, "pov:objects"))
  {
    fprintf (file, "rotate<-90,  0,  0>\n");
    fprintf (file, "scale <  1,  1, -1>\n");
  }
  fprintf (file, "}\n");

  // ut_free_3d_char (&povs, povqty);
  ut_free_1d_int (&povqty2);
  ut_free_1d_char (&line);

  return;
}

void
neut_print_pov2png (char *povray, char *filename, int imagewidth,
                   int imageheight, int antialiasing, int messagetag)
{
  char *pngfilename = NULL;
  char *command = ut_alloc_1d_char (1000);

  neut_povray_check (povray);

  ut_string_string (filename, &pngfilename);
  ut_string_fnrs (pngfilename, ".pov", ".png", 1);

  if (messagetag >= 0)
    ut_print_message (0, messagetag,
                      "Generating png file (%dx%d pixels)...\n", imagewidth,
                      imageheight);

  remove (pngfilename);

  ut_file_openmessage (pngfilename, messagetag >= 0 ? "w" : "W");

  sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D %s 2>/dev/null",
           povray, filename, pngfilename, imagewidth, imageheight,
           antialiasing > 0 ? "+A0.2" : "");

  if (system (command) == -1 || !ut_file_exist (pngfilename))
  {
    ut_print_message (2, 3, "File `%s' could not be generated!\n",
                      pngfilename);

    sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D %s", povray,
             filename, pngfilename, imagewidth, imageheight,
             antialiasing > 0 ? "+A0.2" : "");

    // otherwise, printing debugging info.
    ut_print_message (2, 3,
                      "To debug: use option `-imageformat pov' and run command `%s'.\n",
                      command);
  }

  ut_file_closemessage (pngfilename, messagetag >= 0 ? "w" : "W");

  ut_free_1d_char (&pngfilename);
  ut_free_1d_char (&command);

  return;
}
