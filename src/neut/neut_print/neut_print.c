/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_print_.h"

void
neut_print_set_default (struct PRINT *pPrint)
{
  // space -------------------------------------------------------------
  (*pPrint).space = NULL;
  ut_string_string ("real", &((*pPrint).space));

  // pf ----------------------------------------------------------------
  (*pPrint).pfpolestring = NULL;
  ut_string_string ("1:1:1", &(*pPrint).pfpolestring);
  (*pPrint).pfpoles = NULL;
  (*pPrint).pfpoleqty = 0;
  (*pPrint).pfprojection = NULL;
  ut_string_string ("stereographic", &(*pPrint).pfprojection);
  (*pPrint).pfmode = NULL;
  ut_string_string ("symbol", &(*pPrint).pfmode);
  (*pPrint).pffont = NULL;
  ut_string_string ("TimesRoman", &(*pPrint).pffont);

  (*pPrint).pfsym = NULL;
  ut_string_string ("monoclinic", &(*pPrint).pfsym);

  (*pPrint).pfclustering = 1;

  (*pPrint).pfgridsize = 200;
  (*pPrint).pfkernel = NULL;
  ut_string_string ("normal(3)", &(*pPrint).pfkernel);
  (*pPrint).pfkernelsig = 3;

  (*pPrint).pfdirstring = NULL;
  ut_string_string ("y:-x", &(*pPrint).pfdirstring);
  (*pPrint).pfdir = NULL;

  (*pPrint).pfshape = NULL;
  ut_string_string ("full", &(*pPrint).pfshape);

  // povray ------------------------------------------------------------
  (*pPrint).povray = NULL;
  ut_string_string ("povray", &((*pPrint).povray));

  // asymptote ------------------------------------------------------------
  (*pPrint).asymptote = NULL;
  ut_string_string ("asy", &((*pPrint).asymptote));

  // camera ------------------------------------------------------------
  (*pPrint).cameracoostring = ut_alloc_1d_char (100);
  sprintf ((*pPrint).cameracoostring, "x+length*vx%sy+length*vy%sz+length*vz",
           NEUT_SEP_DEP, NEUT_SEP_DEP);
  (*pPrint).cameracoo = ut_alloc_1d (3);

  (*pPrint).cameralookatstring = ut_alloc_1d_char (100);
  sprintf ((*pPrint).cameralookatstring, "x%sy%sz", NEUT_SEP_DEP,
           NEUT_SEP_DEP);
  (*pPrint).cameralookat = ut_alloc_1d (3);

  (*pPrint).cameraskystring = ut_alloc_1d_char (100);
  ut_string_string ("default", &(*pPrint).cameraskystring);
  (*pPrint).camerasky = ut_alloc_1d (3);

  (*pPrint).cameraangle = 25;

  (*pPrint).cameraprojection = ut_alloc_1d_char (100);
  strcpy ((*pPrint).cameraprojection, "default");

  // light ------------------------------------------------------------
  (*pPrint).lightsourcestring = NULL;
  ut_string_string ("default", &(*pPrint).lightsourcestring);
  (*pPrint).lightsourceqty = 0;
  (*pPrint).lightsources = NULL;
  (*pPrint).lightambientstring = NULL;
  ut_string_string ("default", &(*pPrint).lightambientstring);
  (*pPrint).lightdiffusestring = NULL;
  ut_string_string ("default", &(*pPrint).lightdiffusestring);
  (*pPrint).lightreflectionstring = NULL;
  ut_string_string ("default", &(*pPrint).lightreflectionstring);
  (*pPrint).lightambient = 0;
  (*pPrint).lightdiffuse = 0;
  (*pPrint).lightreflection = 0;

  // image -------------------------------------------------------------
  (*pPrint).imagesize = ut_alloc_1d_char (100);
  sprintf ((*pPrint).imagesize, "1200%s900", NEUT_SEP_DEP);
  (*pPrint).povrayantialiasing = 1;

  (*pPrint).showtess = -1;
  (*pPrint).showtesr = -1;
  (*pPrint).showmesh = -1;
  (*pPrint).showslice = -1;

  (*pPrint).inputqty = 0;
  (*pPrint).inputs = NULL;

  (*pPrint).showseed = ut_alloc_1d_int (1);
  (*pPrint).showcrystal = ut_alloc_1d_int (1);
  (*pPrint).showver = ut_alloc_1d_int (1);
  (*pPrint).showedge = ut_alloc_1d_int (1);
  (*pPrint).showface = ut_alloc_1d_int (1);
  (*pPrint).showfaceinter = 0;
  (*pPrint).showpoly = ut_alloc_1d_int (1);
  (*pPrint).showelt3d = ut_alloc_1d_int (1);
  (*pPrint).shownode = ut_alloc_1d_int (1);
  (*pPrint).showelt0d = ut_alloc_1d_int (1);
  (*pPrint).showelt1d = ut_alloc_1d_int (1);
  (*pPrint).showelt2d = ut_alloc_1d_int (1);
  (*pPrint).showpoint = NULL;
  (*pPrint).showcrystal[0] = -1;
  (*pPrint).showseed[0] = -1;
  (*pPrint).showver[0] = -1;
  (*pPrint).showedge[0] = -1;
  (*pPrint).showface[0] = -1;
  (*pPrint).showpoly[0] = -1;
  (*pPrint).showelt3d[0] = -1;
  (*pPrint).shownode[0] = -1;
  (*pPrint).showelt0d[0] = -1;
  (*pPrint).showelt1d[0] = -1;
  (*pPrint).showelt2d[0] = -1;
  (*pPrint).showcsys = -1;
  (*pPrint).showvox = NULL;
  (*pPrint).showvoxstring = NULL;
  (*pPrint).showvoidvoxstring = NULL;
  (*pPrint).showedgestring = NULL;
  (*pPrint).scenebackground = NULL;
  (*pPrint).showscale = -1;

  ut_string_string ("white", &(*pPrint).scenebackground);

  (*pPrint).datareduction = 1;

  (*pPrint).imageformat = ut_alloc_1d_char (10);
  sprintf ((*pPrint).imageformat, "png");

  (*pPrint).includepov = NULL;

  (*pPrint).slice = NULL;

  (*pPrint).outdir = NULL;
  ut_string_string (".", &(*pPrint).outdir);

  return;
}

void
neut_print_free (struct PRINT *pPrint)
{
  ut_free_1d_char (&(*pPrint).space);

  // camera ------------------------------------------------------------
  ut_free_1d_char (&(*pPrint).cameracoostring);
  ut_free_1d (&(*pPrint).cameracoo);

  ut_free_1d_char (&(*pPrint).cameralookatstring);
  ut_free_1d (&(*pPrint).cameralookat);

  ut_free_1d_char (&(*pPrint).cameraskystring);
  ut_free_1d (&(*pPrint).camerasky);

  ut_free_1d_char (&(*pPrint).cameraprojection);

  // light ------------------------------------------------------------
  ut_free_1d_char (&(*pPrint).lightsourcestring);
  ut_free_2d_char (&(*pPrint).lightsources, (*pPrint).lightsourceqty);
  ut_free_1d_char (&(*pPrint).lightambientstring);
  ut_free_1d_char (&(*pPrint).lightdiffusestring);
  ut_free_1d_char (&(*pPrint).lightreflectionstring);

  // image -------------------------------------------------------------
  ut_free_1d_char (&(*pPrint).imagesize);
  ut_free_1d_char (&(*pPrint).scenebackground);
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

  ut_free_1d_char (&(*pPrint).imageformat);
  ut_free_1d_char (&(*pPrint).includepov);

  return;
}

void
neut_print_outdir (struct PRINT Print, struct SIM Sim, char *format, char **pdir)
{
  int i, qty;
  char **parts = NULL;

  ut_list_break (Print.outdir, NEUT_SEP_NODEP, &parts, &qty);

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

  ut_free_2d_char (&parts, qty);

  return;
}

void
neut_print_imagesize (struct PRINT Print, int *pwidth, int *pheight)
{
  int valqty;
  char **vals = NULL;

  ut_list_break (Print.imagesize, NEUT_SEP_DEP, &vals, &valqty);
  if (valqty != 2)
    ut_print_message (2, 2, "Expression `%s' could not be processed.\n",
                      Print.imagesize);
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
